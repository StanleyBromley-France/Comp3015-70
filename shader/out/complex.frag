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

layout(std140, binding = 0) uniform GlobalSettings {
    SpotLightInfo Spotlight;

    int   lightingMode;
    vec3  pad6;
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

vec3 calculateAmbient(vec3 ambientBase)
{
    return globalSettings.Spotlight.La * ambientBase;
}

SpotlightParams computeSpotlightParams(vec3 pos) {
    SpotlightParams params;
    params.s = normalize(globalSettings.Spotlight.Position - pos);
    params.cosAng = dot(-params.s, normalize(globalSettings.Spotlight.Direction));
    params.spotScale = 0.0;
    
    if (params.cosAng > 0.0 && acos(params.cosAng) < globalSettings.Spotlight.Cutoff) {
        params.spotScale = pow(params.cosAng, globalSettings.Spotlight.Exponent);
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

vec3 computeFinalColor(vec3 ambient, float spotScale, vec3 diffuse, vec3 spec) {
    return ambient + spotScale * (diffuse + spec) * globalSettings.Spotlight.L;
}

//------------ MAIN METHODS ------------//

// normal lighting
vec3 BlinnPhong_LightingNormal(
    vec3 pos, 
    vec3 n, 
    vec3 ambientBase, 
    vec3 diffuseBase 
) {
    vec3 ambient = calculateAmbient(ambientBase);
    SpotlightParams params = computeSpotlightParams(pos);

    if (params.spotScale > 0.0) {
        float sDotN = max(dot(params.s, n), 0.0);
        vec3 diffuse = diffuseBase * sDotN;
        vec3 spec = (useSpecular && sDotN > 0.0) ? calculateSpecular(pos, params.s, n) : vec3(0.0);
        return computeFinalColor(ambient, params.spotScale, diffuse, spec);
    }
    
    return ambient;
}

// toon lighting
vec3 BlinnPhong_LightingToon(
    vec3 pos, 
    vec3 n, 
    vec3 ambientBase, 
    vec3 diffuseBase
) {
    vec3 ambient = calculateAmbient(ambientBase);
    SpotlightParams params = computeSpotlightParams(pos);

    if (params.spotScale > 0.0) {
        float sDotN = max(dot(params.s, n), 0.0);
        float band = floor(sDotN * levels);
        vec3 diffuse = diffuseBase * band * scaleFactor;
        vec3 spec = (sDotN > 0.0) ? calculateSpecular(pos, params.s, n) : vec3(0.0);
        return computeFinalColor(ambient, params.spotScale, diffuse, spec);
    }
    
    return ambient;
}


uniform sampler2D Tex1;
uniform sampler2D Tex2;
uniform sampler2D NormalTex;



layout (location = 0) out vec4 FragColor;

//------------ UTILITY FUNCTIONS ------------//

vec3 calculateColour()
{
    vec4 texColor1 =  texture(Tex1, TexCoord);
    vec4 texColor2 =  texture(Tex2, TexCoord);

    vec3 texColor = mix(texColor1.rgb, texColor2.rgb, 0.5);
    return texColor;
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
vec3 applyLighting(vec3 pos, vec3 n, vec3 ambientBase, vec3 diffuseBase) 
{
    if (globalSettings.lightingMode == 0) {
        return BlinnPhong_LightingNormal(pos, n, ambientBase, diffuseBase);
    } else {
        return BlinnPhong_LightingToon(pos, n, ambientBase, diffuseBase);
    }
}

void main() {
    // calculates TBN matrix
    mat3 TBN = CalculateTBN();
    
    // samples normal map and transform from [0,1] to [-1,1]
    vec3 tangentNormal = texture(NormalTex, TexCoord).rgb;
    tangentNormal = normalize(tangentNormal * 2.0 - 1.0);
    
    // converts normal from tangent space to VIEW space
    vec3 viewNormal = normalize(TBN * tangentNormal);
    
    vec3 finalTexColor = calculateColour();
    vec3 color = applyLighting(Position, viewNormal, finalTexColor, finalTexColor);
    
    FragColor = vec4(color, 1.0);
}
