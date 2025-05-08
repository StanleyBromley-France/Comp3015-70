#version 330 core
in vec2 vUV;
out vec4 FragColor;

uniform sampler2D uSrcTex;

void main() {
    float d = texture(uSrcTex, vUV).r;
    float g = pow(clamp(d, 0.0, 1.0), 300);
    FragColor = vec4(g, g, g, 1.0);
}