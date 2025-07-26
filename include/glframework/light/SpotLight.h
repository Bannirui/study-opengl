//
// Created by dingrui on 25-7-26.
//

#pragma once
#include "Light.h"
#include "glframework/Object.h"

// 聚光灯
class SpotLight : public Light, public Object
{
public:
    SpotLight();
    ~SpotLight();

public:
    // 探照灯看向的方向
    glm::vec3 m_targetDirection{-1.0f};
    // 探照灯照射覆盖的角度
    float m_spotAngle = 45.0f;
};
