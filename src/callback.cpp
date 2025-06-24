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
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}
