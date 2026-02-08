//
// Created by dingrui on 25-6-28.
//

#include "application/Application.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "x_log.h"
#include "input/input_dispatcher.h"
#include "glframework/x_config.h"

// 初始化静态变量
Application *Application::s_Instance = nullptr;
bool Application::s_shouldClose = false;

Application *Application::getInstance() {
    if (s_Instance == nullptr) s_Instance = new Application();
    return s_Instance;
}

Application::~Application() {
    destroy();
}

bool Application::Init(const uint32_t &width, const uint32_t &height) {
    // log
    XLog::Init();

    m_Width = width;
    m_Height = height;
    // 初始化GLFW的基本环境 OpenGL的版本
    glfwInit();
    // 主版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    // 次版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    // 使用核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // 创建窗体对象
    GLFWwindow *window = glfwCreateWindow(m_Width, m_Height, "LearnOpenGL", nullptr, nullptr);
    if (!window) {
        return false;
    }
    m_Window = window;
    // 窗体对象设置给opengl绘制
    glfwMakeContextCurrent(window);
    // 使用glad加载当前opengl版本的所有函数
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        return false;
    }
    // 对glfw窗体事件监听
    glfwSetFramebufferSizeCallback(m_Window, framebufferSizeCallback);
    glfwSetKeyCallback(m_Window, keyboardCallback);
    glfwSetCursorPosCallback(m_Window, mousePosCallback);
    glfwSetScrollCallback(m_Window, mouseScrollCallback);
    glfwSetMouseButtonCallback(m_Window, mouseBtnCallback);

    // 把Application单例的实例放到glfw的window中 以后想要Application就从window中拿
    glfwSetWindowUserPointer(m_Window, this);
    return true;
}

void Application::RegisterCallback() {
    m_ResizeCallback = InputDispatcher::OnResize;
    m_KeyboardCallback = InputDispatcher::OnKey;
    m_MousePosCallback = InputDispatcher::OnCursor;
    m_MouseScrollCallback = InputDispatcher::OnScroll;
    m_MouseBtnCallback = InputDispatcher::OnMouse;
}

void Application::InitImGui() {
    ImGui::CreateContext();
    // 主题
    ImGui::StyleColorsDark();
    // imgui绑定glfw
    ImGui_ImplGlfw_InitForOpenGL(glApp->getWindow(), true);
    // imgui绑定opengl
    ImGui_ImplOpenGL3_Init(X_GL_VERSION_STR);
}

void Application::RenderImGui() {
    // 开启imgui的渲染
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    // 控件
    ImGui::Begin("Settings");
    ImGui::Text("hello world");
    ImGui::Button("test btn", ImVec2(40, 20));
    ImGui::ColorEdit3("clear color", reinterpret_cast<float *>(&m_clearColor));
    ImGui::End();
    // 渲染
    ImGui::Render();
    int display_w, display_h;
    glfwGetWindowSize(glApp->getWindow(), &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

bool Application::Update() {
    if (glfwWindowShouldClose(m_Window)) return false;
    if (s_shouldClose) glfwSetWindowShouldClose(m_Window, true);
    // 处理输入 不是回调类
    processInput();
    // 接收并分发窗口消息
    glfwPollEvents();
    // 双缓冲 每一帧都执行切换双缓存的动作
    glfwSwapBuffers(m_Window);
    return true;
}

void Application::destroy() {
    // 关闭glfw窗口对象
    glfwTerminate();
}

void Application::framebufferSizeCallback(GLFWwindow *window, int width, int height) {
    Application *self = (Application *) glfwGetWindowUserPointer(window);
    if (self->m_ResizeCallback) self->m_ResizeCallback(width, height);
}

void Application::keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
    Application *self = (Application *) glfwGetWindowUserPointer(window);
    if (self->m_KeyboardCallback) self->m_KeyboardCallback(key, scancode, action, mods);
}

void Application::mousePosCallback(GLFWwindow *window, double x, double y) {
    Application *self = (Application *) glfwGetWindowUserPointer(window);
    if (self->m_MousePosCallback) self->m_MousePosCallback(x, y);
}

void Application::mouseScrollCallback(GLFWwindow *window, double x, double y) {
    Application *self = (Application *) glfwGetWindowUserPointer(window);
    if (self->m_MouseScrollCallback) self->m_MouseScrollCallback(x, y);
}

void Application::mouseBtnCallback(GLFWwindow *window, int button, int action, int mods) {
    Application *self = (Application *) glfwGetWindowUserPointer(window);
    if (self->m_MouseBtnCallback) self->m_MouseBtnCallback(button, action, mods);
}

void Application::processInput() {
    // todo
}

void Application::setShouldClose(bool flag) {
    s_shouldClose = flag;
}

void Application::GetMousePos(double *x, double *y) {
    glfwGetCursorPos(m_Window, x, y);
}
