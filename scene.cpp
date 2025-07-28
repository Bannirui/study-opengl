//
// Created by dingrui on 25-6-23.
//

#include <iostream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>

#include "err_check.h"
#include "application/Application.h"
#include "application/camera/Camera.h"
#include "application/camera/CameraController.h"
#include "application/camera/PerspectiveCamera.h"
#include "application/camera/TrackballCameraController.h"
#include "glframework/Mesh.h"
#include "glframework/Scene.h"
#include "glframework/Texture.h"
#include "glframework/geo/Box.h"
#include "glframework/geo/Sphere.h"
#include "glframework/light/AmbientLight.h"
#include "glframework/light/DirectionalLight.h"
#include "glframework/light/PointLight.h"
#include "glframework/light/SpotLight.h"
#include "glframework/material/PhoneMaterial.h"
#include "glframework/material/WhiteMaterial.h"
#include "glframework/renderer/Renderer.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

const unsigned int SCR_WIDTH  = 1600;
const unsigned int SCR_HEIGHT = 800;

Renderer*         renderer         = nullptr;
Scene*            scene            = nullptr;
DirectionalLight* directionalLight = nullptr;
SpotLight*        spot_light       = nullptr;
AmbientLight*     ambient_light    = nullptr;
PointLight*       point_light      = nullptr;

Camera*           camera    = nullptr;
CameraController* cameraCtl = nullptr;

glm::vec3 clear_color = glm::vec3(0.0f, 0.0f, 0.0f);

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
    app->GetMousePos(&x, &y);
    std::cout << "button=" << button << ", action=" << action << ", mods=" << mods << ", x=" << x << ", y=" << y
              << std::endl;
    cameraCtl->OnMouse(button, action, mods, x, y);
}

void prepare()
{
    renderer = new Renderer();
    scene    = new Scene();
    // 箱子
    auto geometryBox            = new Box();
    auto materialBox            = new PhoneMaterial();
    materialBox->m_shiness      = 32.0f;
    materialBox->m_diffuse      = new Texture("resources/texture/box.png", 0);
    materialBox->m_specularMask = new Texture("resources/texture/sp_mask.png", 1);
    auto meshBox                = new Mesh(geometryBox, materialBox);
    // 白色物体
    auto geometryWhite = new Sphere(0.1f);
    auto materialWhite = new WhiteMaterial();
    auto meshWhite     = new Mesh(geometryWhite, materialWhite);
    meshWhite->SetPosition(glm::vec3(2.0f, 0.0f, 0.0f));

    meshBox->AddChild(meshWhite);
    scene->AddChild(meshBox);

    // 光线
    spot_light               = new SpotLight();
    spot_light->m_innerAngle = 15.0f;
    spot_light->m_outerAngle = 30.0f;
    spot_light->SetPosition(meshWhite->GetPosition());

    directionalLight              = new DirectionalLight();
    directionalLight->m_direction = glm::vec3(1.0f, 0.0f, 0.0f);

    point_light = new PointLight();
    point_light->SetPosition(glm::vec3(0.0f, 0.0f, 1.5f));
    point_light->m_specularIntensity = 0.5f;
    point_light->m_k2                = 0.017f;
    point_light->m_k1                = 0.07f;
    point_light->m_kc                = 1.0f;

    ambient_light          = new AmbientLight();
    ambient_light->m_color = glm::vec3(0.2f);
}
void prepareCamera()
{
    camera = new PerspectiveCamera(static_cast<float>(app->getWidth()) / static_cast<float>(app->getHeight()));
    camera->m_Position = glm::vec3(0.0f, 0.0f, 5.0f);
    cameraCtl          = new TrackballCameraController(camera);
}

// 整合imgui
void initIMGUI()
{
    ImGui::CreateContext();
    // 主题
    ImGui::StyleColorsDark();
    // imgui绑定glfw
    ImGui_ImplGlfw_InitForOpenGL(app->getWindow(), true);
    // imgui绑定opengl
    ImGui_ImplOpenGL3_Init("#version 330 core");
}

/**
 * 每一帧渲染gui
 * <ul>
 *   <li>开启当前imgui渲染</li>
 *   <li>gui控件</li>
 *   <li>执行ui渲染</li>
 * </ul>
 */
void renderIMGUI()
{
    // 开启
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // 控件
    ImGui::Begin("Settings");
    ImGui::Text("hello world");
    ImGui::Button("test btn", ImVec2(40, 20));
    ImGui::ColorEdit3("clear color", reinterpret_cast<float*>(&clear_color));
    ImGui::End();
    // 渲染
    ImGui::Render();
    int display_w, display_h;
    glfwGetWindowSize(app->getWindow(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

// 聚光灯 平行光 点光
int main()
{
    if (!app->init(SCR_WIDTH, SCR_HEIGHT)) return -1;

    // 监听事件
    app->setResizeCallback(framebuffer_size_callback);
    app->setKeyboardCallback(keyboard_callback);
    app->setCursorPosCallback(cursor_position_callback);
    app->setScrollCallback(mouse_scroll_callback);
    app->setMouseBtnCallback(mouse_btn_callback);

    GL_CALL_AND_CHECK_ERR(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));
    // 清理画布的时候清成啥样
    GL_CALL_AND_CHECK_ERR(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

    prepareCamera();
    prepare();
    initIMGUI();

    // 窗体循环
    while (app->update())
    {
        cameraCtl->OnUpdate();

        renderer->setClearColor(clear_color);
        renderer->render(scene, camera, directionalLight, point_light, ambient_light, spot_light);

        // imgui渲染
        renderIMGUI();
    }
    // 回收资源
    app->destroy();
    delete renderer;
    delete spot_light;
    delete ambient_light;
    delete camera;
    delete cameraCtl;
    return 0;
}