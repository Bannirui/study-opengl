//
// Created by dingrui on 25-6-23.
//

#include <iostream>

// be sure to include GLAD before other header files that require OpenGL
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>
#include <STB/stb_image.h>

#include "callback.h"
#include "Shader.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    // initialize GLFW OpenGL的版本是3.3
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // tell GLFW explicitly to use the core-profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // create a window object
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    // 设置好上下文后就要初始化glad 后面才能安全使用glad的函数
    glfwMakeContextCurrent(window);
    // 窗口变化回调
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // GLAD manages function pointers for OpenGL, initialize GLAD before calling any OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }
    // 创建shader实例
    Shader ourShader("resources/shader/3.3.shader.vs", "resources/shader/3.3.shader.fs");
    // 三角形的三维坐标 z被压扁 3个顶点
    float vertices[] = {
        // position        // color         // texture坐标
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // 左下
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // 左上
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3,
    };
    // VBO(vertex buffer objects)管理顶点坐标 批量向显卡发送数据
    // OpenGL有许多buffer objects VBO是array buffer
    // VAO(vertex array object)
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind vertex array object first, then buffer
    glBindVertexArray(VAO);
    // OpenGL开放这个函数把创建的buffer绑定到array buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 用户态的数据拷贝到buffer
    // target 接收数据的buffer类型
    // size 要把多少byte数据传递给buffer
    // data 要传递的数据
    // usage 显卡怎么处理这些数据 GL_STREAM_DRAW数据只设置一次GPU有使用次数限制 GL_STATIC_DRAW数据只设置一次GPU可以多次使用 GL_DYNAMIC_DRAW数据会变化GPU可以多次使用
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 把所有坐标点传到了buffer里面 告诉shader怎么用这些数据
    // 位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    unsigned int texture1, texture2;
    // texture1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    int width, height, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("resources/textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // texture2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    data = stbi_load("resources/textures/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);
    // 显式调用shader program
    ourShader.use();
    glUniform1i(glGetUniformLocation(ourShader.m_ID, "texture1"), 0);
    ourShader.setInt("texture2", 1);
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // check for specific key press and react accordingly every frame
        processInput(window);
        // 每一帧都要清屏 防止残留前一帧图像
        glClearColor(0.8f, 1.0f, 0.93f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        ourShader.use();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        // swap the color buffer
        glfwSwapBuffers(window);
        // check if any events are triggered
        glfwPollEvents();
    }
    // 回收资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    // 回收GLFW资源
    glfwTerminate();
    return 0;
}
