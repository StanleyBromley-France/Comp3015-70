#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;

#include "shader/includes/global_data.frag"

#include "shader/includes/blinnPhong_lighting.frag"

#include "shader/includes/shadows.frag"

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
