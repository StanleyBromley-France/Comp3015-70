#version 460

#define PI 3.14159265

uniform vec4 Color;

uniform sampler2D NoiseTex;

uniform vec4 SkyColor   = vec4(0, 0, 0, 0);
uniform vec4 CloudColor = vec4(0.4, 0.4, 0.4, 1);

in vec2 TexCoord;

layout ( location = 0 ) out vec4 FragColor;

void main() {    
    float noiseA = texture(NoiseTex, TexCoord).a;

    float t = (cos(noiseA * PI) + 1.0) * 0.5;

    float a = mix(SkyColor.a, CloudColor.a, t);

    FragColor = vec4(CloudColor.rgb, a);
}