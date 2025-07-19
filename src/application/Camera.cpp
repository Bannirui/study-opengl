//
// Created by rui ding on 2025/7/1.
//

#include "application/Camera.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM)
{
    m_Position = position;
    WorldUp    = up;
    Yaw        = yaw;
    Pitch      = pitch;
    updateCameraVectors();
}

Camera::Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch)
    : m_Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), m_Zoom(ZOOM)
{
    m_Position = glm::vec3(posX, posY, posZ);
    WorldUp    = glm::vec3(upX, upY, upZ);
    Yaw        = yaw;
    Pitch      = pitch;
    updateCameraVectors();
}

Camera::~Camera() {}

glm::mat4 Camera::GetViewMatrix()
{
    // 生成一个viewMatrix
    // eye 摄像机所在的位置
    // center 当前摄像机看向的那个点
    // up 穹顶向量
    return glm::lookAt(m_Position, m_Position + m_Front, m_Up);
}

void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
    float velocity = MovementSpeed * deltaTime;
    if (direction == FORWARD) m_Position += m_Front * velocity;
    if (direction == BACKWARD) m_Position -= m_Front * velocity;
    if (direction == LEFT) m_Position -= m_Right * velocity;
    if (direction == RIGHT) m_Position += m_Right * velocity;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch)
{
    xOffset *= MouseSensitivity;
    yOffset *= MouseSensitivity;
    Yaw += xOffset;
    Pitch += yOffset;
    if (constrainPitch)
    {
        if (Pitch > 89.0f) Pitch = 89.0f;
        if (Pitch < -89.0f) Pitch = -89.0f;
    }
    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset)
{
    m_Zoom -= (float)yOffset;
    if (m_Zoom < 1.0f) m_Zoom = 1.0f;
    if (m_Zoom > 45.0f) m_Zoom = 45.0f;
}

void Camera::updateCameraVectors()
{
    glm::vec3 front;
    front.x = glm::cos(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));
    front.y = glm::sin(glm::radians(Pitch));
    front.z = glm::sin(glm::radians(Yaw)) * glm::cos(glm::radians(Pitch));
    m_Front = glm::normalize(front);
    m_Right = glm::normalize(glm::cross(m_Front, WorldUp));
    m_Up    = glm::normalize(glm::cross(m_Right, m_Front));
}
