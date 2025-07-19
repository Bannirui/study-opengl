//
// Created by dingrui on 25-7-19.
//

#include "application/OrthographicCamera.h"

#include <glm/ext/matrix_clip_space.hpp>

OrthographicCamera::OrthographicCamera(float l, float r, float t, float b, float n, float f)
    : m_Left(l), m_Right(r), m_Top(t), m_Bottom(b), m_Near(n), m_Far(f)
{
}
glm::mat4 OrthographicCamera::GetProjection()
{
    return glm::ortho(m_Left, m_Right, m_Bottom, m_Top, m_Near, m_Far);
}
