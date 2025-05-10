#include "window.h"
#include <stdexcept>// for throwing error
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

Input* Input::instance_ = nullptr;
std::vector<bool> Input::previousKeyStates(GLFW_KEY_LAST + 1, false);

void Input::createInstance() {
    if (instance_) {
        throw std::runtime_error("Input instance already created");
    }
    instance_ = new Input();
}


bool Input::isKeyPressed(int key) { 
    if (!instance_) {
        throw std::runtime_error("Window instance not created");
    }
    return glfwGetKey(glfwGetCurrentContext(), key) == GLFW_PRESS;
}

bool Input::isKeyPressedOnce(int key) {
    if (!instance_) {
        throw std::runtime_error("Window instance not created");
    }
    if (key < 0 || key > GLFW_KEY_LAST) return false;

    bool current = glfwGetKey(glfwGetCurrentContext(), key) == GLFW_PRESS;
    bool previous = previousKeyStates[key];
    return current && !previous; // makes sure that it only works if prevoius is false 
}

void Input::updateKeyState()
{
    if (!instance_) {
        throw std::runtime_error("Window instance not created");
    }

    for (int key = 0; key <= GLFW_KEY_LAST; key++) {
        previousKeyStates[key] = glfwGetKey(glfwGetCurrentContext(), key) == GLFW_PRESS; // checks current state for all keys and assings true if pressed
    }
}



