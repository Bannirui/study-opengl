//
// Created by dingrui on 25-7-30.
//

#include "glframework/geo/Model.h"
Model::Model(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, VertexLayout layout)
{
    SetupBuffers(vertices.data(), sizeof(float) * vertices.size(), layout, indices.data(),
                 sizeof(uint32_t) * indices.size());
}
