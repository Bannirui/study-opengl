//
// Created by dingrui on 25-7-20.
//

#pragma once
#include <glad/glad.h>
#include <cstddef>

enum class VertexAttr
{
    Position = 1 << 0,
    Normal   = 1 << 1,
    TexCoord = 1 << 2,
    Color    = 1 << 3,
};
inline VertexAttr operator|(VertexAttr a, VertexAttr b)
{
    return static_cast<VertexAttr>(static_cast<uint32_t>(a) | static_cast<uint32_t>(b));
}

using VertexLayout = uint32_t;

class Geometry
{
public:
    virtual ~Geometry();

    GLint    GetVAO() const { return m_VAO; }
    uint32_t GetIndicesCnt() const { return m_IndicesCnt; }

protected:
    /**
     * @param vertices vbo
     * @param vertSz sizeof
     * @param layout vertice属性可能不光只有顶点坐标 可能还有uv坐标或者其他的 用枚举位运算标识顶点属性信息类型
     * @param indices ebo
     * @param indexSz sizeof
     */
    void setupBuffers(const void* vertices, size_t vertSz, VertexLayout layout, const uint32_t* indices,
                      size_t indexSz);

protected:
    GLuint m_VAO{0};
    GLuint m_VBO{0};
    GLuint m_EBO{0};
    // EBO的索引数量
    uint32_t m_IndicesCnt{0};
};

// 正方体
class GeoBox : public Geometry
{
public:
    /**
     * @param sz 正方体边长
     */
    explicit GeoBox(float sz = 1.0f);
};