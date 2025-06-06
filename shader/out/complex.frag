#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

//------------ DATA ------------//

struct SpotLightInfo{
    vec3 Position;  float pad0;
    vec3 La;        float pad1;
    vec3 L;         float pad2;
    vec3 Direction; float pad3;
    float Exponent;
    float Cutoff;
};

struct MaterialInfo {
    vec3 Kd;
    vec3 Ka;
    vec3 Ks;
    float Shininess;
};

//------------ UNIFORMS ------------//

uniform MaterialInfo Material;

#define MAX_SPOTLIGHTS 8

layout(std140, binding = 0) uniform GlobalSettings {
    SpotLightInfo Spotlights[MAX_SPOTLIGHTS];
    int numSpotlights;
    int lightingMode;
    vec2 pad2;
} globalSettings;

//------------ DATA ------------//

struct SpotlightParams {
    vec3 s;
    float cosAng;
    float spotScale;
};

//------------ TOON SHADING VARS ------------//

const int levels = 3;
const float scaleFactor = 1.0 / levels;

//------------ HELPER METHODS ------------//

vec3 calculateAmbient(SpotLightInfo sp, vec3 ambientBase)
{
    return sp.La * ambientBase;
}

SpotlightParams computeSpotlightParams(SpotLightInfo sp, vec3 pos) {
    SpotlightParams params;
    params.s = normalize(sp.Position - pos);
    params.cosAng = dot(-params.s, normalize(sp.Direction));
    params.spotScale = 0.0;
    
    if (params.cosAng > 0.0 && acos(params.cosAng) < sp.Cutoff) {
        params.spotScale = pow(params.cosAng, sp.Exponent);
    }
    
    return params;
}

uniform bool useSpecular;

vec3 calculateSpecular(vec3 pos, vec3 s, vec3 n) {
    vec3 v = normalize(-pos);
    vec3 h = normalize(v + s);
    float hDotN = dot(h, n);
    return Material.Ks * pow(max(hDotN, 0.0), Material.Shininess);
}

vec3 computeFinalColor(SpotlightParams params, vec3 diffuse, vec3 spec, SpotLightInfo sp) {
    return params.spotScale * (diffuse + spec) * sp.L;
}

//------------ MAIN METHODS ------------//

// normal lighting
vec3 BlinnPhong_LightingNormal(
    SpotLightInfo sp,
    vec3 pos, 
    vec3 n, 
    vec3 diffuseBase 
) {
    SpotlightParams params = computeSpotlightParams(sp, pos);

    if (params.spotScale > 0.0) {
        float sDotN = max(dot(params.s, n), 0.0);
        vec3 diffuse = diffuseBase * sDotN;
        vec3 spec = (useSpecular && sDotN > 0.0)
                   ? calculateSpecular(pos, params.s, n)
                   : vec3(0.0);
        return computeFinalColor(params, diffuse, spec, sp);
    }
    
    return vec3(0);
}

// toon lighting
vec3 BlinnPhong_LightingToon(
    SpotLightInfo sp,
    vec3 pos, 
    vec3 n, 
    vec3 diffuseBase
) {
    SpotlightParams params = computeSpotlightParams(sp, pos);

    if (params.spotScale > 0.0) {
        float sDotN = max(dot(params.s, n), 0.0);
        float band = floor(sDotN * float(levels));
        vec3 diffuse = diffuseBase * band * scaleFactor;
        vec3 spec = (sDotN > 0.0)
                   ? calculateSpecular(pos, params.s, n)
                   : vec3(0.0);
        return computeFinalColor(params, diffuse, spec, sp);
    }
    
    return vec3(0);
}

#define MAX_SHADOWS 8

uniform sampler2DShadow ShadowMaps[MAX_SHADOWS];

flat in int NumShadows;

in vec4 ShadowCoord[MAX_SHADOWS];

float calculateShadow()
{
    float shadow = 1.0;
    for(int i = 0; i < NumShadows; ++i) {
        if (ShadowCoord[i].z >= 0.0) {
            float s = textureProj(ShadowMaps[i], ShadowCoord[i]);
            shadow = min(shadow, s);
        }
    }
    return shadow;
}

