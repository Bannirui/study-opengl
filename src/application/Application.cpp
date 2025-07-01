//
// Created by dingrui on 25-6-28.
//

#include "application/Application.h"

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "input/Input.h"

// 初始化静态变量
Application* Application::s_Instance = nullptr;
bool Application::s_shouldClose = false;

Application* Application::getInstance()
{
    if (s_Instance == nullptr) s_Instance = new Application();
    return s_Instance;
}

Application::Application()
{

}

Application::~Application()
{

}

bool Application::init(const uint32_t& width, const uint32_t& height)
{
    m_Width = width;
    m_Height = height;
    // 初始化GLFW的基本环境 OpenGL的版本是3.3
    glfwInit();
    // 主版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    // 次版本
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // 使用核心模式
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // 创建窗体对象
    GLFWwindow* window = glfwCreateWindow(m_Width, m_Height, "LearnOpenGL", nullptr, nullptr);
    if (!window) {
        return false;
    }
    m_Window = window;
    // 窗体对象设置给opengl绘制
    glfwMakeContextCurrent(window);
    // 使用glad加载当前opengl版本的所有函数
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return false;
    }
    // 对glfw窗体事件监听
    glfwSetFramebufferSizeCallback(m_Window, framebufferSizeCallback);
    glfwSetKeyCallback(m_Window, keyboardCallback);
    glfwSetCursorPosCallback(m_Window, mousePosCallback);

    // 把Application单例的实例放到glfw的window中 以后想要Application就从window中拿
    glfwSetWindowUserPointer(m_Window, this);
    // 集成Input
    Input::init(m_Window);
    return true;
}
bool Application::update()
{
    if (glfwWindowShouldClose(m_Window)) return false;
    if (s_shouldClose) glfwSetWindowShouldClose(m_Window, true);
    // 更新键盘状态
    Input::update();
    // 处理输入 不是回调类
    processInput();
    // 接收并分发窗口消息
    glfwPollEvents();
    // 双缓冲 每一帧都执行切换双缓存的动作
    glfwSwapBuffers(m_Window);
    return true;
}
void Application::destroy()
{
    // 关闭glfw窗口对象
    glfwTerminate();
}

void Application::framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    Application* self = (Application*)glfwGetWindowUserPointer(window);
    if (self->m_ResizeCallback) self->m_ResizeCallback(width, height);
}
void Application::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Application* self = (Application*)glfwGetWindowUserPointer(window);
    if (self->m_KeyboardCallback) self->m_KeyboardCallback(key, scancode, action, mods);
}
void Application::mousePosCallback(GLFWwindow *window, double x, double y) {
    Application* self = (Application*)glfwGetWindowUserPointer(window);
    if (self->m_MousePosCallback) self->m_MousePosCallback(x, y);
}

void Application::processInput() {
    // todo
}
void Application::setShouldClose(bool flag) {
    s_shouldClose = flag;
}


