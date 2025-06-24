//
// Created by dingrui on 25-6-23.
//

#include <iostream>

// be sure to include GLAD before other header files that require OpenGL
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "callback.h"

int main()
{
    // initialize GLFW
    glfwInit();
    // opengl ver 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // tell GLFW explicitly to use the core-profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    #ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    #endif
    // create a window object
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", nullptr, nullptr);
    if (!window)
    {
        std::cout << "Failed to create GLFW window\n";
        return -1;
    }
    glfwMakeContextCurrent(window);
    // GLAD manages function pointers for OpenGL, initialize GLAD before calling any OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }
    // tell OpenGL the size of the rendering window
    glViewport(0, 0, 800, 600);
    // register callback, call this func on every window resize
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // check for specific key press and react accordingly every frame
        processInput(window);
        // swap the color buffer
        glfwSwapBuffers(window);
        // check if any events are triggered
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}
