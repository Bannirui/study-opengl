//
// Created by dingrui on 25-7-14.
//

#include "glframework/Texture.h"

#include <stb_image.h>

#include "err_check.h"
#include "x_log.h"

// 定义
std::unordered_map<std::string, std::shared_ptr<Texture>> Texture::s_TextureCache{};

std::shared_ptr<Texture> Texture::CreateTexture(const std::string& path, uint32_t uint)
{
    if (s_TextureCache.count(path))
    {
        return s_TextureCache[path];
    }
    auto texture         = std::make_shared<Texture>(path, uint);
    s_TextureCache[path] = texture;
    return texture;
}

std::shared_ptr<Texture> Texture::CreateTexture(const std::string& path, const uint8_t* dataIn, uint32_t widthIn,
                                                uint32_t heightIn, uint32_t uint)
{
    if (s_TextureCache.count(path))
    {
        return s_TextureCache[path];
    }
    auto texture         = std::make_shared<Texture>(dataIn, widthIn, heightIn, uint);
    s_TextureCache[path] = texture;
    return texture;
}

void Texture::CreateColorAttach(uint32_t width, uint32_t height, uint32_t uint, Texture* texture)
{
    if (!texture)
    {
        return;
    }
}

void Texture::CreateDepthStencilAttach(uint32_t width, uint32_t height, uint32_t uint, Texture* texture)
{
    if (!texture)
    {
        return;
    }
    uint32_t depthStencil;
    glGenTextures(1, &depthStencil);
    glBindTexture(GL_TEXTURE_2D, depthStencil);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, static_cast<int>(width), static_cast<int>(height), 0,
                 GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr);

    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glad_glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    texture->m_Texture = depthStencil;
    texture->m_Width   = width;
    texture->m_Height  = height;
    texture->m_Uint    = uint;
}

Texture::Texture(const std::string& path, uint32_t unit) : m_Uint(unit)
{
    // 告诉stbi处理图像数据的时候跟OpenGL保持一致 左下角0坐标
    stbi_set_flip_vertically_on_load(true);
    int            nrChannels;
    unsigned char* data = stbi_load(path.c_str(), reinterpret_cast<int*>(&m_Width), reinterpret_cast<int*>(&m_Height),
                                    &nrChannels, STBI_default);
    if (!data)
    {
        XLOG_ERROR("TEXTURE::fail to read picture, {}", path);
        exit(EXIT_FAILURE);
    }
    // 创建纹理对象
    GL_CALL_AND_CHECK_ERR(glGenTextures(1, &m_Texture));
    // 激活纹理单元
    GL_CALL_AND_CHECK_ERR(glActiveTexture(GL_TEXTURE0 + m_Uint));
    // 纹理对象绑定到OpenGL状态机插槽
    // 将纹理对象绑定到纹理单元 OpenGL默认至少16个纹理单元
    // 没有使用glActiveTexture()显式用指定纹理单元就默认使用0号纹理单元
    GL_CALL_AND_CHECK_ERR(glBindTexture(GL_TEXTURE_2D, m_Texture));
    // 开辟显存 传输数据
    // level mipmap的层级 如果这个api调用的时候level传入了非0的值 就意味着默认要开启opengl的mipmap功能
    // 就一定要传给opengl图片大小一直到1*1大小 如果没有一直传入图片 就会报错
    GLenum internalFormat = (nrChannels == 4) ? GL_RGBA : GL_RGB;
    GLenum format         = (nrChannels == 4) ? GL_RGBA : GL_RGB;
    GL_CALL_AND_CHECK_ERR(
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data));
    // 启用opengl的mipmap功能 给gl状态机插槽GL_TEXTURE_2D上的图片生成mipmap
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    // 纹理包裹 当UV坐标超出0 1怎么处理
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 纹理过滤器
    // 需要像素<图片像素 使用Nearest 当物体远去变小的时候介于2个mipmap之间 让opengl用mipmap的插值图片
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    // 需要像素>图片像素 使用Linear
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::Texture(const uint8_t* dataIn, uint32_t widthIn, uint32_t heightIn, uint32_t uint) : m_Uint(uint)
{
    // 告诉stbi处理图像数据的时候跟OpenGL保持一致 左下角0坐标
    stbi_set_flip_vertically_on_load(true);
    // 图片大小 多少字节
    uint32_t dataSize = 0;
    if (!heightIn)
    {
        dataSize = widthIn;
    }
    else
    {
        dataSize = widthIn * heightIn * 4;
    }
    int            channels = 0;
    unsigned char* data     = stbi_load_from_memory(dataIn, dataSize, reinterpret_cast<int*>(&widthIn),
                                                    reinterpret_cast<int*>(&heightIn), &channels, STBI_default);
    if (!data)
    {
        XLOG_ERROR("TEXTURE::fail to read picture from memory");
        exit(EXIT_FAILURE);
    }
    GL_CALL_AND_CHECK_ERR(glGenTextures(1, &m_Texture));
    GL_CALL_AND_CHECK_ERR(glActiveTexture(GL_TEXTURE0 + m_Uint));
    GL_CALL_AND_CHECK_ERR(glBindTexture(GL_TEXTURE_2D, m_Texture));
    GLenum internalFormat = (channels == 4) ? GL_RGBA : GL_RGB;
    GLenum format         = (channels == 4) ? GL_RGBA : GL_RGB;
    GL_CALL_AND_CHECK_ERR(
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, data));
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::Texture(uint32_t width, uint32_t height, uint32_t uint) : m_Width(width), m_Height(height), m_Uint(uint)
{
    glGenTextures(1, &m_Texture);
    glActiveTexture(GL_TEXTURE0 + m_Uint);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<int>(m_Width), static_cast<int>(m_Height), 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::Texture(const std::vector<std::string>& sortPaths, uint32_t uint)
    : m_Uint(uint), m_textureTarget(GL_TEXTURE_3D)
{
    glGenTextures(1, &m_Texture);
    glActiveTexture(GL_TEXTURE0 + m_Uint);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_Texture);

    int      channels;
    int      width = 0, height = 0;
    uint8_t* data = nullptr;
    for (int i = 0, sz = sortPaths.size(); i < sz; ++i)
    {
        data = stbi_load(sortPaths[i].c_str(), &width, &height, &channels, STBI_rgb_alpha);
        if (!data)
        {
            XLOG_ERROR("TEXTURE::fail to read picture for {}", sortPaths[i]);
        }
        else
        {
            int    internalFormat = (channels == 4) ? GL_RGBA : GL_RGB;
            GLenum format         = (channels == 4) ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, internalFormat, width, height, 0, format,
                         GL_UNSIGNED_BYTE, data);
        }
        stbi_image_free(data);
    }
    // 需要像素<图片像素 使用Nearest 当物体远去变小的时候介于2个mipmap之间 让opengl用mipmap的插值图片
    glTexParameteri(m_textureTarget, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // 需要像素>图片像素 使用Linear
    glTexParameteri(m_textureTarget, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    // 纹理包裹 当UV坐标超出0 1怎么处理
    glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(m_textureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Texture::~Texture()
{
    if (m_Texture != 0)
    {
        glDeleteTextures(1, &m_Texture);
    }
}

void Texture::Bind() const
{
    // OpenGL是状态机 不知道当前状态机的纹理单元 所以要先切换纹理单元 然后绑定纹理对象
    GL_CALL_AND_CHECK_ERR(glActiveTexture(GL_TEXTURE0 + m_Uint));
    GL_CALL_AND_CHECK_ERR(glBindTexture(m_textureTarget, m_Texture));
}
