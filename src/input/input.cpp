//
// Created by dingrui on 2/7/26.
//

#include "input/input.h"

#include "err_check.h"
#include "x_log.h"
#include "application/Application.h"
#include "application/camera/CameraController.h"

void Input::OnResize(int w, int h) const
{
    XLOG_INFO("frame resize, w:{} h:{}", w, h);
    // 视口 设置窗口中opengl负责渲染的区域
    // x y将相对窗口左下角的起始位置
    // width height渲染区域的长度 高度
    GL_CALL_AND_CHECK_ERR(glViewport(0, 0, w, h));
}

void Input::OnKey(int key, int scancode, int action, int mods) const
{
    XLOG_INFO("keyboard pressed, key:{} action:{} 有没有ctrl/shift功能键:{}", static_cast<char>(key), action, mods);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        Application::set_shouldClose(true);
    }
    if (m_cameraCtl)
    {
        m_cameraCtl->OnKey(key, action, mods);
    }
}

void Input::OnCursor(double x, double y) const
{
    XLOG_INFO("mouse pos, x:{} y:{}", x, y);
    if (m_cameraCtl)
    {
        m_cameraCtl->OnCursor(x, y);
    }
}

void Input::OnScroll(double xOffset, double yOffset) const
{
    XLOG_INFO("mouse scroll, x_offset:{}, y_offset:{}", xOffset, yOffset);
    if (m_cameraCtl)
    {
        m_cameraCtl->OnScroll(yOffset);
    }
}

void Input::OnMouse(int button, int action, int mods, double curX, double curY) const
{
    XLOG_INFO("mouse pressed, button:{} action:{} mods:{}", button, action, mods);
    if (m_cameraCtl)
    {
        m_cameraCtl->OnMouse(button, action, mods, curX, curY);
    }
}