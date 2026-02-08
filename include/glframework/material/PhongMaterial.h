//
// Created by dingrui on 25-7-23.
//

#pragma once

#include "glframework/material/Material.h"

class Texture;

// 平行光
class PhongMaterial : public Material
{
public:
    explicit PhongMaterial();
    ~PhongMaterial() override;

    void ApplyUniforms(Shader& shader, const Mesh& mesh, const Camera& camera, const LightPack& lights) const override;

    void set_diffuse(Texture* texture) { this->m_diffuse = texture; }
    void set_specular_mask(Texture* texture) { this->m_specularMask = texture; }
    void set_shines(const float shines) { this->m_shines = shines; }

private:
    // 漫反射和环境光都用贴图颜色
    Texture* m_diffuse{nullptr};
    // 高光蒙版
    Texture* m_specularMask{nullptr};
    // 控制高光反射光斑大小 反射方向跟目光方向夹角的cos 夹角越大 看到的反射高光越弱 cos的n次方
    float m_shines{1.0f};
};
