//
// Created by dingrui on 25-7-23.
//

#pragma once

#include "glframework/material/Material.h"

class Texture;

class PhoneMaterial : public Material
{
public:
    PhoneMaterial();
    ~PhoneMaterial();

public:
    // 漫反射和环境光都用贴图颜色
    Texture* m_diffuse{nullptr};
    // 控制高光反射光斑大小 反射方向跟目光方向夹角的cos 夹角越大 看到的反射高光越弱 cos的n次方
    float m_shiness{1.0f};
};
