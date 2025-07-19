//
// Created by dingrui on 25-7-19.
//

#include "application/PerspectiveCamera.h"

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
    : m_Fovy(fovy), m_Aspect(aspect), m_Near(near), m_Far(far)
{
}

glm::mat4 PerspectiveCamera::GetProjectionMatrix()
{
    return glm::perspective(glm::radians(m_Fovy), m_Aspect, m_Near, m_Far);
}

void PerspectiveCamera::scale(float scale)
{
    m_Position += m_Front * scale;
}
