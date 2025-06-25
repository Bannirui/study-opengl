//
// Created by dingrui on 25-6-23.
//

#include <iostream>
// be sure to include GLAD before other header files that require OpenGL
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "callback.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    std::cout << "w:" << width << ", h:" << height << std::endl;
    // tell OpenGL the size of the rendering window
    glViewport(0, 0, width, height);
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
