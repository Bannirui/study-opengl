//
// Created by dingrui on 25-6-23.
//

#include <iostream>

// glad是加载显卡驱动对opengl函数的实现的 所以一定要在glfw之前引用进来
#include <GLAD/glad.h>
// glfw实现了窗体 在窗体创建好后就用glad加载显卡驱动函数
#include <GLFW/glfw3.h>
#include <STB/stb_image.h>

#include "callback.h"
#include "Shader.h"
#include "err_check.h"
#include "application/Application.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 生成VAO
void prepareBuffer()
{
    // 顶点数据 交叉属性 放到一个VBO里面 用VAO告诉GPU属性信息
    float vertices[] = {
        // position xyz   // color rgb      // texture坐标
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // 左上
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3,
    };
    // VBO EBO
    unsigned int vbo_arr[2] = {0, 0};
    // 创建VBO
    glGenBuffers(2, vbo_arr);
    // 绑定VBO到OpenGL当前VBO的插槽上 后面向OpenGL当前VBO插槽的操作就是间接在操作VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo_arr[0]);
    // 向OpenGL状态机当前VBO插槽填装数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_arr[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 创建VAO绑定到OpenGL的VAO插槽上 此时OpenGL状态机VBO插槽上关联的是vbo_arr[0]这个VBO
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    // 向VAO中添加VBO的属性
    // 激活VAO脚标
    glEnableVertexAttribArray(0);
    // 位置属性放在VAO数组的0号脚标上
    // 位置属性用3个数字
    // 每个数字是float类型
    // 顶点的步长是8个float
    // 位置在顶点内部的偏移是0
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    // 颜色属性放在VAO数组的1号脚标上
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    // texture属性放在VAO数组的2号脚标上
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    // 解绑OpenGL状态机的VAO插槽 最好不要一直保持着某个VAO的绑定状态
    glBindVertexArray(0);
}

int main()
{
    if (!app->init(SCR_WIDTH, SCR_HEIGHT)) return -1;
    // 窗口变化回调
    app->setResizeCallback(framebuffer_size_callback);
    // 键盘回调
    app->setKeyboardCallback(keyboard_callback);
    prepareBuffer();
    // 创建shader实例
    Shader ourShader("resources/shader/3.3.shader.vsh", "resources/shader/3.3.shader.fsh");
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
    // 清理画布的时候清成啥样
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // 窗体循环
    while (app->update())
    {
        // check for specific key press and react accordingly every frame
        // processInput(window);
        // 每一帧都要清屏 防止残留前一帧图像
        GL_CALL_AND_CHECK_ERR(glClear(GL_COLOR_BUFFER_BIT));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        ourShader.use();
        // glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }
    // 回收资源
    // glDeleteVertexArrays(1, &VAO);
    // 销毁VBO
    // glDeleteBuffers(2, vbo_arr);
    app->destroy();
    return 0;
}
