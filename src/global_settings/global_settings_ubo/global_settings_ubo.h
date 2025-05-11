#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <array>

// Wrapper for the GlobalSettings UBO (std140 layout, binding = 0)
class GlobalSettingsUBO {
public:
    // must match 'SpotLightInfo' struct in GLSL
    struct SpotLightInfo {
        glm::vec3 Position;   float pad0;
        glm::vec3 La;         float pad1;
        glm::vec3 L;          float pad2;
        glm::vec3 Direction;  float pad3;
        float     Exponent;
        float     Cutoff;
        float     pad4[2];
    };

    static constexpr int MAX_SPOTS = 8;

    // must match 'GlobalSettings' block in GLSL
    struct Data {
        std::array<SpotLightInfo, MAX_SPOTS> Spotlights;
        int numSpotlights;
        int lightingMode;
        int pad[2];
    } data;

    GlobalSettingsUBO();
    ~GlobalSettingsUBO();

    void addSpotlight(const SpotLightInfo& spot);
    void clearSpotlights();
    // set lighting mode: 0 = Normal, 1 = Toon
    void setLightingMode(int mode);
    // must be called once per-frame to upload if dirty
    void updateGPU();

private:
    GLuint ubo_;
    bool   dirty_;
};
