//
// Created by dingrui on 25-7-24.
//

#include "glframework/material/screen_material.h"

#include "glframework/renderer/Renderer.h"

ScreenMaterial::ScreenMaterial()
    : Material(std::make_shared<Shader>("asset/shader/screen.glsl")) {
}

void ScreenMaterial::set_screenTexture(std::shared_ptr<Texture> texture) {
    m_screenTexture = std::move(texture);
}

void ScreenMaterial::ApplyUniforms(Shader &shader, const Mesh &mesh, const Camera &camera,
                                   const LightPack &lights) const {
    if (m_screenTexture) {
        // 将纹理对象跟纹理单元绑定
        m_screenTexture->Bind();
        // diffuse贴图 将纹理采样器跟纹理单元绑定
        shader.setInt("u_screenTextureSampler", m_screenTexture->get_unit());
    }
}
