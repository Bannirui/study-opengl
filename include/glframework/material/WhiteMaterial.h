//
// Created by dingrui on 25-7-24.
//

#pragma once

#include "Material.h"

// 白色材质用于点光源
class WhiteMaterial : public Material
{
public:
    WhiteMaterial();
    ~WhiteMaterial() override;
};
