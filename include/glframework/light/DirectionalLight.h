//
// Created by dingrui on 25-7-23.
//

#pragma once
#include "glframework/light/Light.h"

// 平行光
class DirectionalLight : public Light
{
public:
    DirectionalLight()           = default;

public:
    // 光照向的方向
    glm::vec3 m_direction{-1.0f};
};
