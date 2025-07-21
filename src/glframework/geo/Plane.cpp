//
// Created by dingrui on 25-7-21.
//

#include "glframework/geo/Plane.h"

Plane::Plane(float width, float height)
{
    float halfW = width / 2.0f;
    float halfH = height / 2.0f;
    // clang-format off
    float vertices[] = {
         // xyz                 uv
        -halfW, -halfH,  0.0f,  0.0f,  0.0f,
         halfW, -halfH,  0.0f,  1.0f,  0.0f,
         halfW,  halfH,  0.0f,  1.0f,  1.0f,
        -halfW,  halfH,  0.0f,  0.0f,  1.0f,
    };
    uint32_t indices[] = {
        0, 1, 2,
        2, 3, 0,
    };
    // clang-format on
    setupBuffers(vertices, sizeof(vertices), static_cast<VertexLayout>(VertexAttr::Position | VertexAttr::TexCoord), indices, sizeof(indices));
}
