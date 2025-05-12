#include "game_cam.h"
#include <glm/gtc/matrix_transform.hpp>

GameCam::GameCam() : position(glm::vec3(0.0f)) {}

void GameCam::Update(float t, const glm::vec3& carPosition, float carYawRadians) {

    float dt = t - lastFrameTime_;
    lastFrameTime_ = t;

    // calculates camera position
    const glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), carYawRadians, glm::vec3(0.0f, 1.0f, 0.0f));
    const glm::vec3 desiredPosition = carPosition + glm::vec3(rotation * glm::vec4(CAM_OFFSET, 1.0f));

    // smooths movement
    position = glm::mix(position, desiredPosition, glm::clamp(SMOOTHNESS * dt, 0.0f, 1.0f));

    const glm::vec3 forwardDir = glm::vec3( sin(carYawRadians), 0.0f, cos(carYawRadians));

    viewMatrix = glm::lookAt(position, position + forwardDir, glm::vec3(0.0f, 1.0f, 0.0f));
}

const glm::mat4& GameCam::GetViewMatrix() const { return viewMatrix; }
