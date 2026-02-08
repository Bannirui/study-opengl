//
// Created by dingrui on 25-6-28.
//

#pragma once

#include <cstdint>

#include "glm/vec4.hpp"

#define glApp Application::getInstance()

// 前向声明
struct GLFWwindow;

// 函数指针类型 窗体大小变化后回调的函数
using ResizeCallback = void (*)(int width, int height);
// 键盘事件
using KeyboardCallback = void (*)(int key, int scancode, int action, int mods);
// 鼠标位置
using MousePosCallbackPtr = void (*)(double x, double y);
// 鼠标滚轮缩放 glfw的滚轮缩放有两个向量 常规的鼠标滚轮是yOffset
// 有的鼠标滚轮可以左右拨动 触摸板也可以左右 所以有x向量
// @Param yOffset 缩放量 >0表示向上滚动 放大 <0表示向下滚动 缩小
using MouseScrollCallbackPtr = void (*)(double xOffset, double yOffset);
using MouseBtnCallbackPtr = void (*)(int button, int action, int mods);

// 单例类
class Application {
public:
    ~Application();

    static Application *getInstance();

    uint32_t get_width() const { return m_Width; }
    uint32_t get_height() const { return m_Height; }

    // glfw窗口初始化 glad函数加载
    bool Init(const uint32_t &width, const uint32_t &height);

    void RegisterCallback();


    // 整合imgui
    void InitImGui();

    /**
     * 每一帧渲染gui
     * <ul>
     *   <li>开启当前imgui渲染</li>
     *   <li>gui控件</li>
     *   <li>执行ui渲染</li>
     * </ul>
     */
    void RenderImGui();

    // 帧循环
    bool Update();

    static void setShouldClose(bool flag);

    GLFWwindow *getWindow() const { return m_Window; }

    // 获取鼠标位置
    void GetMousePos(double *x, double *y);

    glm::vec4 get_clearColor() const { return m_clearColor; }
    void set_clearColor(const glm::vec4 &color) { m_clearColor = color; }

private:
    Application() = default;

    // glfw窗口关闭
    void destroy();

    // glfw窗体事件回调
    // @Param window 哪个窗体发生了变化
    // @Param width 新的宽度
    // @Param height 新的高度
    static void framebufferSizeCallback(GLFWwindow *window, int width, int height);

    // glfw键盘事件回调
    static void keyboardCallback(GLFWwindow *window, int key, int scancode, int action, int mods);

    // glfw鼠标位置回调
    static void mousePosCallback(GLFWwindow *window, double x, double y);

    // gflw鼠标滚轮缩放回调
    // @Param yOffset 缩放量 >0表示向上滚动 放大 <0表示向下滚动 缩小
    static void mouseScrollCallback(GLFWwindow *window, double xOffset, double yOffset);

    // 鼠标点击事件
    static void mouseBtnCallback(GLFWwindow *window, int button, int action, int mods);

    // 处理输入 不是回调类的事件
    void processInput();

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

    // 窗体大小变化回调的函数指针
    ResizeCallback m_ResizeCallback{nullptr};
    // 设置键盘事件回调函数
    KeyboardCallback m_KeyboardCallback{nullptr};
    // 设置鼠标位置变化监听
    MousePosCallbackPtr m_MousePosCallback{nullptr};
    // 设置鼠标滚轮缩放事件
    MouseScrollCallbackPtr m_MouseScrollCallback{nullptr};
    // 鼠标点击事件
    MouseBtnCallbackPtr m_MouseBtnCallback{nullptr};

    glm::vec4 m_clearColor{0.0f};
};
