//
// Created by dingrui on 25-6-23.
//

#include <iostream>

// glad是加载显卡驱动对opengl函数的实现的 所以一定要在glfw之前引用进来
#include <GLAD/glad.h>
// glfw实现了窗体 在窗体创建好后就用glad加载显卡驱动函数
#include <GLFW/glfw3.h>
#include <STB/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "callback.h"
#include "Shader.h"
#include "err_check.h"
#include "application/Application.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// VBO EBO
unsigned int vbo_arr[2] = {0, 0};
unsigned int vao = 0;
// texture1 texture2
unsigned int texture_arr[] = {0, 0};
// 生成VAO
void prepareVAO(Shader shader)
{
    // 顶点数据 交叉属性 放到一个VBO里面 用VAO告诉GPU属性信息
    float vertices[] = {
        // position XYZ   // color RGB      // texture坐标ST
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // 右上
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // 左上
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 左下
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // 右下
    };
    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3,
    };
    // 创建VBO EBO
    glGenBuffers(2, vbo_arr);
    // 绑定VBO到OpenGL当前VBO的插槽上 后面向OpenGL当前VBO插槽的操作就是间接在操作VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo_arr[0]);
    // 向OpenGL状态机当前VBO插槽填装数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 绑定OpenGL状态机EBO插槽
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_arr[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // 创建VAO绑定到OpenGL的VAO插槽上 此时OpenGL状态机VBO插槽上关联的是vbo_arr[0]这个VBO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
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

    glGenTextures(2, texture_arr);
    // 更新OpenGL插槽
    glBindTexture(GL_TEXTURE_2D, texture_arr[0]);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char* data = stbi_load("resources/textures/container.jpg", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
        assert(false);
    }
    stbi_image_free(data);
    // 更新OpenGL插槽
    glBindTexture(GL_TEXTURE_2D, texture_arr[1]);
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
        assert(false);
    }
    stbi_image_free(data);
    // 在帧循环之前告诉OpenGL哪个shader程序用哪个全局变量 只调用一次
    shader.use();
    shader.setInt("texture1", 0);
    shader.setInt("texture2", 1);
}

void render(Shader shader)
{
        // 每一帧都要清屏 防止残留前一帧图像
        GL_CALL_AND_CHECK_ERR(glClear(GL_COLOR_BUFFER_BIT));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture_arr[0]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture_arr[1]);

        // 告诉GPU接下来绘制用的shader程序是哪个
        shader.use();
        // 告诉GPU绘制图形用的VAO
        glBindVertexArray(vao);
        // 绘制时候用到EBO 绑定到OpenGL插槽
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbo_arr[1]);
        // 转换坐标
        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
        projection = glm::perspective(glm::radians(45.0f), (float)app->getWidth()/app->getHeight(), 0.1f, 100.0f);
        // 开辟uniform全局变量给vertex shader
        unsigned int modelLoc = glGetUniformLocation(shader.m_ID, "model");
        unsigned int viewLoc = glGetUniformLocation(shader.m_ID, "view");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, &view[0][0]);
        shader.setMat4("projection", projection);
        // 向GPU发送绘制指令
        GL_CALL_AND_CHECK_ERR(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0));
}

int main()
{
    if (!app->init(SCR_WIDTH, SCR_HEIGHT)) return -1;
    // 窗口变化回调
    app->setResizeCallback(framebuffer_size_callback);
    // 键盘回调
    app->setKeyboardCallback(keyboard_callback);
    // 创建shader实例
    Shader ourShader("resources/shader/3.3.shader.vsh", "resources/shader/3.3.shader.fsh");
    prepareVAO(ourShader);
    // 清理画布的时候清成啥样
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // 窗体循环
    while (app->update())
    {
        render(ourShader);
    }
    // 回收资源
    glDeleteVertexArrays(1, &vao);
    // 销毁VBO
    glDeleteBuffers(2, vbo_arr);
    app->destroy();
    return 0;
}
