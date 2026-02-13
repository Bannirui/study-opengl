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
#include "input/input.h"
#include "glframework/x_config.h"

// 初始化静态变量
bool Application::s_shouldClose = false;

Application::~Application()
{
    destroy();
}

bool Application::Init(const uint32_t& width, const uint32_t& height)
{
    // log
    XLog::Init();

    m_Width  = width;
    m_Height = height;
    if (!glfwInit())
    {
        return false;
    }
    // 初始化GLFW的基本环境 OpenGL的版本
    // 主版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, OPENGL_VERSION_MAJOR);
    // 次版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, OPENGL_VERSION_MINOR);
    // 使用核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // 创建窗体对象
    m_Window = glfwCreateWindow(m_Width, m_Height, "LearnOpenGL", nullptr, nullptr);
    if (!m_Window)
    {
        return false;
    }
    // 窗体对象设置给opengl绘制
    glfwMakeContextCurrent(m_Window);
    // 使用glad加载当前opengl版本的所有函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        return false;
    }
    // input事件派发对glfw窗体事件的监听
    m_input = std::make_unique<Input>();
    // 把Application单例的实例放到glfw的window中 以后想要Application就从window中拿
    glfwSetWindowUserPointer(m_Window, this);
    // 对glfw窗体事件监听
    registerCallback();

    // 整合imgui
    ImGui::CreateContext();
    // 主题
    ImGui::StyleColorsDark();
    // imgui绑定glfw
    ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
    // imgui绑定opengl
    ImGui_ImplOpenGL3_Init(X_GL_VERSION_STR);
    return true;
}

void Application::Run()
{
    OnInit();

    float lasTime = glfwGetTime();
    while (!glfwWindowShouldClose(m_Window))
    {
        if (s_shouldClose)
        {
            glfwSetWindowShouldClose(m_Window, true);
        }
        float currentTime = glfwGetTime();
        float dt          = currentTime - lasTime;
        lasTime           = currentTime;

        // 接收并分发窗口消息
        glfwPollEvents();

        OnUpdate(dt);

        OnRender();

        // 开启imgui的渲染
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        OnImGui();

        // 渲染
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        // 双缓冲 每一帧都执行切换双缓存的动作
        glfwSwapBuffers(m_Window);
    }
    OnShutdown();
}

void Application::registerCallback() const
{
    glfwSetFramebufferSizeCallback(m_Window, framebufferSizeCallbackDispatch);
    glfwSetKeyCallback(m_Window, keyboardCallbackDispatch);
    glfwSetCursorPosCallback(m_Window, mousePosCallbackDispatch);
    glfwSetScrollCallback(m_Window, mouseScrollCallbackDispatch);
    glfwSetMouseButtonCallback(m_Window, mouseBtnCallbackDispatch);
}

void Application::destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    if (m_Window)
    {
        glfwDestroyWindow(m_Window);
    }
    // 关闭glfw窗口对象
    glfwTerminate();
}

void Application::framebufferSizeCallbackDispatch(GLFWwindow* window, int width, int height)
{
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app)
    {
        app->m_Width  = width;
        app->m_Height = height;
        app->OnResize(width, height);
    }
    if (app && app->m_input)
    {
        app->m_input->OnResize(width, height);
    }
}

void Application::keyboardCallbackDispatch(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app)
    {
        return;
    }
    if (app->m_input)
    {
        app->m_input->OnKey(key, scancode, action, mods);
    }
}

void Application::mousePosCallbackDispatch(GLFWwindow* window, double x, double y)
{
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app && app->m_input)
    {
        app->m_input->OnCursor(x, y);
    }
}

void Application::mouseScrollCallbackDispatch(GLFWwindow* window, double x, double y)
{
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (app && app->m_input)
    {
        app->m_input->OnScroll(x, y);
    }
}

void Application::mouseBtnCallbackDispatch(GLFWwindow* window, int button, int action, int mods)
{
    auto* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (!app)
    {
        return;
    }
    double x, y;
    glfwGetCursorPos(app->m_Window, &x, &y);
    if (app->m_input)
    {
        app->m_input->OnMouse(button, action, mods, x, y);
    }
}