float calculateShadowPCF()
{
    float shadow = 1.0;
    for(int i = 0; i < NumShadows; ++i) {
        if (ShadowCoord[i].z >= 0.0) {
            
            vec4 sc = ShadowCoord[i];
            bool insideXY = sc.x >= 0.0 && sc.x <= sc.w
                            && sc.y >= 0.0 && sc.y <= sc.w;
            bool insideZ  = sc.z >= 0.0 && sc.z <= sc.w;
            if (!insideXY || !insideZ) {
                // fragment is outside light-i�s view: skip its test
                continue;
            }

            // PCF: sample 4 neighbouring texels
            float sum = 0.0;
            sum += textureProjOffset(ShadowMaps[i], ShadowCoord[i], ivec2(-1, -1));
            sum += textureProjOffset(ShadowMaps[i], ShadowCoord[i], ivec2(-1,  1));
            sum += textureProjOffset(ShadowMaps[i], ShadowCoord[i], ivec2( 1,  1));
            sum += textureProjOffset(ShadowMaps[i], ShadowCoord[i], ivec2( 1, -1));
            float avg = sum * 0.25;
            shadow = min(shadow, avg);
        }
    }
    return shadow;
}

#define MAX_TEXTURES 5
uniform sampler2D albedoArray[MAX_TEXTURES];
uniform int numTextures;

uniform sampler2D NormalTex;


layout (location = 0) out vec4 FragColor;

//------------ UTILITY FUNCTIONS ------------//

vec3 calculateColour()
{
    if (numTextures == 0) return vec3(0.0); // Handle empty case
    
    vec4 result = texture(albedoArray[0], TexCoord);
    
    for(int i = 1; i < numTextures; i++) {
        // Ensure we don't exceed array bounds
        if (i >= MAX_TEXTURES) break;
        
        vec4 next = texture(albedoArray[i], TexCoord);
        float weight = 1.0 / float(i + 1);
        result = mix(result, next, weight);
    }
    
    return result.rgb;
}


mat3 CalculateTBN()
{
    // gets screen-space derivatives
    vec3 dPosX = dFdx(Position);
    vec3 dPosY = dFdy(Position);
    vec2 dUVX = dFdx(TexCoord);
    vec2 dUVY = dFdy(TexCoord);

    // solves for TBN matrix
    mat3 M = mat3(dPosX, dPosY, cross(dPosX, dPosY));
    mat3 inverseUV = inverse(mat3(vec3(dUVX, 0), vec3(dUVY, 0), vec3(0, 0, 1)));
    mat3 TBN = M * inverseUV;

    // ensures orthogonality with normal
    vec3 N = normalize(Normal);
    vec3 T = normalize(TBN[0] - N * dot(N, TBN[0]));
    vec3 B = normalize(cross(N, T));

    return mat3(T, B, N);
}


// applies lighting based on global var
vec3 applyLighting(vec3 pos, vec3 n, vec3 diffuseBase) 
{
    vec3 result = vec3(0.0);

    // loop over all active spotlights
    for (int i = 0; i < globalSettings.numSpotlights; ++i) {
        SpotLightInfo sp = globalSettings.Spotlights[i];

        if (globalSettings.lightingMode == 0) {
            result += BlinnPhong_LightingNormal(sp, pos, n, diffuseBase);
        } else {
            result += BlinnPhong_LightingToon(sp, pos, n, diffuseBase);
        }
    }

    return result;
}

void main() {

    // calculates TBN matrix
    mat3 TBN = CalculateTBN();
    
    // samples normal map and transform from [0,1] to [-1,1]
    vec3 tangentNormal = texture(NormalTex, TexCoord).rgb;
    tangentNormal = normalize(tangentNormal * 2.0 - 1.0);
    
    // converts normal from tangent space to VIEW space
    vec3 viewNormal = normalize(TBN * tangentNormal);
    
    vec3 texColor = calculateColour();

    vec3 ambient = vec3(0.0);

    if (globalSettings.numSpotlights > 0) {
        SpotLightInfo firstSp = globalSettings.Spotlights[0];
        ambient = calculateAmbient(firstSp, texColor);
    }    
    vec3 lit = applyLighting(Position, viewNormal, texColor);

    float shadow = calculateShadowPCF();

    FragColor = vec4(lit * shadow + ambient, 1.0);
}
