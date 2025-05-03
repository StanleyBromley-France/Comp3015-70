#version 460

layout(location = 0) in vec3 VertexPosition;

out vec2 TexCoord;

uniform mat4 MVP;
uniform float TextureScale; // controls tiling density (e.g., 1.0 = 1 texture per world unit)

void main()
{
    gl_Position = MVP * vec4(VertexPosition, 1.0);
    
    // calculates UVs based on world position with scaling
    TexCoord = VertexPosition.xz * TextureScale; // uses XZ plane for ground textures
}