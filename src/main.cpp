//
// Created by dingrui on 25-6-23.
//

#include <iostream>

#include <STB/stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "glframework/Core.h"
#include "callback.h"
#include "err_check.h"
#include "application/Application.h"
#include "application/Camera.h"
#include "glframework/Shader.h"
#include "input/Input.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// 相机坐标
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool firstMouse = true;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int vbo = 0;
unsigned int vao = 0;
unsigned int ebo = 0;
// texture1 texture2
unsigned int texture_arr[] = {0, 0};

// 创建shader实例
Shader* prepareShader()
{
    Shader* shader = new Shader("resources/shader/3.3.vertex.glsl", "resources/shader/3.3.fragment.glsl");
    return shader;
}

// 生成VAO
void prepareVAO(Shader* shader)
{
    // 顶点数据 交叉属性 放到一个VBO里面 用VAO告诉GPU属性信息
    // 立方体6个面 每个面1个矩形 1个矩形等于2个三角形 1个三角形3个顶点 那就总共需要36个顶点信息
    float vertices[] = {
        // position XYZ       color RGB           texture坐标ST
         0.5f, -0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        -0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
        -0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0,
        4, 5, 6,
        6, 7, 4,
        0, 1, 5,
        5, 4, 0,
        3, 2, 6,
        6, 7, 3,
        1, 2, 6,
        6, 5, 1,
        0, 3, 7,
        7, 4, 0,
    };
    // 创建VBO
    glGenBuffers(1, &vbo);
    // 绑定VBO到OpenGL当前VBO的插槽上 后面向OpenGL当前VBO插槽的操作就是间接在操作VBO
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // 向OpenGL状态机当前VBO插槽填装数据
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 创建EBO
    glGenBuffers(1, &ebo);
    // GL状态机
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    // 灌数据
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // 创建VAO绑定到OpenGL的VAO插槽上 此时OpenGL状态机VBO插槽上关联的是vbo_arr[0]这个VBO
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // VAO中加入VBO的属性信息
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));

    // VAO中加入EBO的索引信息
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

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
    unsigned char* data = stbi_load("resources/texture/container.jpg", &width, &height, &nrChannels, 0);
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
    data = stbi_load("resources/texture/awesomeface.png", &width, &height, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cout << "Failed to load texture" << std::endl;
        assert(false);
    }
    stbi_image_free(data);
    // 在帧循环之前告诉OpenGL哪个shader程序用哪个全局变量 只调用一次
    shader->use();
    shader->setInt("texture1", 0);
    shader->setInt("texture2", 1);
    shader->end();
}


void render(Shader* shader)
{
    float curFrameTime = static_cast<float>(glfwGetTime());
    deltaTime = curFrameTime - lastFrame;
    lastFrame = curFrameTime;

    // 每一帧都要清屏 防止残留前一帧图像
    GL_CALL_AND_CHECK_ERR(glClear(GL_COLOR_BUFFER_BIT));
    // 删除depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture_arr[0]);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture_arr[1]);

    // 告诉GPU接下来绘制用的shader程序是哪个
    shader->use();
    // 告诉GPU绘制图形用的VAO
    glBindVertexArray(vao);
    // 视图矩阵 世界空间->摄影机空间
    glm::mat4 view = camera.GetViewMatrix();
    shader->setMat4("view", glm::value_ptr(view));
    // 投影矩阵 摄影机空间->剪裁空间
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)app->getWidth() / (float)app->getHeight(), 0.1f, 100.0f);
    shader->setMat4("projection", glm::value_ptr(projection));
    // 开辟uniform全局变量给vertex shader
    // 多个立方体的位置
    glm::vec3 positions[] = {
        glm::vec3( 0.0f,  0.0f,  0.0f),
        glm::vec3( 2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f,  2.0f, -2.5f),
        glm::vec3( 1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };
    for (unsigned int i = 0, sz=sizeof(positions); i < sz / sizeof(positions[0]); i++) {
        // 模型矩阵 aPos模型->世界空间
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, positions[i]);
        float angle = 20.0f * i;
        model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        shader->setMat4("model", glm::value_ptr(model));
        // 向GPU发送绘制指令
        GL_CALL_AND_CHECK_ERR(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0));
    }
    // 绘制完一帧解绑VAO防止状态误用
    glBindVertexArray(0);
    // 每一帧拿到系统时间告诉shader 达到呼吸效果
    shader->setFloat("systime", glfwGetTime());
    // 控制运动速率
    shader->setFloat("movSpeed", 4.0f);
    shader->end();
}

void processInput() {
    if (Input::isKeyPressed(GLFW_KEY_W)) camera.ProcessKeyboard(FORWARD, deltaTime);
    if (Input::isKeyPressed(GLFW_KEY_S)) camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (Input::isKeyPressed(GLFW_KEY_A)) camera.ProcessKeyboard(LEFT, deltaTime);
    if (Input::isKeyPressed(GLFW_KEY_D)) camera.ProcessKeyboard(RIGHT, deltaTime);
}

int main()
{
    if (!app->init(SCR_WIDTH, SCR_HEIGHT)) return -1;
    // 监听事件
    app->setResizeCallback(framebuffer_size_callback);
    app->setKeyboardCallback(keyboard_callback);
    app->setCursorPosCallback(cursor_position_callback);
    app->setScrollCallback(mouse_scroll_callback);

    // 开启deep testing
    glEnable(GL_DEPTH_TEST);
    Shader* shader = prepareShader();
    prepareVAO(shader);
    // 清理画布的时候清成啥样
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    // 窗体循环
    while (app->update())
    {
        // 键盘状态
        processInput();
        render(shader);
    }
    // 回收资源
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteBuffers(1, &ebo);
    app->destroy();
    delete shader;
    return 0;
}