//------------ DATA ------------//

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

//------------ UNIFORMS ------------//

uniform MaterialInfo Material;

layout(std140, binding = 0) uniform GlobalSettings {
    SpotLightInfo Spotlight;

    int   lightingMode;
    vec3  pad6;
} globalSettings;