//
// Created by dingrui on 25-7-24.
//

#include "glframework/material/screen_material.h"

ScreenMaterial::ScreenMaterial()
    : Material("asset/shader/screen.glsl") {
}

void ScreenMaterial::ApplyUniforms(Shader &shader, Mesh &mesh, const Camera &camera,
                                   const LightPack &lights) const {
    if (m_screenTexture) {
        // 将纹理对象跟纹理单元绑定
        m_screenTexture->Bind();
        // diffuse贴图 将纹理采样器跟纹理单元绑定
        shader.setInt("u_screenTextureSampler", m_screenTexture->get_unit());
        // todo how to get app size
        shader.setFloat("u_textureWidth", 1200);
        shader.setFloat("u_textureHeight", 800);
    }
}
