//
// Created by dingrui on 25-7-21.
//

#include "glframework/geo/ScreenPlane.h"

ScreenPlane::ScreenPlane() {
    // xyz, no z
    // no normal
    // clang-format off
    float vertices[] = {
         // x   y      u      v
        -1.0f,  1.0f,  0.0f,  1.0f,
        -1.0f, -1.0f,  0.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  1.0f,
    };
    uint32_t indices[] = {
        0, 1, 2,
        0, 2, 3,
    };
    // clang-format on
    VertexLayout layout{};
    layout.posDim = VertexPosDim::k2D;
    layout.attrs = VertexAttr::kTexCoord;
    Geometry::SetupBuffers(vertices, sizeof(vertices), layout, indices, sizeof(indices));
}
