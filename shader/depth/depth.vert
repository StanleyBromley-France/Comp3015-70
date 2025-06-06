#version 460
layout (location = 0) in vec3 VertexPosition;

uniform mat4 model;
uniform mat4 lightSpaceMatrix;

void main() {
    gl_Position = lightSpaceMatrix * model * vec4(VertexPosition, 1.0);
}
