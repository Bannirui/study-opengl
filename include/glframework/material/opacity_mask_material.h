//
// Created by dingrui on 25-7-23.
//

#pragma once

#include "glframework/material/Material.h"

class Texture;

class OpacityMaskMaterial : public Material {
public:
    explicit OpacityMaskMaterial();

    void ApplyUniforms(Shader &shader, const Mesh &mesh, const Camera &camera, const LightPack &lights) override;

    void set_diffuse(std::shared_ptr<Texture>& texture) { m_diffuse = texture; }
    void set_opacityMask(std::shared_ptr<Texture>& texture) { m_opacityMask = texture; }
    void set_shines(const float shines) { m_shines = shines; }

private:
    // 漫反射和环境光都用贴图颜色
    std::shared_ptr<Texture> m_diffuse;
    // 蒙版
    std::shared_ptr<Texture> m_opacityMask;
    // 控制高光反射光斑大小 反射方向跟目光方向夹角的cos 夹角越大 看到的反射高光越弱 cos的n次方
    float m_shines{1.0f};
};
