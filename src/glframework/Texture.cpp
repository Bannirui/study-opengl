//
// Created by dingrui on 25-7-14.
//

#include "glframework/Texture.h"

#include <stb_image.h>

Texture::Texture(const std::string& path, unsigned int unit) : m_Uint(unit)
{
    // 告诉stbi处理图像数据的时候跟OpenGL保持一致 左下角0坐标
    stbi_set_flip_vertically_on_load(true);
    int            nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &m_Width, &m_Height, &nrChannels, STBI_rgb_alpha);
    // 创建纹理对象
    glGenTextures(1, &m_Texture);
    // 激活纹理单元
    glActiveTexture(GL_TEXTURE0 + m_Uint);
    // 纹理对象绑定到OpenGL状态机插槽
    // 将纹理对象绑定到纹理单元 OpenGL默认至少16个纹理单元
    // 没有使用glActiveTexture()显式用指定纹理单元就默认使用0号纹理单元
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    // 开辟显存 传输数据
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);
    // 纹理包裹 当UV坐标超出0 1怎么处理
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 纹理过滤器
    // 需要像素<图片像素 使用Nearest
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // 需要像素>图片像素 使用Linear
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

Texture::~Texture()
{
    if (m_Texture != 0) glDeleteTextures(1, &m_Texture);
}

void Texture::Bind()
{
    // OpenGL是状态机 不知道当前状态机的纹理单元 所以要先切换纹理单元 然后绑定纹理对象
    glActiveTexture(GL_TEXTURE0 + m_Uint);
    glBindTexture(GL_TEXTURE_2D, m_Texture);
}
