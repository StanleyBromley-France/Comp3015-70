#pragma once
#include <glm/glm.hpp>

class GameCam {
public:
    GameCam();

    void Update(float currentTime, const glm::vec3& carPosition, float carYawRadians);

    const glm::mat4& GetViewMatrix() const;

private:
    glm::vec3 position;
    glm::mat4 viewMatrix;

    float lastFrameTime_ = -1.0f;

    const glm::vec3 CAM_OFFSET = glm::vec3(0.0f, 15.f, -18.f);
    const float SMOOTHNESS = 4.0f;
};