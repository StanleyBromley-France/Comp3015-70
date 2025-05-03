#version 460

in vec3 VecPos;

uniform samplerCube SkyBoxTex;

layout (location = 0) out vec4 FragColor;

vec4 pass1() 
{
    vec3 skyTexColor = texture(SkyBoxTex, normalize(VecPos)).rgb;
    skyTexColor = pow(skyTexColor, vec3(1.0 / 2.2));
    return vec4(skyTexColor, 1.0);
}

void main() {
    FragColor = pass1();
}
