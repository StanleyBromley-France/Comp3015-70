#pragma once
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class ShowcaseCam {
public:
    ShowcaseCam(GLFWwindow* window);

    // Callback functions
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void cursorCallback(GLFWwindow* window, double xpos, double ypos);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

    // Get the current view matrix
    glm::mat4 getViewMatrix() const;

private:
    void updateCameraVectors();

    // Camera parameters
    glm::vec3 cameraPosition;
    glm::vec3 center;
    glm::vec3 up;

    // Control variables
    float radius;
    float theta;
    float phi;
    double lastX;
    double lastY;
    bool isDragging;
    float minY;
    float maxY;
};
