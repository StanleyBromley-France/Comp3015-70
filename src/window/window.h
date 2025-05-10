#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>

class Input {
public:
    static void createInstance();

    // removes copy constructor and assignment operator
    Input(const Input&) = delete;
    Input& operator=(const Input&) = delete;



    static bool isKeyPressed(int key);
    static bool isKeyPressedOnce(int key);
    static void updateKeyState();

private:
    Input() = default;

    static Input* instance_;

    static std::vector<bool> previousKeyStates; // tracks previous key states
};