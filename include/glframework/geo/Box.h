//
// Created by dingrui on 25-7-21.
//

#pragma once

#include "glframework/geo/Geometry.h"

// 正方体
class Box : public Geometry
{
public:
    /**
     * @param sz 正方体边长
     */
    explicit Box(float sz = 1.0f);
};
