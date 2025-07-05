//
// Created by dingrui on 25-6-23.
//

#include <iostream>
// be sure to include GLAD before other header files that require OpenGL
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "callback.h"

#include <application/Camera.h>

#include "application/Application.h"

void framebuffer_size_callback(int width, int height)
{
    std::cout << "新窗口大小 w:" << width << ", h:" << height << std::endl;
    // 视口 设置窗口中opengl负责渲染的区域
    // x y将相对窗口左下角的起始位置
    // width height渲染区域的长度 高度
    glViewport(0, 0, width, height);
}

void keyboard_callback(int key, int scancode, int action, int mods)
{
    std::cout << "键盘事件 键位" << static_cast<char>(key) << ", 操作" << action << ", 有没有ctrl/shift功能键"<< mods << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) Application::setShouldClose(true);
}

extern Camera camera;
extern bool firstMouse;
extern float lastX;
extern float lastY;
void cursor_position_callback(double x, double y) {
    std::cout << "鼠标位置发生了变化 现在的 x=" << x << ", y=" << y << std::endl;
    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }
    float xoffset = x - lastX;
    float yoffset = lastY - y;
    lastX = x;
    lastY = y;
    camera.ProcessMouseMovement(xoffset, yoffset);
}

void mouse_scroll_callback(double yoffset) {
    if (yoffset > 0) std::cout << "鼠标滚轮放大 yoffset: " << yoffset << std::endl;
    else std::cout << "鼠标滚轮缩小 yoffset: " << yoffset << std::endl;
    camera.ProcessMouseScroll(yoffset);
}
