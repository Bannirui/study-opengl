#include <iostream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "err_check.h"
#include "application/Application.h"
#include "application/assimpLoader.h"
#include "application/camera/CameraController.h"
#include "application/camera/PerspectiveCamera.h"
#include "application/camera/TrackballCameraController.h"
#include "glframework/Mesh.h"
#include "glframework/Scene.h"
#include "glframework/Texture.h"
#include "glframework/x_config.h"
#include "glframework/geo/Plane.h"
#include "glframework/geo/Sphere.h"
#include "glframework/light/AmbientLight.h"
#include "glframework/light/DirectionalLight.h"
#include "glframework/light/PointLight.h"
#include "glframework/material/DepthMaterial.h"
#include "glframework/material/PhongMaterial.h"
#include "glframework/renderer/Renderer.h"
#include "glframework/renderer/light_pack.h"
#include "input/input_dispatcher.h"

const unsigned int SCR_WIDTH = 1200;
const unsigned int SCR_HEIGHT = 800;

glm::vec4 clear_color = glm::vec4(1.0f, 0.5f, 0.2f, 1.0f);

// 整合imgui
void initIMGUI() {
    ImGui::CreateContext();
    // 主题
    ImGui::StyleColorsDark();
    // imgui绑定glfw
    ImGui_ImplGlfw_InitForOpenGL(glApp->getWindow(), true);
    // imgui绑定opengl
    ImGui_ImplOpenGL3_Init(X_GL_VERSION_STR);
}

/**
 * 每一帧渲染gui
 * <ul>
 *   <li>开启当前imgui渲染</li>
 *   <li>gui控件</li>
 *   <li>执行ui渲染</li>
 * </ul>
 */
void renderIMGUI() {
    // 开启imgui的渲染
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // 控件
    ImGui::Begin("Settings");
    ImGui::Text("hello world");
    ImGui::Button("test btn", ImVec2(40, 20));
    ImGui::ColorEdit3("clear color", reinterpret_cast<float *>(&clear_color));
    ImGui::End();
    // 渲染
    ImGui::Render();
    int display_w, display_h;
    glfwGetWindowSize(glApp->getWindow(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main() {
    if (!glApp->init(SCR_WIDTH, SCR_HEIGHT)) return -1;
    // 监听事件
    InputDispatcher inputDispatcher(glApp);
    glApp->set_resizeCallback(InputDispatcher::OnResize);
    glApp->set_keyboardCallback(InputDispatcher::OnKey);
    glApp->set_cursorPosCallback(InputDispatcher::OnCursor);
    glApp->set_scrollCallback(InputDispatcher::OnScroll);
    glApp->set_mouseBtnCallback(InputDispatcher::OnMouse);

    // 渲染器
    Renderer renderer;

    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    std::shared_ptr<Plane> geometry = std::make_unique<Plane>(5.0f, 5.0f);
    std::shared_ptr<DepthMaterial> material = std::make_shared<DepthMaterial>();

    std::shared_ptr<Mesh> meshA = std::make_shared<Mesh>(geometry, material);
    scene->AddChild(meshA);

    std::shared_ptr<Mesh> meshB = std::make_shared<Mesh>(geometry, material);
    meshB->SetPosition(glm::vec3(2.0f, 0.5f, -1.0f));
    scene->AddChild(meshB);

    std::shared_ptr<Mesh> meshC = std::make_shared<Mesh>(geometry, material);
    meshC->SetPosition(glm::vec3(4.0f, 1.0f, -2.0f));
    scene->AddChild(meshC);

    // 光线
    std::shared_ptr<DirectionalLight> directionalLight = std::make_shared<DirectionalLight>();
    // 光源从右后方
    directionalLight->m_direction = glm::vec3(-1.0f, -1.0f, -1.0f);
    directionalLight->set_specular_intensity(1.0f);
    std::shared_ptr<AmbientLight> ambientLight = std::make_shared<AmbientLight>();
    ambientLight->set_color(glm::vec3(0.2f));
    struct LightPack lights;
    lights.directional = directionalLight;
    lights.ambient = ambientLight;
    PerspectiveCamera camera(static_cast<float>(glApp->getWidth()) / static_cast<float>(glApp->getHeight()));
    camera.set_position(glm::vec3(0.0f, 0.0f, 5.0f));
    // 相机控制器
    inputDispatcher.CreateCameraController<TrackballCameraController>(camera);
    auto cameraCtl = inputDispatcher.get_CameraController();

    initIMGUI();

    // 窗体循环
    while (glApp->update()) {
        cameraCtl->OnUpdate();

        renderer.setClearColor(clear_color);
        // 每一帧清一次屏
        Renderer::BeginFrame();
        renderer.render(scene, camera, lights);

        // imgui渲染
        renderIMGUI();
    }
    // 回收资源
    glApp->destroy();
    return 0;
}
