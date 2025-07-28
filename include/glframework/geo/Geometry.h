//
// Created by dingrui on 25-7-20.
//

#pragma once
#include <glad/glad.h>
#include <cstddef>
#include <vector>

enum class VertexAttr
{
    // xyz坐标
    Position = 1 << 0,
    // uv坐标
    TexCoord = 1 << 1,
    // 颜色
    Color = 1 << 2,
    // 法线
    Normal = 1 << 3,
    // 切线
    Tangent = 1 << 4,
};
inline VertexAttr operator|(VertexAttr a, VertexAttr b)
{
    return static_cast<VertexAttr>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

using VertexLayout = uint32_t;

class Geometry
{
public:
    Geometry();
    virtual ~Geometry();

    GLint    GetVAO() const { return m_VAO; }
    uint32_t GetIndicesCnt() const { return m_IndicesCnt; }

protected:
    /**
     * @param vertices vbo
     * @param vertSz 多少个byte
     * @param layout vertice属性可能不光只有顶点坐标 可能还有uv坐标或者其他的 用枚举位运算标识顶点属性信息类型
     * @param indices ebo
     * @param indexSz 多少个byte
     */
    void setupBuffers(const void* vertices, size_t vertSz, VertexLayout layout, const uint32_t* indices,
                      size_t indexSz);
    /**
     * @param positions vbo的顶点坐标
     * @param normals vbo的法线
     * @param uvs vbo的uv坐标
     * @param indices vao索引
     */
    void setupBuffers(const std::vector<float>& positions, const std::vector<float>& uvs,
                      const std::vector<float>& normals, const std::vector<uint32_t>& indices);

protected:
    GLuint m_VAO{0};
    GLuint m_VBO{0};
    GLuint m_EBO{0};
    // EBO的索引数量
    uint32_t m_IndicesCnt{0};
};