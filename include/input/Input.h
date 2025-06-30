//
// Created by rui ding on 2025/6/30.
//

#pragma once

#include <unordered_map>

// 前向声明
struct GLFWwindow;

class Input {
public:
    static void init(GLFWwindow* window);
    // 每帧调用
    static void update();
    static bool isKeyPressed(int key);
    static bool isKeyHeld(int key);
    static bool isKeyReleased(int key);
private:
    static GLFWwindow* s_Window;
    static std::unordered_map<int, bool> s_KeyState;     // 当前帧状态
    static std::unordered_map<int, bool> s_KeyStateLast; // 上一帧状态
};