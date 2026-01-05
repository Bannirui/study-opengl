//
// Created by dingrui on 25-6-23.
//

#include <iostream>
#include <memory>

#include <glm/glm.hpp>

#include "err_check.h"
#include "application/Application.h"
#include "application/camera/Camera.h"
#include "application/camera/CameraController.h"
#include "application/camera/PerspectiveCamera.h"
#include "application/camera/TrackballCameraController.h"
#include "glframework/Mesh.h"
#include "glframework/Texture.h"
#include "glframework/geo/Sphere.h"
#include "glframework/light/AmbientLight.h"
#include "glframework/light/DirectionalLight.h"
#include "glframework/light/PointLight.h"
#include "glframework/light/SpotLight.h"
#include "glframework/material/PhongMaterial.h"
#include "glframework/material/WhiteMaterial.h"
#include "glframework/renderer/Renderer.h"

const unsigned int SCR_WIDTH  = 1600;
const unsigned int SCR_HEIGHT = 800;

Renderer* renderer = nullptr;
// 渲染列表
std::vector<Mesh*> meshes{};
DirectionalLight*  directionalLight = nullptr;
SpotLight*         spot_light       = nullptr;
AmbientLight*      ambient_light    = nullptr;
PointLight*        point_light      = nullptr;

Camera*           camera    = nullptr;
CameraController* cameraCtl = nullptr;

void framebuffer_size_callback(int width, int height)
{
    std::cout << "新窗口大小 w:" << width << ", h:" << height << std::endl;
    // 视口 设置窗口中opengl负责渲染的区域
    // x y将相对窗口左下角的起始位置
    // width height渲染区域的长度 高度
    GL_CALL_AND_CHECK_ERR(glViewport(0, 0, width, height));
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
    glApp->GetMousePos(&x, &y);
    std::cout << "button=" << button << ", action=" << action << ", mods=" << mods << ", x=" << x << ", y=" << y
              << std::endl;
    cameraCtl->OnMouse(button, action, mods, x, y);
}

void prepare()
{
    renderer = new Renderer();
    // ball
    auto geometryBall       = new Sphere();
    auto materialBall       = new PhongMaterial();
    materialBall->m_shiness = 10.0f;
    materialBall->m_diffuse = new Texture("asset/texture/wall.jpg", 0);
    auto meshBall           = new Mesh(geometryBall, materialBall);
    meshes.push_back(meshBall);
    // earth
    auto geoEarth            = new Sphere(1.0f);
    auto materialEarth       = new PhongMaterial();
    materialEarth->m_shiness = 16.0f;
    materialEarth->m_diffuse = new Texture("asset/texture/earth.jpg", 1);
    auto meshEarth           = new Mesh(geoEarth, materialEarth);
    meshEarth->SetPosition(glm::vec3(2.5f, 0.0f, 0.0f));
    meshes.push_back(meshEarth);
    // 光线
    directionalLight              = new DirectionalLight();
    directionalLight->m_direction = glm::vec3(-1.0f, -1.0f, -1.0f);

    ambient_light          = new AmbientLight();
    ambient_light->m_color = glm::vec3(0.2f);
}
void prepareCamera()
{
    camera = new PerspectiveCamera(static_cast<float>(glApp->getWidth()) / static_cast<float>(glApp->getHeight()));
    camera->m_Position = glm::vec3(0.0f, 0.0f, 5.0f);
    cameraCtl          = new TrackballCameraController(camera);
}

// 聚光灯 平行光 点光
int main()
{
    if (!glApp->init(SCR_WIDTH, SCR_HEIGHT)) return -1;

    // 监听事件
    glApp->setResizeCallback(framebuffer_size_callback);
    glApp->setKeyboardCallback(keyboard_callback);
    glApp->setCursorPosCallback(cursor_position_callback);
    glApp->setScrollCallback(mouse_scroll_callback);
    glApp->setMouseBtnCallback(mouse_btn_callback);

    GL_CALL_AND_CHECK_ERR(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));
    // 清理画布的时候清成啥样
    GL_CALL_AND_CHECK_ERR(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

    prepareCamera();
    prepare();

    // 窗体循环
    while (glApp->update())
    {
        cameraCtl->OnUpdate();
        meshes[1]->SetRotationY(0.2f);
        renderer->render(meshes, camera, directionalLight, ambient_light);
    }
    // 回收资源
    glApp->destroy();
    delete renderer;
    delete spot_light;
    delete ambient_light;
    delete camera;
    delete cameraCtl;
    return 0;
}