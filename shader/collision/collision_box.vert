#version 460

layout (location = 0) in vec2 VertexPosition;

out vec3 VecPos;

uniform mat4 MVP;  

void main()
{
    VecPos = vec3(VertexPosition, 0.0);

    gl_Position = MVP * vec4(VertexPosition, 0.0, 1.0);
}