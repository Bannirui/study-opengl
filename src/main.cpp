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
#include "application/camera/OrthographicCamera.h"
#include "application/camera/PerspectiveCamera.h"
#include "application/camera/TrackballCameraController.h"
#include "glframework/geo/Geometry.h"
#include "glframework/geo/Box.h"
#include "glframework/Shader.h"
#include "glframework/Texture.h"
#include "glframework/geo/Plane.h"
#include "glframework/geo/Sphere.h"

const unsigned int SCR_WIDTH  = 800;
const unsigned int SCR_HEIGHT = 600;

// 模型矩阵初始化为单位矩阵 缩放平移旋转作用在它身上
glm::mat4 model = glm::mat4(1.0f);
// 法线矩阵 在shader中计算法线矩阵的话对于GPU而言性能不友好 所以由CPU计算好直接传过去
glm::mat3 normalMatrix = glm::mat4(1.0f);
// 平行光
// 光照向的方向
glm::vec3 lightDirection = glm::vec3(-1.0f, 0.0f, -1.0f);
// 光强
glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
// 高光反射强度
constexpr float c_specularIntensity = 0.4f;
// 环境光
glm::vec3 ambientColor = glm::vec3(0.2f, 0.2f, 0.2f);

Geometry* geometry = nullptr;
Texture*  texture  = nullptr;

Shader*           shader    = nullptr;
Camera*           camera    = nullptr;
CameraController* cameraCtl = nullptr;

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
    cameraCtl->OnKey(key, action, mods);
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
    double x, y;
    app->GetMousePos(&x, &y);
    std::cout << "button=" << button << ", action=" << action << ", mods=" << mods << ", x=" << x << ", y=" << y
              << std::endl;
    cameraCtl->OnMouse(button, action, mods, x, y);
}
void prepareCamera()
{
    camera = new PerspectiveCamera(static_cast<float>(app->getWidth()) / static_cast<float>(app->getHeight()));
    camera->m_Position = glm::vec3(0.0f, 0.0f, 5.0f);
    cameraCtl          = new TrackballCameraController(camera);
}

// 创建shader实例
void prepareShader()
{
    shader = new Shader("resources/shader/my_shader.glsl");
}

void prepareTexture()
{
    texture = new Texture("resources/texture/2k_earth_daymap.jpg", 0);
}

void prepareVAO()
{
    geometry = new Sphere;
}

void render()
{
    // 每一帧都要清屏 防止残留前一帧图像 清理的时候也要清理深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 告诉GPU接下来绘制用的shader程序是哪个 綁定shader 更新uniform
    shader->Bind();
    // 模型矩阵 aPos模型->世界空间
    shader->setMat4("u_model", glm::value_ptr(model));
    // 模型矩阵
    shader->setMat3("u_normalMatrix", glm::value_ptr(normalMatrix));
    // 视图矩阵 世界空间->摄影机空间
    auto view = camera->GetViewMatrix();
    shader->setMat4("u_view", glm::value_ptr(view));
    auto projection = camera->GetProjectionMatrix();
    shader->setMat4("u_projection", glm::value_ptr(projection));
    // 采样器sampler1采样0号纹理单元
    texture->Bind();
    shader->setInt("u_sampler", texture->GetUnit());
    // 光源参数
    shader->setFloatVec3("u_lightDirection", lightDirection);
    shader->setFloatVec3("u_lightColor", lightColor);
    // 相机位置
    shader->setFloatVec3("u_cameraPos", camera->m_Position);
    // 高光反射强度
    shader->setFloat("u_specularIntensity", c_specularIntensity);
    // 环境光
    shader->setFloatVec3("u_ambientColor", ambientColor);
    // 绑定当前VAO
    glBindVertexArray(geometry->GetVAO());
    // 向GPU发送绘制指令
    GL_CALL_AND_CHECK_ERR(glDrawElements(GL_TRIANGLES, geometry->GetIndicesCnt(), GL_UNSIGNED_INT, 0));
    // 绘制完一帧解绑VAO防止状态误用
    glBindVertexArray(0);
    shader->Unbind();
}

void prepareState()
{
    // 开启deep testing 不开启深度缓存的话 后绘制的会覆盖先绘制的
    glEnable(GL_DEPTH_TEST);
    // 设置深度测试方法
    glDepthFunc(GL_LESS);
}
void doTransform()
{
    // model matrix
    model = glm::rotate(model, 0.01f, glm::vec3(-0.1f, 1.0f, 0.0f));
    // normal matrix
    normalMatrix = glm::transpose(glm::inverse(glm::mat3(model)));
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
        doTransform();
        cameraCtl->OnUpdate();
        render();
    }
    // 回收资源
    app->destroy();
    return 0;
}