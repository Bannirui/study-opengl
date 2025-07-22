//
// Created by dingrui on 25-7-23.
//

#pragma once

#include "glframework/Object.h"

class Geometry;
class Material;

/**
 * 网格体
 */
class Mesh : public Object
{
public:
    Mesh(Geometry* geometry, Material* material);
    ~Mesh() = default;

public:
    Geometry* m_geometry{nullptr};
    Material* m_material{nullptr};
};
