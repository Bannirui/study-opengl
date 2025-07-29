//
// Created by dingrui on 25-7-30.
//

#include "glframework/geo/Model.h"
Model::Model(const std::vector<float>& vertices, const std::vector<unsigned int>& indices, VertexLayout layout)
{
    setupBuffers(vertices.data(), sizeof(GLfloat) * vertices.size(), layout, indices.data(),
                 sizeof(GLuint) * indices.size());
}
