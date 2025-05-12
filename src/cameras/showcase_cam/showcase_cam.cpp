#include "showcase_cam.h"
#include <glm/ext/matrix_transform.hpp>
#include "../../ui/imgui_wrapper/imgui/imgui_impl_glfw.h"
#include "../../ui/imgui_wrapper/imgui_core.h"

ShowcaseCam::ShowcaseCam(GLFWwindow* window)
    : cameraPosition(0.0f),
    center(0.0f, 3.75f, 0.0f),
    up(0.0f, 1.0f, 0.0f),
    radius(30.0f),
    theta(glm::radians(25.0f)),
    phi(glm::radians(65.0f)),
    lastX(0.0),
    lastY(0.0),
    isDragging(false),
    minY(0.5f),
    maxY(15.0f)
{
    // stores pointer for static-to-instance bridge
    glfwSetWindowUserPointer(window, this);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetCursorPosCallback(window, cursorCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    updateCameraVectors();
}

glm::mat4 ShowcaseCam::getViewMatrix() const {
    return glm::lookAt(cameraPosition, center, up);
}

void ShowcaseCam::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    ImGui_ImplGlfw_ScrollCallback(window, xoffset, yoffset);
    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse)
        return;
    
    auto* self = static_cast<ShowcaseCam*>(glfwGetWindowUserPointer(window));


    self->radius -= static_cast<float>(yoffset) * 0.5f;
    self->radius = glm::clamp(self->radius, 20.0f, 30.0f);
    self->updateCameraVectors();
}

void ShowcaseCam::cursorCallback(GLFWwindow* window, double xpos, double ypos) {
    ImGui_ImplGlfw_CursorPosCallback(window, xpos, ypos);

    ImGuiIO& io = ImGui::GetIO();
    if (io.WantCaptureMouse) return;
    auto* self = static_cast<ShowcaseCam*>(glfwGetWindowUserPointer(window));

    if (self->isDragging) {
        float dx = static_cast<float>(xpos - self->lastX) * 0.005f;
        float dy = static_cast<float>(ypos - self->lastY) * 0.005f;

        self->theta -= dx;
        self->phi += dy;
        self->phi = glm::clamp(self->phi, 0.1f, glm::pi<float>() - 0.1f);
        self->updateCameraVectors();
    }
    self->lastX = xpos;
    self->lastY = ypos;
}

void ShowcaseCam::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {

    ImGui_ImplGlfw_MouseButtonCallback(window, button, action, mods);

    if (ImGui::GetIO().WantCaptureMouse) return;

    auto* self = static_cast<ShowcaseCam*>(glfwGetWindowUserPointer(window));

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        if (action == GLFW_PRESS) {
            self->isDragging = true;
            glfwGetCursorPos(window, &self->lastX, &self->lastY);
            // hides the cursor
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        }
        else if (action == GLFW_RELEASE) {
            self->isDragging = false;
            // shows the cursor
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
    }
}

void ShowcaseCam::updateCameraVectors() {
    cameraPosition.x = center.x + radius * sin(phi) * cos(theta);
    cameraPosition.y = center.y + radius * cos(phi);
    cameraPosition.z = center.z + radius * sin(phi) * sin(theta);

    if (cameraPosition.y < minY || cameraPosition.y > maxY) {
        float targetY = glm::clamp(cameraPosition.y, minY, maxY);
        float targetCosPhi = (targetY - center.y) / radius;
        targetCosPhi = glm::clamp(targetCosPhi, -1.0f, 1.0f);
        phi = glm::acos(targetCosPhi);

        cameraPosition.x = center.x + radius * sin(phi) * cos(theta);
        cameraPosition.y = center.y + radius * cos(phi);
        cameraPosition.z = center.z + radius * sin(phi) * sin(theta);
    }
}
