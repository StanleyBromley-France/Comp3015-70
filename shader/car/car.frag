#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Tangent;
in vec3 Bitangent;
in vec3 VecPos;

#include "shader/includes/lighting_common.frag"

layout (binding = 3) uniform sampler2D Tex1;
layout (binding = 4) uniform sampler2D Tex2;
layout (binding = 5) uniform sampler2D NormalTex;

layout (location = 0) out vec4 FragColor;

//------------ UTILITY FUNCTIONS ------------//

vec3 calculateColour()
{

    vec4 texColor1 =  texture(Tex1, TexCoord);
    vec4 texColor2 =  texture(Tex2, TexCoord);

    vec3 texColor = mix(texColor1.rgb, texColor2.rgb, 0.5);
    return texColor;
}



void main() {
    // calculates the TBN matrix
    vec3 T = normalize(Tangent);
    vec3 B = normalize(Bitangent);
    vec3 N = normalize(Normal);
    mat3 TBN = mat3(T, B, N);

    // samples the normal map and transform to world space
    vec3 normalMap = texture(NormalTex, TexCoord).rgb;
    normalMap = normalize(normalMap * 2.0 - 1.0); // converts from [0,1] to [-1,1]
    
    vec3 worldNormal = normalize(TBN * normalMap);

    vec3 finalTexColour = calculateColour();
    vec3 color = applyLighting(Position, Normal, finalTexColour, finalTexColour);

    FragColor = vec4(color, 1.0);
}
