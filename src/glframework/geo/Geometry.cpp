//
// Created by dingrui on 25-7-20.
//

#include "glframework/geo/Geometry.h"

#include <glm/glm.hpp>

Geometry::~Geometry() {
    if (m_VAO != 0) { glDeleteVertexArrays(1, &m_VAO); }
    if (m_VBO != 0) { glDeleteBuffers(1, &m_VBO); }
    if (m_EBO != 0) { glDeleteBuffers(1, &m_EBO); }
}

void Geometry::SetupBuffers(const void *vertices, size_t vertSz, VertexLayout layout, const uint32_t *indices,
                            size_t indexSz) {
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    glGenVertexArrays(1, &m_VAO);

    glBindVertexArray(m_VAO);

    // VBO
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertSz, vertices, GL_STATIC_DRAW);

    // EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSz, indices, GL_STATIC_DRAW);

    // calculate stride
    // xyz
    size_t stride = static_cast<uint32_t>(layout.posDim) * sizeof(float);
    // for uv, 颜色rgb, 法线normal, 切线tangent
    for (const auto &desc: kAttrTable) {
        if (Has(layout.attrs, desc.attr)) {
            stride += desc.components * sizeof(float);
        }
    }
    // setup vertex attributes
    size_t offset = 0;
    uint32_t index = 0;
    {
        // xyz
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index,
            static_cast<uint32_t>(layout.posDim),
            GL_FLOAT,
            GL_FALSE,
            stride,
            reinterpret_cast<void *>(offset)
        );
        offset += static_cast<uint32_t>(layout.posDim) * sizeof(float);
        index++;
    }
    // other attributes, uv 颜色 法线 切线
    for (const auto &desc: kAttrTable) {
        if (!Has(layout.attrs, desc.attr)) { continue; }
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index,
            desc.components,
            GL_FLOAT,
            GL_FALSE,
            stride,
            reinterpret_cast<void *>(offset)
        );
        offset += desc.components * sizeof(float);
        index++;
    }
    m_IndicesCnt = indexSz / sizeof(uint32_t);
    // 解绑VAO
    glBindVertexArray(0);
}
