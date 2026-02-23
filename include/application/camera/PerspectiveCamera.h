//
// Created by dingrui on 25-7-19.
//

#pragma once

#include "Camera.h"

// 默认视角
const float c_FOVY = 45.0f;
// 默认近平面
const float c_NEAR = 0.1f;
// 默认远平面
const float c_FAR = 1000.0f;

class PerspectiveCamera : public Camera
{
public:
    PerspectiveCamera(float aspect);
    PerspectiveCamera(float fovy, float aspect, float near, float far);

    glm::mat4 GetProjectionMatrix() const override;
    void      scale(float scale) override;

private:
    // y方向视张角 角度 glm使用的时候转换为弧度t commit工具栏
    float m_Fovy = 0.0f;
    // 近平面纵横百分比
    float m_Aspect = 0.0f;
};
