//
// Created by dingrui on 25-7-19.
//

#include "application/camera/PerspectiveCamera.h"

#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/ext/matrix_clip_space.hpp>

PerspectiveCamera::PerspectiveCamera(float aspect)
{
    m_Fovy   = c_FOVY;
    m_Aspect = aspect;
    m_Near   = c_NEAR;
    m_Far    = c_FAR;
}
PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float near, float far)
    : m_Fovy(fovy), m_Aspect(aspect)
{
    m_Near = near;
    m_Far = far;
}

glm::mat4 PerspectiveCamera::GetProjectionMatrix() const
{
    // 透视投影矩阵 摄影机空间->剪裁空间
    // fovy 在y轴方向的视张角
    // aspect 近平面的横纵百分比
    // near 近平面距离
    // far 远平面距离
    return glm::perspective(glm::radians(m_Fovy), m_Aspect, m_Near, m_Far);
}

void PerspectiveCamera::scale(float scale)
{
    auto front = glm::cross(m_Up, m_Right);
    m_Position += (front * scale);
}
