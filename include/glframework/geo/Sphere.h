//
// Created by dingrui on 25-7-21.
//

#pragma once

#include "glframework/geo/Geometry.h"

// 球体
class Sphere : public Geometry
{
public:
    /**
     * @param radius 半径
     */
    explicit Sphere(float radius = 1.0f);
};
