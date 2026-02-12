//
// Created by dingrui on 2/11/26.
//

#include "glframework/framebuffer/frame_buffer.h"

#include <glad/glad.h>

#include "x_log.h"
#include "glframework/Texture.h"

FrameBuffer::FrameBuffer(uint32_t width, uint32_t height)
    : m_width(width), m_height(height) {
    // generate and bind
    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
    // color attachment and insert to FBO
    m_colorAttach = std::make_unique<Texture>(m_width, m_height, 0);
    Texture::CreateColorAttach(m_width, m_height, 0, m_colorAttach.get());
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colorAttach->get_texture(), 0);
    // depth stencil attachment and insert to FBO
    m_depthStencilAttach = std::make_unique<Texture>();
    Texture::CreateDepthStencilAttach(m_width, m_height, 0, m_depthStencilAttach.get());
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D,
                           m_depthStencilAttach->get_texture(), 0);
    // check
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        XLOG_ERROR("FrameBuffer is not complete");
    }
    // unbind
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

FrameBuffer::~FrameBuffer() {
    if (m_FBO) {
        glDeleteFramebuffers(1, &m_FBO);
    }
}
