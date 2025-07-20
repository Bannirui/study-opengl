//
// Created by dingrui on 25-6-23.
//

#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "err_check.h"
#include "application/Application.h"
#include "application/camera/Camera.h"
#include "application/camera/CameraController.h"
#include "application/camera/GameCameraController.h"
#include "application/camera/PerspectiveCamera.h"
#include "glframework/Geometry.h"
#include "glframework/Shader.h"
#include "glframework/Texture.h"

const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;

Geometry* geometry = nullptr;
// texture1 texture2
Texture* texture1 = nullptr;
Texture* texture2 = nullptr;

Shader*               shader    = nullptr;
Camera*               camera    = nullptr;
GameCameraController* cameraCtl = nullptr;

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
    std::cout << "键盘事件 键位" << static_cast<char>(key) << ", 操作" << action << ", 有没有ctrl/shift功能键" << mods
              << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) Application::setShouldClose(true);
    cameraCtl->OnKey(key, scancode, action);
}

void cursor_position_callback(double x, double y)
{
    std::cout << "鼠标位置发生了变化 现在的 x=" << x << ", y=" << y << std::endl;
    cameraCtl->OnCursor(x, y);
}

void mouse_scroll_callback(double yoffset)
{
    if (yoffset > 0)
        std::cout << "鼠标滚轮放大 yoffset: " << yoffset << std::endl;
    else
        std::cout << "鼠标滚轮缩小 yoffset: " << yoffset << std::endl;
    cameraCtl->OnScroll(yoffset);
}
void mouse_btn_callback(int button, int action, int mods)
{
    std::cout << "button=" << button << ", action=" << action << ", mods=" << mods << std::endl;
    double x, y;
    app->GetMousePos(&x, &y);
    cameraCtl->OnMouse(button, action, mods, x, y);
}
void prepareCamera()
{
    camera    = new PerspectiveCamera(static_cast<float>(app->getWidth()) / static_cast<float>(app->getHeight()));
    camera->m_Position = glm::vec3(2.0f, -4.0f, 10.0f);
    cameraCtl = new GameCameraController(camera);
    cameraCtl->SetSensitivity(0.4f);
    cameraCtl->SetSpeed(0.1f);
}

// 创建shader实例
void prepareShader()
{
    shader = new Shader("resources/shader/3.3.vertex.glsl", "resources/shader/3.3.fragment.glsl");
}

void prepareTexture()
{
    texture1 = new Texture("resources/texture/container.jpg", 0);
    texture2 = new Texture("resources/texture/awesomeface.png", 1);
}

void prepareVAO()
{
    geometry = Geometry::CreateBox(3.0f);
}

void render()
{
    // 每一帧都要清屏 防止残留前一帧图像 清理的时候也要清理深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 告诉GPU接下来绘制用的shader程序是哪个 綁定shader 更新uniform
    shader->use();
    // 视图矩阵 世界空间->摄影机空间
    auto view = camera->GetViewMatrix();
    shader->setMat4("u_view", glm::value_ptr(view));
    auto projection = camera->GetProjectionMatrix();
    shader->setMat4("u_projection", glm::value_ptr(projection));
    // 采样器sampler1采样0号纹理单元
    texture1->Bind();
    shader->setInt("u_sampler1", texture1->GetUnit());
    // 采样器sampler2采样1号纹理单元
    texture2->Bind();
    shader->setInt("u_sampler2", texture2->GetUnit());
    // 模型矩阵 aPos模型->世界空间
    // 初始化单位矩阵
    auto model = glm::mat4(1.0f);
    shader->setMat4("u_model", glm::value_ptr(model));
    // 绑定当前VAO
    glBindVertexArray(geometry->GetVAO());
    // 向GPU发送绘制指令
    GL_CALL_AND_CHECK_ERR(glDrawElements(GL_TRIANGLES, geometry->GetIndicesCnt(), GL_UNSIGNED_INT, 0));
    // 绘制完一帧解绑VAO防止状态误用
    glBindVertexArray(0);
    shader->end();
}

void prepareState()
{
    // 开启deep testing 不开启深度缓存的话 后绘制的会覆盖先绘制的
    glEnable(GL_DEPTH_TEST);
    // 设置深度测试方法
    glDepthFunc(GL_LESS);
}

int main()
{
    if (!app->init(SCR_WIDTH, SCR_HEIGHT)) return -1;

    // 监听事件
    app->setResizeCallback(framebuffer_size_callback);
    app->setKeyboardCallback(keyboard_callback);
    app->setCursorPosCallback(cursor_position_callback);
    app->setScrollCallback(mouse_scroll_callback);
    app->setMouseBtnCallback(mouse_btn_callback);

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
    // 清理画布的时候清成啥样
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

    prepareShader();
    prepareVAO();
    prepareTexture();
    prepareCamera();
    prepareState();

    // 窗体循环
    while (app->update())
    {
        cameraCtl->OnUpdate();
        render();
    }
    // 回收资源
    app->destroy();
    return 0;
}