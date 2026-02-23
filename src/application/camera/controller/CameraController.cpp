//
// Created by dingrui on 25-7-19.
//

#include "application/camera/controller/CameraController.h"

#include "x_log.h"

#include <GLFW/glfw3.h>

CameraController::CameraController(Camera& camera) : m_Camera(camera) {}

void CameraController::OnMouse(int button, int action, int mods, double x, double y)
{
    // 鼠标是被按下还是抬起
    bool pressed = (action == GLFW_PRESS);
    if (pressed)
    {
        m_CurX = static_cast<float>(x);
        m_CurY = static_cast<float>(y);
    }
    // 哪个按键
    switch (button)
    {
        case GLFW_MOUSE_BUTTON_LEFT:
            m_LeftButtonPressed = pressed;
            break;
        case GLFW_MOUSE_BUTTON_MIDDLE:
            m_MidButtonPressed = pressed;
            break;
        case GLFW_MOUSE_BUTTON_RIGHT:
            m_RightButtonPressed = pressed;
            break;
    }
}

void CameraController::OnScroll(float yOffset) {}

void CameraController::OnCursor(double x, double y) {}

void CameraController::OnKey(int key, int action, int mods)
{
    // 过滤repeat情况
    if (action == GLFW_REPEAT) return;
    m_KeyPressedMap[key] = (action == GLFW_PRESS);
}

void CameraController::OnUpdate() {}