//
// Created by dingrui on 25-7-20.
//

#include "glframework/Geometry.h"

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
    // 法线
    if (layout & static_cast<uint32_t>(VertexAttr::Normal)) stride += 3 * sizeof(float);
    // uv
    if (layout & static_cast<uint32_t>(VertexAttr::TexCoord)) stride += 2 * sizeof(float);
    // 颜色
    if (layout & static_cast<uint32_t>(VertexAttr::Color)) stride += 3 * sizeof(float);
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
    // 法线
    if (layout & static_cast<uint32_t>(VertexAttr::Normal))
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
GeoSphere::GeoSphere(float radius)
{
    // xyz坐标
    std::vector<GLfloat> positions{};
    // uv坐标
    std::vector<GLfloat> uvs{};
    // 法线
    std::vector<GLfloat> normals{};
    // 切线
    std::vector<GLfloat> tangents{};
    // 索引
    std::vector<GLuint> indices{};

    // 纬线数量
    int numLatLines = 60;
    // 经线数量
    int numLongLines = 60;
    for (int i = 0; i <= numLatLines; i++)
    {
        for (int j = 0; j <= numLongLines; j++)
        {
            float phi   = i * glm::pi<float>() / numLatLines;
            float theta = j * 2 * glm::pi<float>() / numLongLines;
            // 定点坐标
            float y = radius * cos(phi);
            float x = radius * sin(phi) * cos(theta);
            float z = radius * sin(phi) * sin(theta);
            positions.push_back(x);
            positions.push_back(y);
            positions.push_back(z);
            // uv坐标
            float u = 1.0 - (float)j / (float)numLongLines;
            float v = 1.0 - (float)i / (float)numLatLines;
            uvs.push_back(u);
            uvs.push_back(v);
            // 法线方向没有问题 法线的长度不为1
            normals.push_back(x);
            normals.push_back(y);
            normals.push_back(z);
        }
    }

    // 没有=号 顶点索引
    for (int i = 0; i < numLatLines; i++)
    {
        for (int j = 0; j < numLongLines; j++)
        {
            int p1 = i * (numLongLines + 1) + j;
            int p2 = p1 + numLongLines + 1;
            int p3 = p1 + 1;
            int p4 = p2 + 1;
            // 索引
            indices.push_back(p1);
            indices.push_back(p2);
            indices.push_back(p3);

            indices.push_back(p3);
            indices.push_back(p2);
            indices.push_back(p4);
        }
    }
    // 切线
    tangents.resize(positions.size());
    // 以三角形为单位进行indices的遍历
    for (long unsigned int i = 0; i < indices.size(); i += 3)
    {
        // 取出当前三角形的三个顶点的索引
        int idx0 = indices[i];
        int idx1 = indices[i + 1];
        int idx2 = indices[i + 2];
        // 根据三个顶点的索引 从positions数组中找到三个顶点的位置信息
        auto p0 = glm::vec3(positions[idx0 * 3], positions[idx0 * 3 + 1], positions[idx0 * 3 + 2]);
        auto p1 = glm::vec3(positions[idx1 * 3], positions[idx1 * 3 + 1], positions[idx1 * 3 + 2]);
        auto p2 = glm::vec3(positions[idx2 * 3], positions[idx2 * 3 + 1], positions[idx2 * 3 + 2]);
        // 根据三个顶点的索引 从uvs数组中找到三个顶点的uv信息
        auto uv0 = glm::vec2(uvs[idx0 * 2], uvs[idx0 * 2 + 1]);
        auto uv1 = glm::vec2(uvs[idx1 * 2], uvs[idx1 * 2 + 1]);
        auto uv2 = glm::vec2(uvs[idx2 * 2], uvs[idx2 * 2 + 1]);
        // 计算当前三角形的切线
        glm::vec3 e0 = p1 - p0;
        glm::vec3 e1 = p2 - p1;

        glm::vec2 duv0 = uv1 - uv0;
        glm::vec2 duv1 = uv2 - uv1;

        float f = 1.0f / (duv0.x * duv1.y - duv1.x * duv0.y);

        glm::vec3 tangent;
        tangent.x = f * (duv1.y * e0.x - duv0.y * e1.x);
        tangent.y = f * (duv1.y * e0.y - duv0.y * e1.y);
        tangent.z = f * (duv1.y * e0.z - duv0.y * e1.z);
        tangent   = glm::normalize(tangent);

        // 5 针对本三角形的三个顶点的normal，使tangent正交化(三个不同的tangent）
        auto normal0 = glm::normalize(glm::vec3(normals[idx0 * 3], normals[idx0 * 3 + 1], normals[idx0 * 3 + 2]));
        auto normal1 = glm::normalize(glm::vec3(normals[idx1 * 3], normals[idx1 * 3 + 1], normals[idx1 * 3 + 2]));
        auto normal2 = glm::normalize(glm::vec3(normals[idx2 * 3], normals[idx2 * 3 + 1], normals[idx2 * 3 + 2]));

        auto tangent0 = tangent - glm::dot(tangent, normal0) * normal0;
        auto tangent1 = tangent - glm::dot(tangent, normal1) * normal1;
        auto tangent2 = tangent - glm::dot(tangent, normal2) * normal2;

        // 6 累加到每个顶点的tangent属性上
        tangents[idx0 * 3] += tangent0.x;
        tangents[idx0 * 3 + 1] += tangent0.y;
        tangents[idx0 * 3 + 2] += tangent0.z;

        tangents[idx1 * 3] += tangent1.x;
        tangents[idx1 * 3 + 1] += tangent1.y;
        tangents[idx1 * 3 + 2] += tangent1.z;

        tangents[idx2 * 3] += tangent2.x;
        tangents[idx2 * 3 + 1] += tangent2.y;
        tangents[idx2 * 3 + 2] += tangent2.z;
    }
    // 对每个顶点的最终tangent累加值进行normalize
    for (long unsigned int i = 0; i < tangents.size(); i += 3)
    {
        glm::vec3 tangent = glm::vec3(tangents[i], tangents[i + 1], tangents[i + 2]);
        tangent           = glm::normalize(tangent);
        tangents[i]       = tangent.x;
        tangents[i + 1]   = tangent.y;
        tangents[i + 2]   = tangent.z;
    }
    // 合并定点属性
    std::vector<GLfloat> vertices;
    size_t               vertexCount = positions.size() / 3;
    for (size_t i = 0; i < vertexCount; ++i)
    {
        // position
        vertices.push_back(positions[i * 3 + 0]);
        vertices.push_back(positions[i * 3 + 1]);
        vertices.push_back(positions[i * 3 + 2]);
        // uv
        vertices.push_back(uvs[i * 2 + 0]);
        vertices.push_back(uvs[i * 2 + 1]);
        // normal
        vertices.push_back(normals[i * 3 + 0]);
        vertices.push_back(normals[i * 3 + 1]);
        vertices.push_back(normals[i * 3 + 2]);
        // tangent
        vertices.push_back(tangents[i * 3 + 0]);
        vertices.push_back(tangents[i * 3 + 1]);
        vertices.push_back(tangents[i * 3 + 2]);
    }
    // 数据灌到gl状态机
    setupBuffers(vertices.data(), sizeof(GLfloat) * vertices.size(),
                 static_cast<VertexLayout>(VertexAttr::Position | VertexAttr::TexCoord | VertexAttr::Normal |
                                           VertexAttr ::TexCoord | VertexAttr::Tangent),
                 indices.data(), sizeof(GLuint) * indices.size());
}
