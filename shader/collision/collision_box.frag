#version 460

in  vec3 VecPos;
out vec4 FragColor;

uniform vec3 BoxColor = vec3(0.0, 1.0, 0.0);

void main()
{
    FragColor = vec4(BoxColor, 1.0);
}
