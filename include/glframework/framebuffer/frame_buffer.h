//
// Created by dingrui on 2/11/26.
//

#pragma once

#include <cstdint>
#include <memory>

class Texture;

class FrameBuffer {
public:
    FrameBuffer(uint32_t width, uint32_t height);

    ~FrameBuffer();

    uint32_t get_FBO() const { return m_FBO; }
    Texture *get_colorAttach() const { return m_colorAttach.get(); };
    Texture *get_depthStencilAttach() const { return m_depthStencilAttach.get(); };

    uint32_t get_width() const { return m_width; }
    uint32_t get_height() const { return m_height; }

private:
    uint32_t m_width{0};
    uint32_t m_height{0};

    uint32_t m_FBO{0};

    std::unique_ptr<Texture> m_colorAttach;
    std::unique_ptr<Texture> m_depthStencilAttach;
};
