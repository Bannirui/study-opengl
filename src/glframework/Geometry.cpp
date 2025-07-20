//
// Created by dingrui on 25-7-20.
//

#include "glframework/Geometry.h"

Geometry::Geometry() {}
Geometry::~Geometry()
{
    if (m_VAO != 0) glDeleteVertexArrays(1, &m_VAO);
    if (m_VBO != 0) glDeleteBuffers(1, &m_VBO);
    if (m_EBO != 0) glDeleteBuffers(1, &m_EBO);
}
Geometry* Geometry::CreateBox(float length)
{
    Geometry* geometry     = new Geometry();
    geometry->m_IndicesCnt = 36;
    float half             = length / 2.0f;
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
    // VBO创建
    glGenBuffers(1, &geometry->m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, geometry->m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // EBO创建
    glGenBuffers(1, &geometry->m_EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->m_EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // VAO创建
    glGenVertexArrays(1, &geometry->m_VAO);
    glBindVertexArray(geometry->m_VAO);
    // VAO中加入VBO的属性信息
    glBindBuffer(GL_ARRAY_BUFFER, geometry->m_VBO);
    // 属性0 xyz坐标
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    // 属性1 uv坐标
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));
    // 加入ebo到当前的vao
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geometry->m_EBO);
    // 解绑VAO
    glBindVertexArray(0);
    return geometry;
}
