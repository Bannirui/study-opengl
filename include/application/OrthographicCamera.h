//
// Created by dingrui on 25-7-19.
//

#pragma once

#include "application/Camera.h"

class OrthographicCamera : public Camera
{
public:
    OrthographicCamera(float l, float r, float t, float b, float n, float f);
    ~OrthographicCamera() = default;

    glm::mat4 GetProjection() override;

private:
    // 正交盒子
    float m_Left{0.0f};
    float m_Right{0.0f};
    float m_Top{0.0f};
    float m_Bottom{0.0f};
    float m_Near{0.0f};
    float m_Far{0.0f};
};
