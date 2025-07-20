//
// Created by rui ding on 2025/7/1.
//

#include "../../../include/application/camera/Camera.h"

#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>

Camera::Camera() {}
Camera::~Camera() {}

glm::mat4 Camera::GetViewMatrix()
{
    // 生成一个viewMatrix
    // eye 摄像机所在的位置
    // center 当前摄像机看向的那个点
    // up 穹顶向量
    auto front  = glm::cross(m_Up, m_Right);
    auto center = m_Position + front;
    return glm::lookAt(m_Position, center, m_Up);
}