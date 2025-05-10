#version 460

#define PI 3.14159265

uniform vec4 Color;

uniform sampler2D NoiseTex;

uniform vec4 SkyColor   = vec4(0, 0, 0, 0);
uniform vec4 CloudColor = vec4(0.5, 0.5, 0.5, 1);

in vec2 TexCoord;

layout ( location = 0 ) out vec4 FragColor;

void main() {    
    vec4 noise = texture(NoiseTex, TexCoord);    
    float t = (cos( noise.a * PI ) + 1.0) / 2.0;    
    vec4 color = mix( SkyColor, CloudColor, t );

    FragColor = color;
}