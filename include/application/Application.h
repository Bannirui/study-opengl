//
// Created by dingrui on 25-6-28.
//

#pragma once

#include "imgui.h"
#include "glm/vec3.hpp"

#include <cstdint>
#include <memory>

// 前向声明
struct GLFWwindow;
class Input;

class Application
{
public:
    Application() = default;
    ~Application();

    // glfw窗口初始化 glad函数加载
    bool Init(const uint32_t& width, const uint32_t& height);

    // 帧循环
    void Run();

    static void set_shouldClose(bool flag) { s_shouldClose = flag; }

protected:
    virtual void OnInit() {}
    virtual void OnUpdate(float dt) {}
    virtual void OnResize(int width, int height) {}
    virtual void OnRender() {}
    virtual void OnImGui()
    {
        // 控件
        ImGui::Begin("Settings");
        ImGui::Text("hello world");
        ImGui::Button("test btn", ImVec2(40, 20));
        ImGui::ColorEdit3("clear color", reinterpret_cast<float*>(&m_clearColor));
        ImGui::End();
    }
    virtual void OnShutdown() {}

private:
    void registerCallback() const;
    // glfw窗口关闭
    void destroy();

    // glfw窗体事件回调->Application转发给Input
    /**
     * @param window 哪个窗体发生了变化
     * @param width 新的宽度
     * @param height 新的高度
     */
    static void framebufferSizeCallbackDispatch(GLFWwindow* window, int width, int height);
    // glfw键盘事件回调
    static void keyboardCallbackDispatch(GLFWwindow* window, int key, int scancode, int action, int mods);
    // glfw鼠标位置回调
    static void mousePosCallbackDispatch(GLFWwindow* window, double x, double y);
    /**
     * gflw鼠标滚轮缩放回调
     * @param yOffset 大于0表示向上滚动 小于0表示向下滚动
     */
    static void mouseScrollCallbackDispatch(GLFWwindow* window, double xOffset, double yOffset);
    // 鼠标点击事件
    static void mouseBtnCallbackDispatch(GLFWwindow* window, int button, int action, int mods);

protected:
    // 窗体宽度
    uint32_t m_Width{0};
    // 窗体高度
    uint32_t m_Height{0};
    // 窗体对象
    GLFWwindow* m_Window{nullptr};

    std::unique_ptr<Input> m_input;
    glm::vec3              m_clearColor{1.0f};

private:
    static bool s_shouldClose;
};
