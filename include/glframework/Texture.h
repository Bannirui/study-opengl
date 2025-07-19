//
// Created by dingrui on 25-7-14.
//

#pragma once

#include <string>

#include "glframework/Core.h"

class Texture
{
   public:
    /**
     * @param path 图片路径
     * @param unit 要绑到哪个纹理单元
     */
    Texture(const std::string& path, int unit);
    ~Texture();

    void Bind();

    int GetUnit();

   private:
    GLuint m_Texture{0};
    int    m_Width{0};
    int    m_Height{0};
    // 纹理单元
    int m_Uint{0};
};
