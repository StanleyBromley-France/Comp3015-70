#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 ModelViewMatrix;
uniform mat3 NormalMatrix;
uniform mat4 MVP;

#define MAX_SHADOWS 4
uniform mat4 ShadowMatrices[MAX_SHADOWS];
uniform int numShadows;
out int NumShadows;

out vec4 ShadowCoord[MAX_SHADOWS];

uniform mat4 Model;

void main()
{
    TexCoord = VertexTexCoord;
    Normal = normalize(NormalMatrix * VertexNormal);
    Position = (ModelViewMatrix * vec4(VertexPosition, 1.0)).xyz;

    for(int i = 0; i < numShadows; ++i) {
    ShadowCoord[i] = ShadowMatrices[i] * Model * vec4(VertexPosition, 1.0);
    }
    NumShadows = numShadows;
    gl_Position = MVP * vec4(VertexPosition, 1.0);
}