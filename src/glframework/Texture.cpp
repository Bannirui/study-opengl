//
// Created by dingrui on 25-7-14.
//

#include "glframework/Texture.h"

#include <stb_image.h>
#include <iostream>

Texture::Texture(const std::string& path, int unit) : m_Uint(unit)
{
    // 告诉stbi处理图像数据的时候跟OpenGL保持一致 左下角0坐标
    stbi_set_flip_vertically_on_load(true);
    int            nrChannels;
    unsigned char* data = stbi_load(path.c_str(), &m_Width, &m_Height, &nrChannels, STBI_default);
    if (!data)
    {
        std::cout << "ERROR::TEXTURE::fail to read picture" << std::endl;
        exit(EXIT_FAILURE);
    }
    // 创建纹理对象
    glGenTextures(1, &m_Texture);
    // 激活纹理单元
    glActiveTexture(GL_TEXTURE0 + m_Uint);
    // 纹理对象绑定到OpenGL状态机插槽
    // 将纹理对象绑定到纹理单元 OpenGL默认至少16个纹理单元
    // 没有使用glActiveTexture()显式用指定纹理单元就默认使用0号纹理单元
    glBindTexture(GL_TEXTURE_2D, m_Texture);
    // 开辟显存 传输数据
    // level mipmap的层级 如果这个api调用的时候level传入了非0的值 就意味着默认要开启opengl的mipmap功能 就一定要传给opengl图片大小一直到1*1大小 如果没有一直传入图片 就会报错
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
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

int Texture::GetUnit()
{
    return m_Uint;
}

