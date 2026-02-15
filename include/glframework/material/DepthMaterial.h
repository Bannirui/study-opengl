//
// Created by dingrui on 25-7-24.
//

#pragma once

#include "Material.h"

class DepthMaterial : public Material
{
public:
    explicit DepthMaterial();
    void ApplyUniforms(Shader& shader, Mesh& mesh, const Camera& camera, const LightPack& lights) const override;
};
