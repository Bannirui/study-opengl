//
// Created by dingrui on 2026/2/15.
//

#pragma once
#include "Material.h"

class Texture;

class CubeMaterial : public Material
{
public:
    CubeMaterial();

    void set_diffuse(std::shared_ptr<Texture>& diffuse) { m_diffuse = diffuse; }

protected:
    void ApplyUniforms(Shader& shader, Mesh& mesh, const Camera& camera, const LightPack& lights) const override;

private:
    std::shared_ptr<Texture> m_diffuse;
};
