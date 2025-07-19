//
// Created by dingrui on 25-7-19.
//

#pragma once

#include "application/Camera.h"

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(float fovy, float aspect, float near, float far);
    ~PerspectiveCamera() = default;

    glm::mat4 GetProjection() override;

private:
    // y方向视张角 角度 glm使用的时候转换为弧度t commit工具栏
    float m_Fovy = 0.0f;
    // 近平面纵横百分比
    float m_Aspect = 0.0f;
    // 近平面距离
    float m_Near = 0.0f;
    // 远平面距离
    float m_Far = 0.0f;
};
