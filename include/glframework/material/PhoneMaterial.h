//
// Created by dingrui on 25-7-23.
//

#pragma once

#include "glframework/material/Material.h"

class Texture;

// 平行光
class PhoneMaterial : public Material
{
public:
    PhoneMaterial();
    ~PhoneMaterial() override;

public:
    // 漫反射和环境光都用贴图颜色
    Texture* m_diffuse{nullptr};
    // 高光蒙版
    Texture* m_specularMask{nullptr};
    // 控制高光反射光斑大小 反射方向跟目光方向夹角的cos 夹角越大 看到的反射高光越弱 cos的n次方
    float m_shiness{1.0f};
};
