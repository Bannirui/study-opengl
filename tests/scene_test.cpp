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
#include "glframework/material/PhongMaterial.h"
#include "glframework/material/WhiteMaterial.h"
#include "glframework/renderer/Renderer.h"

#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "glframework/renderer/light_pack.h"

const unsigned int SCR_WIDTH  = 1600;
const unsigned int SCR_HEIGHT = 800;

// 相机控制器
std::unique_ptr<CameraController> cameraCtl;

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
    if (cameraCtl)
    {
        cameraCtl->OnKey(key, action, mods);
    }
}

void cursor_position_callback(double x, double y)
{
    std::cout << "鼠标位置发生了变化 现在的 x=" << x << ", y=" << y << std::endl;
    if (cameraCtl)
    {
        cameraCtl->OnCursor(x, y);
    }
}

void mouse_scroll_callback(double yoffset)
{
    if (yoffset > 0)
        std::cout << "鼠标滚轮放大 yoffset: " << yoffset << std::endl;
    else
        std::cout << "鼠标滚轮缩小 yoffset: " << yoffset << std::endl;
    if (cameraCtl)
    {
        cameraCtl->OnScroll(yoffset);
    }
}

void mouse_btn_callback(int button, int action, int mods)
{
    double x, y;
    glApp->GetMousePos(&x, &y);
    std::cout << "button=" << button << ", action=" << action << ", mods=" << mods << ", x=" << x << ", y=" << y
              << std::endl;
    if (cameraCtl)
    {
        cameraCtl->OnMouse(button, action, mods, x, y);
    }
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

    // 渲染器
    std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>();
    // 场景
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    // 箱子
    std::shared_ptr<Box>           boxGeometry = std::make_shared<Box>();
    std::shared_ptr<PhongMaterial> boxMaterial = std::make_shared<PhongMaterial>();
    boxMaterial->set_shines(32.0f);
    boxMaterial->set_diffuse(new Texture("asset/texture/box.png", 0));
    boxMaterial->set_specular_mask(new Texture("asset/texture/sp_mask.png", 1));
    std::shared_ptr<Mesh> boxMesh = std::make_shared<Mesh>(boxGeometry, boxMaterial);
    boxMesh->SetRotationY(-15.0f);
    boxMesh->SetRotationX(15.0f);
    // 白色物体
    std::shared_ptr<Sphere>        whiteObjGeometry = std::make_shared<Sphere>(0.1f);
    std::shared_ptr<WhiteMaterial> whiteObjMaterial = std::make_shared<WhiteMaterial>();
    std::shared_ptr<Mesh>          whiteObjMesh     = std::make_shared<Mesh>(whiteObjGeometry, whiteObjMaterial);
    whiteObjMesh->SetPosition(glm::vec3(2.0f, 0.0f, 0.0f));

    // todo
    boxMesh->AddChild(whiteObjMesh);
    scene->AddChild(boxMesh);

    // 光线
    std::shared_ptr<SpotLight> spotLight = std::make_shared<SpotLight>();
    spotLight->m_innerAngle              = 15.0f;
    spotLight->m_outerAngle              = 30.0f;
    spotLight->SetPosition(whiteObjMesh->GetPosition());

    std::shared_ptr<DirectionalLight> directionalLight = std::make_shared<DirectionalLight>();
    directionalLight->m_direction                      = glm::vec3(1.0f, 0.0f, 0.0f);

    std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>();
    pointLight->SetPosition(glm::vec3(0.0f, 0.0f, 1.5f));
    pointLight->set_specular_intensity(0.5f);
    pointLight->m_k2 = 0.017f;
    pointLight->m_k1 = 0.07f;
    pointLight->m_kc = 1.0f;

    std::shared_ptr<AmbientLight> ambientLight = std::make_shared<AmbientLight>();
    ambientLight->set_color(glm::vec3(0.2f));

    struct LightPack lights;
    lights.directional = directionalLight;
    lights.point       = pointLight;
    lights.spot        = spotLight;
    lights.ambient     = ambientLight;

    // 相机
    PerspectiveCamera camera(static_cast<float>(glApp->getWidth()) / static_cast<float>(glApp->getHeight()));
    camera.m_Position = glm::vec3(0.0f, 0.0f, 5.0f);
    // 相机控制器
    cameraCtl = std::make_unique<TrackballCameraController>(camera);

    initIMGUI();

    // 窗体循环
    while (glApp->update())
    {
        cameraCtl->OnUpdate();

        renderer->setClearColor(clear_color);
        // 每一帧清一次屏
        Renderer::BeginFrame();
        renderer->render(scene, camera, lights);

        // imgui渲染
        renderIMGUI();
    }
    // 回收资源
    glApp->destroy();
    return 0;
}