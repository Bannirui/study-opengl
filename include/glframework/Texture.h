//
// Created by dingrui on 25-7-14.
//

#pragma once

#include <memory>
#include <string>

#include "glframework/Core.h"

class Texture {
public:
    /**
     * from picture file
     */
    static Texture *CreateTexture(const std::string &path, uint32_t uint);

    /**
     * from stbi, memory data
     */
    static Texture *CreateTexture(const std::string &path, const uint8_t *dataIn, uint32_t widthIn, uint32_t heightIn,
                                  uint32_t uint);

    /**
     * @param texture out param
     */
    static void CreateColorAttach(uint32_t width, uint32_t height, uint32_t uint, Texture *texture);

    /**
     * @param texture out param
     */
    static void CreateDepthStencilAttach(uint32_t width, uint32_t height, uint32_t uint, Texture *texture);

    Texture() = default;

    /**
     * @param path 图片路径
     * @param unit 要绑到哪个纹理单元
     */
    Texture(const std::string &path, uint32_t unit);

    /**
     * 把从assimp中加载出来的纹理图片生成纹理对象
     * assimp规定 如果内嵌纹理是png或者jpg格式 height=0 width就代表了图片的大小
     * @param dataIn 内存中图片 压缩过的
     * @param widthIn 图片宽度
     * @param heightIn 图片高度
     * @param uint 要绑定的纹理单元
     */
    Texture(const uint8_t *dataIn, uint32_t widthIn, uint32_t heightIn, uint32_t uint);

    // no data, it means empty texture
    Texture(uint32_t width, uint32_t height, uint32_t uint);

    ~Texture();

    void Bind() const;

    uint32_t get_texture() const { return m_Texture; }
    /**
     * @return 纹理对象绑定的纹理单元是哪个
     */
    uint32_t get_unit() const { return m_Uint; }
    void set_uint(const uint32_t unit) { m_Uint = unit; }

public:
    // 声明纹理缓存
    static std::unordered_map<std::string, std::unique_ptr<Texture>> s_TextureCache;

private:
    uint32_t m_Texture{0};
    uint32_t m_Width{0};
    uint32_t m_Height{0};
    // 纹理单元
    uint32_t m_Uint{0};
};
