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
    // 怎么计算这个值=目标点-光源起点
    // 比如想要的效果是光从右后方照 目标点=(0,0,0) 右后方=(1,1,1) direction=(0,0,0)-(1,1,1)=(-1,-1,-1)
    glm::vec3 m_direction{-1.0f};
};
