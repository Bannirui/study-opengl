//
// Created by dingrui on 25-7-21.
//
#include "glframework/geo/Box.h"

Box::Box(float sz)
{
    float half = sz / 2.0f;
    // clang-format off
    float vertices[] = {
        // xyz                uv            法线
        // Front face
        -half, -half,  half,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         half, -half,  half,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         half,  half,  half,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        -half,  half,  half,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        // Back face
        -half, -half, -half,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f,
        -half,  half, -half,  1.0f,  0.0f,  0.0f,  0.0f, -1.0f,
         half,  half, -half,  1.0f,  1.0f,  0.0f,  0.0f, -1.0f,
         half, -half, -half,  0.0f,  1.0f,  0.0f,  0.0f, -1.0f,
        // Top face
        -half,  half,  half,  0.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         half,  half,  half,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
         half,  half, -half,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        -half,  half, -half,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
        // Bottom face
        -half, -half, -half,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f,
         half, -half, -half,  1.0f,  0.0f,  0.0f, -1.0f,  0.0f,
         half, -half,  half,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f,
        -half, -half,  half,  0.0f,  1.0f,  0.0f, -1.0f,  0.0f,
        // Right face
         half, -half,  half,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         half, -half, -half,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
         half,  half, -half,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
         half,  half,  half,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,
        // Left face
        -half, -half, -half,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        -half, -half,  half,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
        -half,  half,  half,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
        -half,  half, -half,  0.0f,  1.0f, -1.0f,  0.0f,  0.0f,
    };
    uint32_t indices[] = {
        0,  1,  2,  2,  3,  0,   // Front face
        4,  5,  6,  6,  7,  4,   // Back face
        8,  9,  10, 10, 11, 8,   // Top face
        12, 13, 14, 14, 15, 12,  // Bottom face
        16, 17, 18, 18, 19, 16,  // Right face
        20, 21, 22, 22, 23, 20   // Left face
    };
    // clang-format on
    setupBuffers(vertices, sizeof(vertices),
                 static_cast<VertexLayout>(VertexAttr::Position | VertexAttr::TexCoord | VertexAttr::Normal), indices,
                 sizeof(indices));
}
