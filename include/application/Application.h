//
// Created by dingrui on 25-6-28.
//

#pragma once

#include <cstdint>
#include <memory>

#include "glm/vec4.hpp"

#define glApp Application::get_instance()

// 前向声明
struct GLFWwindow;
class Input;

// 单例
class Application
{
public:
    ~Application();

    static Application *get_instance();

    uint32_t get_width() const { return m_Width; }
    uint32_t get_height() const { return m_Height; }

    // glfw窗口初始化 glad函数加载
    bool Init(const uint32_t &width, const uint32_t &height);
    // 帧循环
    bool Update();

    // 整合imgui
    static void InitImGui();
    /**
     * 每一帧渲染gui
     * <ul>
     *   <li>开启当前imgui渲染</li>
     *   <li>gui控件</li>
     *   <li>执行ui渲染</li>
     * </ul>
     */
    void RenderImGui();

    static void setShouldClose(bool flag) { s_shouldClose = flag; };

    GLFWwindow *get_window() const { return m_Window; }
    Input* get_input() const { return m_input.get(); }

    // 获取鼠标位置
    void GetMousePos(double *x, double *y) const;

    glm::vec4 get_clearColor() const { return m_clearColor; }
    void set_clearColor(const glm::vec4 &color) { m_clearColor = color; }

private:
    Application() = default;
    void registerCallback() const;
    // 处理输入 不是回调类的事件
    void processInput();
    // glfw窗口关闭
    static void destroy();

    // glfw窗体事件回调->Application转发给Input
    /**
     * @param window 哪个窗体发生了变化
     * @param width 新的宽度
     * @param height 新的高度
     */
    static void framebufferSizeCallbackDispatch(GLFWwindow *window, int width, int height);
    // glfw键盘事件回调
    static void keyboardCallbackDispatch(GLFWwindow *window, int key, int scancode, int action, int mods);
    // glfw鼠标位置回调
    static void mousePosCallbackDispatch(GLFWwindow *window, double x, double y);
    /**
     * gflw鼠标滚轮缩放回调
     * @param yOffset 大于0表示向上滚动 小于0表示向下滚动
     */
    static void mouseScrollCallbackDispatch(GLFWwindow *window, double xOffset, double yOffset);
    // 鼠标点击事件
    static void mouseBtnCallbackDispatch(GLFWwindow *window, int button, int action, int mods);

private:
    // 单例对象
    static Application *s_Instance;
    static bool s_shouldClose;

    // 窗体宽度
    uint32_t m_Width{0};
    // 窗体高度
    uint32_t m_Height{0};
    // 窗体对象
    GLFWwindow *m_Window{nullptr};

    glm::vec4 m_clearColor{0.0f};

    std::unique_ptr<Input> m_input;
};
