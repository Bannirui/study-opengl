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
    ~SpotLight() override = default;

public:
    // 探照灯看向的方向
    glm::vec3 m_targetDirection;
    // 探照灯照射覆盖的角度 如果只设置一个角度 会导致边缘太锐化 因此设置两个角度 负责可视和不可视之间过渡
    float m_innerAngle;
    float m_outerAngle;
};
