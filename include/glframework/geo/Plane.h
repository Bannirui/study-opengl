//
// Created by dingrui on 25-7-21.
//

#pragma once

#include "glframework/geo/Geometry.h"

// 平面
class Plane : public Geometry
{
public:
    /**
     * @param width 宽度
     * @param height 高度
     */
    explicit Plane(float width, float height);
};
