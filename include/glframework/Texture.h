//
// Created by dingrui on 25-7-14.
//

#pragma once

#include <string>

#include "glframework/Core.h"

class Texture
{
public:
    static Texture* CreateTexture(const std::string& path, unsigned int uint);
    static Texture* CreateTexture(const std::string& path, const uint8_t* dataIn, int widthIn, int heightIn,
                                  uint32_t uint);

    /**
     * @param path 图片路径
     * @param unit 要绑到哪个纹理单元
     */
    Texture(const std::string& path, int unit);
    /**
     * 把从assimp中加载出来的纹理图片生成纹理对象
     * assimp规定 如果内嵌纹理是png或者jpg格式 height=0 width就代表了图片的大小
     * @param dataIn 内存中图片 压缩过的
     * @param widthIn 图片宽度
     * @param heightIn 图片高度
     * @param uint 要绑定的纹理单元
     */
    Texture(const uint8_t* dataIn, int widthIn, int heightIn, uint32_t uint);
    ~Texture();

    void Bind() const;
    /**
     * @return 纹理对象绑定的纹理单元是哪个
     */
    int  GetUnit() const;
    void SetUint(const int unit) { m_Uint = unit; }

private:
    GLuint m_Texture{0};
    int    m_Width{0};
    int    m_Height{0};
    // 纹理单元
    int m_Uint{0};

    // 声明纹理缓存
    static std::unordered_map<std::string, Texture*> m_TextureCache;
};
