//
// Created by dingrui on 25-7-19.
//

#include "application/camera/controller/TrackballCameraController.h"

#include <glm/gtx/transform.hpp>

TrackballCameraController::TrackballCameraController(Camera &camera) : CameraController(camera) {
}

void TrackballCameraController::OnCursor(double x, double y) {
    if (m_LeftButtonPressed) {
        float dx = (x - m_CurX) * m_Sensitivity;
        float dy = (y - m_CurY) * m_Sensitivity;
        pitch(-dy);
        yaw(-dx);
    } else if (m_MidButtonPressed) {
        float dx = (x - m_CurX) * m_MovSpeed;
        float dy = (y - m_CurY) * m_MovSpeed;
        m_Camera.set_position(m_Camera.get_position() + m_Camera.get_up() * dy);
        m_Camera.set_position(m_Camera.get_position() - m_Camera.get_right() * dx);
    }
    m_CurX = x;
    m_CurY = y;
}

void TrackballCameraController::OnScroll(float yOffset) {
    m_Camera.scale(m_ScaleSpeed * yOffset);
}

void TrackballCameraController::pitch(float angle) {
    // 相机前后转动绕着right向量 x轴旋转
    auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), m_Camera.get_right());
    m_Camera.set_up(mat * glm::vec4(m_Camera.get_up(), 0.0f));
    m_Camera.set_position(mat * glm::vec4(m_Camera.get_position(), 0.0f));
}

void TrackballCameraController::yaw(float angle) {
    // 相机左右转动绕着世界坐标系y轴旋转
    auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    m_Camera.set_up(mat * glm::vec4(m_Camera.get_up(), 0.0f));
    m_Camera.set_right(mat * glm::vec4(m_Camera.get_right(), 0.0f));
    m_Camera.set_position(mat * glm::vec4(m_Camera.get_position(), 1.0f));
}
