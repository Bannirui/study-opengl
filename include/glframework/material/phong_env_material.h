#pragma once

#include "glframework/material/Material.h"

class Texture;

class PhongEnvMaterial : public Material
{
public:
    explicit PhongEnvMaterial();

    void ApplyUniforms(Shader& shader, Mesh& mesh, const Camera& camera, const LightPack& lights) const override;

    void set_diffuse(std::shared_ptr<Texture>& texture) { m_diffuse = texture; }
    void set_specular_mask(std::shared_ptr<Texture>& texture) { m_specularMask = texture; }
    void set_shines(const float shines) { this->m_shines = shines; }
    void set_env(std::shared_ptr<Texture>& texture) { m_env = texture; }

private:
    // 漫反射和环境光都用贴图颜色
    std::shared_ptr<Texture> m_diffuse;
    // 高光蒙版
    std::shared_ptr<Texture> m_specularMask;
    // 控制高光反射光斑大小 反射方向跟目光方向夹角的cos 夹角越大 看到的反射高光越弱 cos的n次方
    float                m_shines{1.0f};
    std::shared_ptr<Texture> m_env;
};
