//
// Created by dingrui on 25-7-19.
//

#include "application/camera/controller/GameCameraController.h"

#include <glm/gtx/transform.hpp>
#include <GLFW/glfw3.h>

GameCameraController::GameCameraController(Camera &camera) : CameraController(camera) {
}

void GameCameraController::OnCursor(double x, double y) {
    // 鼠标移动的距离
    float dx = (static_cast<float>(x) - m_CurX) * m_Sensitivity;
    float dy = (static_cast<float>(y) - m_CurY) * m_Sensitivity;
    if (m_RightButtonPressed) {
        // 相机前后转动
        pitch(-dy);
        // 相机左右转动
        yaw(-dx);
    }
    m_CurX = x;
    m_CurY = y;
}

void GameCameraController::OnUpdate() {
    // 最终移动方向
    glm::vec3 direction(0.0f);
    auto front = glm::cross(m_Camera.get_up(), m_Camera.get_right());
    auto right = m_Camera.get_right();
    if (m_KeyPressedMap[GLFW_KEY_W]) direction += front;
    if (m_KeyPressedMap[GLFW_KEY_S]) direction -= front;
    if (m_KeyPressedMap[GLFW_KEY_A]) direction -= right;
    if (m_KeyPressedMap[GLFW_KEY_D]) direction += right;
    // 此时direction可能不为1的长度 可能是0的长度
    if (glm::length(direction) != 0) {
        direction = glm::normalize(direction);
        m_Camera.set_position(m_Camera.get_position() + (direction * m_MovSpeed));
    }
}

void GameCameraController::pitch(float angle) {
    m_Pitch += angle;
    if (m_Pitch > 89.0f || m_Pitch < -89.0f) {
        m_Pitch -= angle;
        return;
    }
    // 在game情况下 pitch不会影响position
    // 相机前后转动就是绕着x轴转动
    auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), m_Camera.get_right());
    m_Camera.set_up(mat * glm::vec4(m_Camera.get_up(), 0.0f));
}

void GameCameraController::yaw(float angle) {
    // 相机左右转动就是绕着z轴转动
    auto mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    m_Camera.set_up(mat * glm::vec4(m_Camera.get_up(), 0.0f));
    m_Camera.set_right(mat * glm::vec4(m_Camera.get_right(), 0.0f));
}
