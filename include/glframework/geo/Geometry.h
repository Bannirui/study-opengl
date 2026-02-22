//
// Created by dingrui on 25-7-20.
//

#pragma once
#include <glad/glad.h>
#include <cstddef>

class Geometry
{
public:
    enum class VertexPosDim : uint8_t
    {
        kNone = 0,  // no need stride
        k2D   = 2,  // vertex xy pos, stride equals 2*float
        k3D   = 3,  // vertex xyz pos, stride equals 3*float
    };

    enum class VBOType : uint32_t
    {
        kNone = 0,
        // uv坐标
        kTexCoord = 1 << 0,
        // 颜色
        kColor = 1 << 1,
        // 法线
        kNormal = 1 << 2,
        // 切线
        kTangent = 1 << 3,
    };

    struct VertexLayout
    {
        // xyz坐标
        VertexPosDim posDim;
        // uv坐标 颜色 法线 切线
        VBOType attrs;
    };

public:
    virtual ~Geometry();

    GLint    get_VAO() const { return m_VAO; }
    uint32_t get_IndicesCnt() const { return m_IndicesCnt; }

protected:
    Geometry() = default;

    /**
     * @param vertices vbo
     * @param vertSz 多少个byte
     * @param layout vertice属性可能不光只有顶点坐标 可能还有uv坐标或者其他的 用枚举位运算标识顶点属性信息类型
     * @param indices ebo
     * @param indexSz 多少个byte
     */
    void SetupBuffers(const void* vertices, size_t vertSz, VertexLayout layout, const uint32_t* indices,
                              size_t indexSz);

protected:
    struct AttrDesc
    {
        VBOType  types;       // attribute, uv 颜色 法线 切线
        uint32_t components;  // how many float for the attribute
    };

    static constexpr AttrDesc kVBOTable[] = {
        {VBOType::kTexCoord, 2},    // uv, stride 2*floats
        {VBOType::kColor, 3},    // rgb, stride 3*floats
        {VBOType::kNormal, 3},   // xyz, stride 3*floats
        {VBOType::kTangent, 3},  // xyz, stride 3*floats
    };

protected:
    friend constexpr VBOType operator|(VBOType x, VBOType y)
    {
        return static_cast<VBOType>(static_cast<uint32_t>(x) | static_cast<uint32_t>(y));
    }

    friend constexpr VBOType operator&(VBOType x, VBOType y)
    {
        return static_cast<VBOType>(static_cast<uint32_t>(x) & static_cast<uint32_t>(y));
    }

    friend constexpr VBOType& operator|=(VBOType& x, VBOType y)
    {
        x = x | y;
        return x;
    }

    static constexpr bool Has(VBOType value, VBOType flag) { return (value & flag) != VBOType::kNone; }

protected:
    GLuint m_VAO{0};
    GLuint m_VBO{0};
    GLuint m_EBO{0};
    // EBO的索引数量
    uint32_t m_IndicesCnt{0};
};
