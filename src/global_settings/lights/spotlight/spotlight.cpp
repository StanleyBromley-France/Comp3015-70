#include "spotlight.h"

spotlight::spotlight()
{
    // defaults
    data_.La = glm::vec3(0.2f);
    data_.L = glm::vec3(0.6f);
    data_.Exponent = 20.0f;
    data_.Cutoff = glm::radians(10.0f);
    data_.Position = glm::vec3(0.0f);
    data_.Direction = glm::vec3(0.0f, -1.0f, 0.0f);

}

void spotlight::upload(GlobalSettingsUBO& globalUbo) const {
    globalUbo.setSpotlight(data_);
}

void spotlight::set_ambient(const glm::vec3& a) {
    data_.La = a;
}

void spotlight::set_intensity(const glm::vec3& i) {
    data_.L = i;
}

void spotlight::set_exponent(float e) {
    data_.Exponent = e;
}

void spotlight::set_cutoff(float radians) {
    data_.Cutoff = radians;
}

void spotlight::set_position(const glm::vec3& pos) {
    data_.Position = pos;
}

void spotlight::set_direction(const glm::vec3& d) {
    data_.Direction = d;
}
