//
// Created by dingrui on 25-6-23.
//

#include "callback.h"

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    std::cout << "w:" << width << ", h:" << height << std::endl;
    glViewport(0, 0, width, height);
}
