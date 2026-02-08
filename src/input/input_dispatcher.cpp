//
// Created by dingrui on 2/7/26.
//

#include "input/input_dispatcher.h"

#include "err_check.h"
#include "x_log.h"
#include "application/Application.h"
#include "application/camera/CameraController.h"

InputDispatcher *InputDispatcher::s_instance = nullptr;

InputDispatcher::InputDispatcher(Application *app)
    : m_app(app) {
    s_instance = this;
}

void InputDispatcher::OnResize(int w, int h) {
    s_instance->resize(w, h);
}

void InputDispatcher::OnKey(int key, int scancode, int action, int mods) {
    s_instance->key(key, action, mods);
}

void InputDispatcher::OnCursor(double x, double y) {
    s_instance->cursor(x, y);
}

void InputDispatcher::OnScroll(double xOffset, double yOffset) {
    s_instance->scroll(xOffset, yOffset);
}

void InputDispatcher::OnMouse(int button, int action, int mods) {
    s_instance->mouse(button, action, mods);
}

void InputDispatcher::resize(int w, int h) {
    XLOG_INFO("frame resize, w:{} h:{}", w, h);
    // 视口 设置窗口中opengl负责渲染的区域
    // x y将相对窗口左下角的起始位置
    // width height渲染区域的长度 高度
    GL_CALL_AND_CHECK_ERR(glViewport(0, 0, w, h));
}

void InputDispatcher::key(int key, int action, int mods) {
    XLOG_INFO("keyboard pressed, key:{} action:{} 有没有ctrl/shift功能键:{}", static_cast<char>(key), action, mods);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        Application::setShouldClose(true);
    }
    if (m_cameraCtl) {
        m_cameraCtl->OnKey(key, action, mods);
    }
}

void InputDispatcher::cursor(double x, double y) {
    XLOG_INFO("mouse pos, x:{} y:{}", x, y);
    if (m_cameraCtl) {
        m_cameraCtl->OnCursor(x, y);
    }
}

void InputDispatcher::scroll(double x, double y) {
    XLOG_INFO("mouse scroll, x_offset:{}, y_offset:{}", x, y);
    if (m_cameraCtl) {
        m_cameraCtl->OnScroll(y);
    }
}

void InputDispatcher::mouse(int button, int action, int mods) {
    XLOG_INFO("mouse pressed, button:{} action:{} mods:{}", button, action, mods);
    double x, y;
    glApp->GetMousePos(&x, &y);
    if (m_cameraCtl) {
        m_cameraCtl->OnMouse(button, action, mods, x, y);
    }
}
