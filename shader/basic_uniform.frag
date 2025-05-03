#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Tangent;
in vec3 Bitangent;
in vec3 VecPos;

layout (location = 0) out vec4 FragColor;

struct SpotLightInfo{
    vec3 Position;  float pad0;
    vec3 La;        float pad1;
    vec3 L;         float pad2;
    vec3 Direction; float pad3;
    float Exponent;
    float Cutoff;
};

struct MaterialInfo {
    vec3 Kd;
    vec3 Ka;
    vec3 Ks;
    float Shininess;
};

uniform MaterialInfo Material;

layout(std140, binding = 0) uniform GlobalSettings {
    SpotLightInfo Spotlight;

    int   lightingMode;
    vec3  pad6;
} globalSettings;

uniform bool IsTextured;
uniform bool IsToonLighting;


const int levels = 3;
const float scaleFactor = 1.0 / levels;

vec3 BlinnPhongModel(vec3 pos, vec3 n) {
    vec3 ambientBase, diffuseBase;
    
    ambientBase = Material.Ka;
    diffuseBase = Material.Kd;

    vec3 diffuse = vec3(0), spec = vec3(0);
    vec3 ambient = globalSettings.Spotlight.La * ambientBase;

    vec3 s = normalize(globalSettings.Spotlight.Position - pos);
    float cosAng = dot(-s, normalize(globalSettings.Spotlight.Direction));
    float angle = acos(cosAng);
    float spotScale = 0.0;

    if (angle >= 0.0 && angle < globalSettings.Spotlight.Cutoff) {
        spotScale = pow(cosAng, globalSettings.Spotlight.Exponent);
        float sDotN = max(dot(s, n), 0.0);

        if (IsToonLighting)
            diffuse = diffuseBase * floor(sDotN * levels) * scaleFactor;
        else
            diffuse = diffuseBase * sDotN;

        if (sDotN > 0.0) {
            vec3 v = normalize(-pos.xyz);
            vec3 h = normalize(v + s);
            spec = Material.Ks * pow(max(dot(h, n), 0.0), Material.Shininess);
        }
    }

    return ambient + spotScale * (diffuse + spec) * globalSettings.Spotlight.L;
}

void main() {
    FragColor = vec4(BlinnPhongModel(Position, Normal), 1.0);
}
