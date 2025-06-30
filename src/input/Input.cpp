//
// Created by rui ding on 2025/6/30.
//

#include "input/Input.h"

#include <GLFW/glfw3.h>

GLFWwindow* Input::s_Window = nullptr;
std::unordered_map<int, bool> Input::s_KeyState;
std::unordered_map<int, bool> Input::s_KeyStateLast;

void Input::init(GLFWwindow* window) {
    s_Window = window;
}

void Input::update() {
    s_KeyStateLast = s_KeyState;
    for (int key = 0; key <= GLFW_KEY_LAST; ++key) {
        s_KeyState[key] = glfwGetKey(s_Window, key) == GLFW_PRESS;
    }
}

bool Input::isKeyPressed(int key) {
    return s_KeyState[key] && !s_KeyStateLast[key];
}

bool Input::isKeyHeld(int key) {
    return s_KeyState[key];
}

bool Input::isKeyReleased(int key) {
    return !s_KeyState[key] && s_KeyStateLast[key];
}