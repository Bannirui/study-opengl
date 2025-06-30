//
// Created by dingrui on 25-6-23.
//

#include <iostream>
// be sure to include GLAD before other header files that require OpenGL
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "callback.h"

#include <application/Application.h>

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
    std::cout << "key:" << key << ", scancode:" << scancode << ", action: " << action << ", mods:" << mods << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) Application::setShouldClose(true);
}

void processInput(GLFWwindow* window)
{
    // esc退出
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    // ctrl-c退出
    if ((glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS
        || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
        && glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}
