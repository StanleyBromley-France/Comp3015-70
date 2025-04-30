//------------ DATA ------------//

struct SpotLightInfo{
    vec3 Position;
    vec3 La;
    vec3 L;
    vec3 Direction;
    float Exponent;
    float Cutoff;
};

struct MaterialInfo {
    vec3 Kd;
    vec3 Ka;
    vec3 Ks;
    float Shininess;
};

struct SpotlightParams {
    vec3 s;
    float cosAng;
    float spotScale;
};

//------------ UNIFORMS ------------//

uniform MaterialInfo Material;

layout(std140, binding = 0) uniform GlobalSettings {
    SpotLightInfo Spotlight;

    int   lightingMode;
    vec3  pad6;
} globalSettings;

//------------ TOON SHADING VARS ------------//

const int levels = 3;
const float scaleFactor = 1.0 / levels;

//------------ HELPER METHODS ------------//

vec3 calculateAmbient(vec3 ambientBase)
{
    return globalSettings.Spotlight.La * ambientBase;
}

SpotlightParams computeSpotlightParams(vec3 pos) {
    SpotlightParams params;
    params.s = normalize(globalSettings.Spotlight.Position - pos);
    params.cosAng = dot(-params.s, normalize(globalSettings.Spotlight.Direction));
    params.spotScale = 0.0;
    
    if (params.cosAng > 0.0 && acos(params.cosAng) < globalSettings.Spotlight.Cutoff) {
        params.spotScale = pow(params.cosAng, globalSettings.Spotlight.Exponent);
    }
    
    return params;
}

vec3 calculateSpecular(vec3 pos, vec3 s, vec3 n) {
    vec3 v = normalize(-pos);
    vec3 h = normalize(v + s);
    float hDotN = dot(h, n);
    return Material.Ks * pow(max(hDotN, 0.0), Material.Shininess);
}

vec3 computeFinalColor(vec3 ambient, float spotScale, vec3 diffuse, vec3 spec) {
    return ambient + spotScale * (diffuse + spec) * globalSettings.Spotlight.L;
}

//------------ MAIN METHODS ------------//

// normal lighting
vec3 BlinnPhong_LightingNormal(
    vec3 pos, 
    vec3 n, 
    vec3 ambientBase, 
    vec3 diffuseBase 
) {
    vec3 ambient = calculateAmbient(ambientBase);
    SpotlightParams params = computeSpotlightParams(pos);

    if (params.spotScale > 0.0) {
        float sDotN = max(dot(params.s, n), 0.0);
        vec3 diffuse = diffuseBase * sDotN;
        vec3 spec = (sDotN > 0.0) ? calculateSpecular(pos, params.s, n) : vec3(0.0);
        return computeFinalColor(ambient, params.spotScale, diffuse, spec);
    }
    
    return ambient;
}

// toon lighting
vec3 BlinnPhong_LightingToon(
    vec3 pos, 
    vec3 n, 
    vec3 ambientBase, 
    vec3 diffuseBase
) {
    vec3 ambient = calculateAmbient(ambientBase);
    SpotlightParams params = computeSpotlightParams(pos);

    if (params.spotScale > 0.0) {
        float sDotN = max(dot(params.s, n), 0.0);
        float band = floor(sDotN * levels);
        vec3 diffuse = diffuseBase * band * scaleFactor;
        vec3 spec = (sDotN > 0.0) ? calculateSpecular(pos, params.s, n) : vec3(0.0);
        return computeFinalColor(ambient, params.spotScale, diffuse, spec);
    }
    
    return ambient;
}

// applies lighting based on global var
vec3 applyLighting(vec3 pos, vec3 n, vec3 ambientBase, vec3 diffuseBase) 
{
    if (globalSettings.lightingMode == 0) {
        return BlinnPhong_LightingNormal(pos, n, ambientBase, diffuseBase);
    } else {
        return BlinnPhong_LightingToon(pos, n, ambientBase, diffuseBase);
    }
}