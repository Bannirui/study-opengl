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
#include "glframework/Scene.h"
#include "glframework/Texture.h"
#include "glframework/geo/Sphere.h"
#include "glframework/light/AmbientLight.h"
#include "glframework/light/DirectionalLight.h"
#include "glframework/light/PointLight.h"
#include "glframework/renderer/Renderer.h"
#include "glframework/renderer/light_pack.h"
#include "glframework/x_config.h"

const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 800;

std::unique_ptr<CameraController> cameraCtl;

glm::vec3 clear_color = glm::vec3(0.0f, 0.0f, 0.0f);

void framebuffer_size_callback(int width, int height) {
    std::cout << "新窗口大小 w:" << width << ", h:" << height << std::endl;
    // 视口 设置窗口中opengl负责渲染的区域
    // x y将相对窗口左下角的起始位置
    // width height渲染区域的长度 高度
    GL_CALL_AND_CHECK_ERR(glViewport(0, 0, width, height));
}

void keyboard_callback(int key, int scancode, int action, int mods) {
    std::cout << "键盘事件 键位" << static_cast<char>(key) << ", 操作" << action << ", 有没有ctrl/shift功能键" << mods
            << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) Application::setShouldClose(true);
    if (cameraCtl) {
        cameraCtl->OnKey(key, action, mods);
    }
}

void cursor_position_callback(double x, double y) {
    std::cout << "鼠标位置发生了变化 现在的 x=" << x << ", y=" << y << std::endl;
    if (cameraCtl) {
        cameraCtl->OnCursor(x, y);
    }
}

void mouse_scroll_callback(double yoffset) {
    if (yoffset > 0)
        std::cout << "鼠标滚轮放大 yoffset: " << yoffset << std::endl;
    else
        std::cout << "鼠标滚轮缩小 yoffset: " << yoffset << std::endl;
    if (cameraCtl) {
        cameraCtl->OnScroll(yoffset);
    }
}

void mouse_btn_callback(int button, int action, int mods) {
    double x, y;
    glApp->GetMousePos(&x, &y);
    std::cout << "button=" << button << ", action=" << action << ", mods=" << mods << ", x=" << x << ", y=" << y
            << std::endl;
    if (cameraCtl) {
        cameraCtl->OnMouse(button, action, mods, x, y);
    }
}

// 整合imgui
void initIMGUI() {
    ImGui::CreateContext();
    // 主题
    ImGui::StyleColorsDark();
    // imgui绑定glfw
    ImGui_ImplGlfw_InitForOpenGL(glApp->getWindow(), true);
    // imgui绑定opengl
    ImGui_ImplOpenGL3_Init("#version " X_GLSL_VERSION_STR " core");
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
    glApp->setResizeCallback(framebuffer_size_callback);
    glApp->setKeyboardCallback(keyboard_callback);
    glApp->setCursorPosCallback(cursor_position_callback);
    glApp->setScrollCallback(mouse_scroll_callback);
    glApp->setMouseBtnCallback(mouse_btn_callback);

    GL_CALL_AND_CHECK_ERR(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));
    // 清理画布的时候清成啥样
    GL_CALL_AND_CHECK_ERR(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

    // 渲染器
    Renderer renderer;
    // 渲染场景
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    auto model = AssimpLoader::load("asset/fbx/backpack/backpack.obj");
    scene->AddChild(model);
    // 有个初始角度方便观察
    // todo 现在没有生效 应该是因为加载进来的模型自带了世界坐标 我在渲染的时候并没有把模型自己的坐标系因素也加进来
    scene->SetAngleX(30.0f);
    scene->SetAngleY(30.0f);
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
    cameraCtl = std::make_unique<TrackballCameraController>(camera);

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
