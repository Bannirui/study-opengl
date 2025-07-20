//
// Created by dingrui on 25-7-20.
//

#include "glframework/Geometry.h"

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
    if (layout & static_cast<uint32_t>(VertexAttr::Position)) stride += 3 * sizeof(float);
    if (layout & static_cast<uint32_t>(VertexAttr::Normal)) stride += 3 * sizeof(float);
    if (layout & static_cast<uint32_t>(VertexAttr::TexCoord)) stride += 2 * sizeof(float);
    if (layout & static_cast<uint32_t>(VertexAttr::Color)) stride += 3 * sizeof(float);
    size_t offset = 0;
    GLuint index  = 0;
    if (layout & static_cast<uint32_t>(VertexAttr::Position))
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        offset += 3 * sizeof(float);
        index++;
    }
    if (layout & static_cast<uint32_t>(VertexAttr::Normal))
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        offset += 3 * sizeof(float);
        index++;
    }
    if (layout & static_cast<uint32_t>(VertexAttr::Position))
    {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, stride, (void*)offset);
        offset += 2 * sizeof(float);
        index++;
    }
    if (layout & static_cast<uint32_t>(VertexAttr::Position))
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

GeoBox::GeoBox(float sz)
{
    float half = sz / 2.0f;
    // clang-format off
    float vertices[] = {
        // xyz                uv
        // Front face
        -half, -half,  half,  0.0f,  0.0f,
         half, -half,  half,  1.0f,  0.0f,
         half,  half,  half,  1.0f,  1.0f,
        -half,  half,  half,  0.0f,  1.0f,
        // Back face
        -half, -half, -half,  0.0f,  0.0f,
        -half,  half, -half,  1.0f,  0.0f,
         half,  half, -half,  1.0f,  1.0f,
         half, -half, -half,  0.0f,  1.0f,
        // Top face
        -half,  half,  half,  0.0f,  0.0f,
         half,  half,  half,  1.0f,  0.0f,
         half,  half, -half,  1.0f,  1.0f,
        -half,  half, -half,  0.0f,  1.0f,
        // Bottom face
        -half, -half, -half,  0.0f,  0.0f,
         half, -half, -half,  1.0f,  0.0f,
         half, -half,  half,  1.0f,  1.0f,
        -half, -half,  half,  0.0f,  1.0f,
        // Right face
         half, -half,  half,  0.0f,  0.0f,
         half, -half, -half,  1.0f,  0.0f,
         half,  half, -half,  1.0f,  1.0f,
         half,  half,  half,  0.0f,  1.0f,
        // Left face
        -half, -half, -half,  0.0f,  0.0f,
        -half, -half,  half,  1.0f,  0.0f,
        -half,  half,  half,  1.0f,  1.0f,
        -half,  half, -half,  0.0f,  1.0f,
    };
    unsigned int indices[] = {
        0,  1,  2,  2,  3,  0,   // Front face
        4,  5,  6,  6,  7,  4,   // Back face
        8,  9,  10, 10, 11, 8,   // Top face
        12, 13, 14, 14, 15, 12,  // Bottom face
        16, 17, 18, 18, 19, 16,  // Right face
        20, 21, 22, 22, 23, 20   // Left face
    };
    // clang-format on
    setupBuffers(vertices, sizeof(vertices), static_cast<VertexLayout>(VertexAttr::Position | VertexAttr::TexCoord),
                 indices, sizeof(indices));
}
