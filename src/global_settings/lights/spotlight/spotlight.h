// spotlight.h
#pragma once

#include "../../global_settings_ubo/global_settings_ubo.h"

// Feeds only the Spotlight part of the global UBO
class spotlight {
public:
    spotlight();
    ~spotlight() = default;

    // update GPU copy of spotlight params
    void upload(GlobalSettingsUBO& globalUbo) const;

    // setters mutate local data + call upload()
    void set_ambient(const glm::vec3& a);
    void set_intensity(const glm::vec3& i);
    void set_exponent(float e);
    void set_cutoff(float radians);
    void set_position(const glm::vec3& pos);
    void set_direction(const glm::vec3& d);

private:
    GlobalSettingsUBO::SpotLightInfo data_;
};
