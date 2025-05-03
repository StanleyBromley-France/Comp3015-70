#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;


#include "shader/includes/lighting_common.frag"

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
    //FragColor = vec4(fract(TexCoord.x), fract(TexCoord.y), 0.0, 1.0);
}
