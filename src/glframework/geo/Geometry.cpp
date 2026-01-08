//
// Created by dingrui on 25-7-20.
//

#include "../../../include/glframework/geo/Geometry.h"

#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>
#include <vector>

Geometry::~Geometry()
{
    if (m_VAO != 0) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO != 0) glDeleteBuffers(1, &m_VBO);
    if (m_EBO != 0) glDeleteBuffers(1, &m_EBO);
}

void Geometry::setupBuffers(const void* vertices, size_t vertSz, VertexLayout layout, const uint32_t* indices,
                            size_t indexSz)
{
    glGenBuffers(1, &m_VBO);
    glGenBuffers(1, &m_EBO);
    glGenVertexArrays(1, &m_VAO);

    glBindVertexArray(m_VAO);

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertSz, vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexSz, indices, GL_STATIC_DRAW);

    size_t stride = 0;
    // xyz
    if (layout & static_cast<uint32_t>(VertexAttr::Position)) stride += 3 * sizeof(float);
    // uv
    if (layout & static_cast<uint32_t>(VertexAttr::TexCoord)) stride += 2 * sizeof(float);
    // 颜色
    if (layout & static_cast<uint32_t>(VertexAttr::Color)) stride += 3 * sizeof(float);
    // 法线
    if (layout & static_cast<uint32_t>(VertexAttr::Normal)) stride += 3 * sizeof(float);
    // 切线
    if (layout & static_cast<uint32_t>(VertexAttr::Tangent)) stride += 3 * sizeof(float);
    size_t offset = 0;
    GLuint index  = 0;
    // xyz
    if (layout & static_cast<uint32_t>(VertexAttr::Position))
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        offset += 3 * sizeof(float);
        index++;
    }
    // uv
    if (layout & static_cast<uint32_t>(VertexAttr::TexCoord))
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        offset += 2 * sizeof(float);
        index++;
    }
    // 颜色
    if (layout & static_cast<uint32_t>(VertexAttr::Color))
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        offset += 3 * sizeof(float);
        index++;
    }
    // 法线
    if (layout & static_cast<uint32_t>(VertexAttr::Normal))
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        offset += 3 * sizeof(float);
        index++;
    }
    // 切线
    if (layout & static_cast<uint32_t>(VertexAttr::Tangent))
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        offset += 3 * sizeof(float);
        index++;
    }
    m_IndicesCnt = indexSz / sizeof(unsigned int);
    // 解绑VAO
    glBindVertexArray(0);
}