//
// Created by dingrui on 25-7-23.
//

#pragma once

#include "glframework/Core.h"

class Light
{
public:
    Light()          = default;
    virtual ~Light() = default;

public:
    // 光的颜色强度
    glm::vec3 m_color{1.0f};
    // 镜面反射的强度
    float m_specularIntensity{1.0f};
};
