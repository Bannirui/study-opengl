//
// Created by dingrui on 25-6-28.
//

#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "application/Application.h"

// 初始化静态变量
Application* Application::s_Instance = nullptr;

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
    return true;
}
bool Application::update()
{
    if (glfwWindowShouldClose(m_Window)) return false;
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
