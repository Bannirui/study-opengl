//
// Created by dingrui on 25-7-23.
//

#pragma once

#include "PhoneMaterial.h"
#include "glframework/material/Material.h"

class Texture;

// 点光
class SpotLightMaterial : public PhoneMaterial
{
public:
    SpotLightMaterial() { m_type = MaterialType::SpotLightMaterial; }
    ~SpotLightMaterial() override {}
};
