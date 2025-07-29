//
// Created by dingrui on 25-7-30.
//

#pragma once

#include "glframework/geo/Geometry.h"

#include <vector>

// 几何体由模型数据创建
class Model : public Geometry
{
public:
    explicit Model(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, VertexLayout layout);
};
