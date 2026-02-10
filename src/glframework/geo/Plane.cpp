//
// Created by dingrui on 25-7-21.
//

#include "glframework/geo/Plane.h"

Plane::Plane(float width, float height) {
    float halfW = width / 2.0f;
    float halfH = height / 2.0f;
    // clang-format off
    float vertices[] = {
         // x    y       z      u      v      法线
        -halfW, -halfH,  0.0f,  0.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         halfW, -halfH,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
         halfW,  halfH,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
        -halfW,  halfH,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    };
    uint32_t indices[] = {
        0, 1, 2,
        2, 3, 0,
    };
    // clang-format on
    VertexLayout layout{};
    layout.posDim = VertexPosDim::k3D;
    layout.attrs = VertexAttr::kTexCoord | VertexAttr::kNormal;
    Geometry::SetupBuffers(vertices, sizeof(vertices), layout, indices, sizeof(indices));
}
