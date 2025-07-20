//
// Created by dingrui on 25-7-20.
//

#pragma once
#include <glad/glad.h>

class Geometry
{
public:
    Geometry();
    ~Geometry();

    /**
     * @param length 正方体边长
     */
    static Geometry* CreateBox(float length);

    GLint    GetVAO() const { return m_VAO; }
    uint32_t GetIndicesCnt() const { return m_IndicesCnt; }

private:
    GLuint m_VAO{0};
    GLuint m_VBO{0};
    GLuint m_EBO{0};
    // EBO的索引数量
    uint32_t m_IndicesCnt{0};
};
