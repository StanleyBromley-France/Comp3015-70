#pragma once

#include "../../global_settings_ubo/global_settings_ubo.h"

class spotlight {
public:
    spotlight();
    ~spotlight() = default;

    void set_ambient(const glm::vec3& a);
    void set_intensity(const glm::vec3& i);
    void set_exponent(float e);
    void set_cutoff(float radians);
    void set_position(const glm::vec3& pos);
    void set_direction(const glm::vec3& d);

    GlobalSettingsUBO::SpotLightInfo data_;
};
