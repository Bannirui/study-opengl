//
// Created by dingrui on 25-7-19.
//

#include "application/TrackballCameraController.h"

#include <glm/gtx/transform.hpp>

TrackballCameraController::TrackballCameraController(Camera* camera) : CameraController(camera) {}
void TrackballCameraController::OnCursor(double x, double y)
{
    if (m_LeftButtonPressed)
    {
        float dx = (x - m_CurX) * m_Sensitivity;
        float dy = (y - m_CurY) * m_Sensitivity;
        pitch(-dy);
        yaw(-dx);
    }
    else if (m_MidButtonPressed)
    {
        float dx = (x - m_CurX) * m_Sensitivity;
        float dy = (y - m_CurY) * m_Sensitivity;
        m_CameraPtr->m_Position += m_CameraPtr->m_Up * dy;
        m_CameraPtr->m_Position -= m_CameraPtr->m_Right * dx;
    }
    m_CurX = x;
    m_CurY = y;
}
void TrackballCameraController::OnScroll(float yOffset)
{
    m_CameraPtr->scale(m_ScaleSpeed * yOffset);
}
void TrackballCameraController::pitch(float angle)
{
    // 绕着right向量旋转
    auto mat                = glm::rotate(glm::mat4(1.0f), glm::radians(angle), m_CameraPtr->m_Right);
    m_CameraPtr->m_Up       = mat * glm::vec4(m_CameraPtr->m_Up, 0.0f);
    m_CameraPtr->m_Position = mat * glm::vec4(m_CameraPtr->m_Position, 0.0f);
}
void TrackballCameraController::yaw(float angle)
{
    // 绕着世界坐标系y轴旋转
    auto mat                = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
    m_CameraPtr->m_Up       = mat * glm::vec4(m_CameraPtr->m_Up, 0.0f);
    m_CameraPtr->m_Right    = mat * glm::vec4(m_CameraPtr->m_Right, 0.0f);
    m_CameraPtr->m_Position = mat * glm::vec4(m_CameraPtr->m_Position, 1.0f);
}
