#version 460

in vec3 Position;
in vec3 Normal;
in vec2 TexCoord;
in vec3 Tangent;
in vec3 Bitangent;
in vec3 VecPos;

layout (binding = 3) uniform sampler2D Tex1;
layout (binding = 4) uniform sampler2D Tex2;
layout (binding = 5) uniform sampler2D NormalTex;

layout (binding = 6) uniform samplerCube SkyBoxTex;

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
uniform float mixLevel = 0.5;
uniform bool DoNormalMap = true;

uniform bool IsSkyBox;

const int levels = 3;
const float scaleFactor = 1.0 / levels;

vec3 BlinnPhongModel(vec3 pos, vec3 n) {
    vec3 ambientBase, diffuseBase;
    if (IsTextured) {
        vec4 texColor1 = texture(Tex1, TexCoord);
        vec4 texColor2 = texture(Tex2, TexCoord);
        vec3 texColor = mix(texColor1.rgb, texColor2.rgb, mixLevel);
        ambientBase = texColor;
        diffuseBase = texColor;
    } else {
        ambientBase = Material.Ka;
        diffuseBase = Material.Kd;
    }

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

vec4 pass1() {
    if (IsSkyBox) {
        vec3 skyTexColor = texture(SkyBoxTex, normalize(VecPos)).rgb;
        skyTexColor = pow(skyTexColor, vec3(1.0 / 2.2));
        return vec4(skyTexColor, 1.0);
    }

    vec3 T = normalize(Tangent);
    vec3 B = normalize(Bitangent);
    vec3 N = normalize(Normal);
    mat3 TBN = mat3(T, B, N);

    vec3 normalMap = texture(NormalTex, TexCoord).rgb;
    normalMap = normalize(normalMap * 2.0 - 1.0);
    vec3 worldNormal;

    worldNormal = Normal;

    if (IsTextured)
        return vec4(BlinnPhongModel(Position, worldNormal), 1.0);
    else
        return vec4(BlinnPhongModel(Position, Normal), 1.0);
}


void main() {
    FragColor = pass1();
}
