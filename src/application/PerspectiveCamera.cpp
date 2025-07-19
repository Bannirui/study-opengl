//
// Created by dingrui on 25-7-19.
//

#include "application/PerspectiveCamera.h"

#include <glm/ext/matrix_clip_space.hpp>

PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float near, float far)
    : m_Fovy(fovy), m_Aspect(aspect), m_Near(near), m_Far(far)
{
}

glm::mat4 PerspectiveCamera::GetProjection()
{
    return glm::perspective(glm::radians(m_Fovy), m_Aspect, m_Near, m_Far);
}
