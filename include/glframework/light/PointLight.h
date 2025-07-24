//
// Created by dingrui on 25-7-25.
//

#pragma once
#include "Light.h"
#include "glframework/Object.h"

// 点光源
class PointLight : public Light, public Object
{
public:
    PointLight();
    ~PointLight();

public:
    // 控制点光源的衰减
    // 2次项系数
    float m_k2 = 1.0f;
    // 1次项系数
    float m_k1 = 1.0f;
    // 常数项系数
    float m_kc = 1.0f;
};
