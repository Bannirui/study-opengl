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
#include "glframework/Texture.h"
#include "glframework/geo/Box.h"
#include "glframework/geo/Sphere.h"
#include "glframework/light/AmbientLight.h"
#include "glframework/light/DirectionalLight.h"
#include "glframework/light/PointLight.h"
#include "glframework/light/SpotLight.h"
#include "glframework/material/PhongMaterial.h"
#include "glframework/material/WhiteMaterial.h"
#include "glframework/renderer/Renderer.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

const unsigned int SCR_WIDTH  = 1600;
const unsigned int SCR_HEIGHT = 800;

std::unique_ptr<Renderer> renderer;
// 渲染列表
std::vector<Mesh*>                meshes{};
std::unique_ptr<DirectionalLight> directionalLight;
std::unique_ptr<SpotLight>        spot_light;
std::unique_ptr<AmbientLight>     ambient_light;
std::unique_ptr<PointLight>       point_light;

std::unique_ptr<Camera>           camera;
std::unique_ptr<CameraController> cameraCtl;

std::unique_ptr<Box>           boxGeometry;
std::unique_ptr<PhongMaterial> boxMaterial;
std::unique_ptr<Sphere>        whiteObjectGeometry;
std::unique_ptr<WhiteMaterial> whiteObjectMaterial;

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
    glApp->GetMousePos(&x, &y);
    std::cout << "button=" << button << ", action=" << action << ", mods=" << mods << ", x=" << x << ", y=" << y
              << std::endl;
    cameraCtl->OnMouse(button, action, mods, x, y);
}

void prepare()
{
    renderer = std::make_unique<Renderer>();
    // 箱子
    boxGeometry                 = std::make_unique<Box>();
    boxMaterial                 = std::make_unique<PhongMaterial>();
    boxMaterial->m_shiness      = 32.0f;
    boxMaterial->m_diffuse      = new Texture("asset/texture/box.png", 0);
    boxMaterial->m_specularMask = new Texture("asset/texture/sp_mask.png", 1);
    auto meshBox                = new Mesh(*boxGeometry, *boxMaterial);
    meshes.push_back(meshBox);
    // 白色物体
    whiteObjectGeometry = std::make_unique<Sphere>(0.1f);
    whiteObjectMaterial = std::make_unique<WhiteMaterial>();
    auto meshWhite      = new Mesh(*whiteObjectGeometry, *whiteObjectMaterial);
    meshWhite->SetPosition(glm::vec3(2.0f, 0.0f, 0.0f));
    meshes.push_back(meshWhite);
    // 光线
    spot_light               = std::make_unique<SpotLight>();
    spot_light->m_innerAngle = 15.0f;
    spot_light->m_outerAngle = 30.0f;
    spot_light->SetPosition(meshWhite->GetPosition());

    directionalLight              = std::make_unique<DirectionalLight>();
    directionalLight->m_direction = glm::vec3(1.0f, 0.0f, 0.0f);

    point_light = std::make_unique<PointLight>();
    point_light->SetPosition(glm::vec3(0.0f, 0.0f, 1.5f));
    point_light->m_specularIntensity = 0.5f;
    point_light->m_k2                = 0.017f;
    point_light->m_k1                = 0.07f;
    point_light->m_kc                = 1.0f;

    ambient_light          = std::make_unique<AmbientLight>();
    ambient_light->m_color = glm::vec3(0.2f);
}

void prepareCamera()
{
    camera             = std::make_unique<PerspectiveCamera>(static_cast<float>(glApp->getWidth()) /
                                                             static_cast<float>(glApp->getHeight()));
    camera->m_Position = glm::vec3(0.0f, 0.0f, 5.0f);
    cameraCtl          = std::make_unique<TrackballCameraController>(*camera);
}

// 点光跟着白球的位置 让白球运动起来 点光位置就会变化
void meshTransform()
{
    float xPos = glm::sin(glfwGetTime()) + 2.0f;
    meshes[1]->SetPosition(glm::vec3(xPos, 0.0f, 0.0f));
    spot_light->SetPosition(meshes[1]->GetPosition());

    meshes[0]->SetRotationX(1.0f);
    meshes[0]->SetRotationY(0.4f);
    meshes[0]->SetRotationZ(0.1f);
}

// 整合imgui
void initIMGUI()
{
    ImGui::CreateContext();
    // 主题
    ImGui::StyleColorsDark();
    // imgui绑定glfw
    ImGui_ImplGlfw_InitForOpenGL(glApp->getWindow(), true);
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
    glfwGetWindowSize(glApp->getWindow(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
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
    initIMGUI();

    // 窗体循环
    while (glApp->update())
    {
        cameraCtl->OnUpdate();
        meshTransform();
        renderer->setClearColor(clear_color);
        renderer->render(meshes, camera.get(),
                         {directionalLight.get(), point_light.get(), spot_light.get(), ambient_light.get()});
        // imgui渲染
        renderIMGUI();
    }
    // 回收资源
    glApp->destroy();
    return 0;
}