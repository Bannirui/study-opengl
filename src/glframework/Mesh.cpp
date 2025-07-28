//
// Created by dingrui on 25-7-23.
//

#include "glframework/Mesh.h"

Mesh::Mesh(Geometry* geometry, Material* material)
{
    m_type     = ObjectType::Mesh;
    m_geometry = geometry;
    m_material = material;
}
Mesh::~Mesh() {}
