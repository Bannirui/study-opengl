//
// Created by dingrui on 25-7-24.
//

#pragma once

#include "Material.h"
#include "glframework/Texture.h"

class ScreenMaterial : public Material {
public:
    explicit ScreenMaterial();

    void ApplyUniforms(Shader &shader, Mesh &mesh, const Camera &camera, const LightPack &lights) const override;

    void set_screenTexture(std::shared_ptr<Texture>& texture) { m_screenTexture = texture; }

private:
    std::shared_ptr<Texture> m_screenTexture;
};
