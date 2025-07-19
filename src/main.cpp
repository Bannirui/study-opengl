//
// Created by dingrui on 25-6-23.
//

#include <iostream>

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
#include "glframework/Texture.h"

const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;

// 相机坐标
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

bool  firstMouse = true;
float lastX      = SCR_WIDTH / 2.0f;
float lastY      = SCR_HEIGHT / 2.0f;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int vbo = 0;
unsigned int vao = 0;
unsigned int ebo = 0;
// texture1 texture2
Texture* texture1 = nullptr;
Texture* texture2 = nullptr;

// 创建shader实例
Shader* prepareShader()
{
    Shader* shader = new Shader("resources/shader/3.3.vertex.glsl", "resources/shader/3.3.fragment.glsl");
    return shader;
}

// 生成VAO
void prepareVAO()
{
    // 顶点数据 交叉属性 放到一个VBO里面 用VAO告诉GPU属性信息
    // 立方体6个面 每个面1个矩形 1个矩形等于2个三角形 1个三角形3个顶点 那就总共需要36个顶点信息
    // clang-format off
    float vertices[] = {
        // position XYZ       UV坐标
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };
    unsigned int indices[] = {
        0, 1, 2,
        3, 4, 5,
        6, 7, 8,
        9, 10, 11,
        12, 13, 14,
        15, 16, 17,
        18, 19, 20,
        21, 22, 23,
        24, 25, 26,
        27, 28, 29,
        30, 31, 32,
        33, 34, 35,
    };
    // clang-format on
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
    // 坐标
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    // UV坐标
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

    // VAO中加入EBO的索引信息
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

    // 解绑OpenGL状态机的VAO插槽 最好不要一直保持着某个VAO的绑定状态
    glBindVertexArray(0);
}

void prepareTexture()
{
    texture1 = new Texture("resources/texture/container.jpg", 0);
    texture2 = new Texture("resources/texture/awesomeface.png", 1);
}

void render(Shader* shader)
{
    auto curFrameTime = static_cast<float>(glfwGetTime());
    deltaTime         = curFrameTime - lastFrame;
    lastFrame         = curFrameTime;

    // 每一帧都要清屏 防止残留前一帧图像
    GL_CALL_AND_CHECK_ERR(glClear(GL_COLOR_BUFFER_BIT));
    // 删除depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture1->Bind();
    texture2->Bind();
    // 告诉GPU接下来绘制用的shader程序是哪个
    shader->use();
    // 告诉GPU绘制图形用的VAO
    glBindVertexArray(vao);
    // 视图矩阵 世界空间->摄影机空间
    glm::mat4 view = camera.GetViewMatrix();
    shader->setMat4("u_view", glm::value_ptr(view));
    // 投影矩阵 摄影机空间->剪裁空间
    glm::mat4 projection =
        glm::perspective(glm::radians(camera.Zoom), (float)app->getWidth() / (float)app->getHeight(), 0.1f, 100.0f);
    shader->setMat4("u_projection", glm::value_ptr(projection));
    // 开辟uniform全局变量给vertex shader
    // 多个立方体的位置
    // clang-format off
    // 平移变换 xyz3个轴的平移 用来通过translate生成平移矩阵
    glm::vec3 positions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
    };
    // clang-format on
    // 采样器sampler1采样0号纹理单元
    shader->setInt("u_sampler1", texture1->GetUnit());
    // 采样器sampler2采样1号纹理单元
    shader->setInt("u_sampler2", texture2->GetUnit());
    // 每一帧拿到系统时间告诉shader 达到呼吸效果
    shader->setFloat("u_systime", static_cast<float>(glfwGetTime()));
    // 控制运动速率
    shader->setFloat("u_movSpeed", 4.0f);
    for (unsigned int i = 0, sz = sizeof(positions); i < sz / sizeof(positions[0]); i++)
    {
        // 模型矩阵 aPos模型->世界空间
        // 初始化单位矩阵
        auto model  = glm::mat4(1.0f);
        // translate平移变换
        model       = glm::translate(model, positions[i]);
        float angle = 20.0f * i;
        // rotate生成旋转矩阵
        // angle接收的参数是弧度 不是角度 glm::radians函数将角度转化为弧度
        model       = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
        shader->setMat4("u_model", glm::value_ptr(model));
        // 向GPU发送绘制指令
        GL_CALL_AND_CHECK_ERR(glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0));
    }
    // 绘制完一帧解绑VAO防止状态误用
    glBindVertexArray(0);
    shader->end();
}

void processInput()
{
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
    prepareVAO();
    prepareTexture();
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
    delete texture1;
    delete texture2;
    delete shader;

    app->destroy();
    return 0;
}