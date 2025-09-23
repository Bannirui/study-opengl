//
// Created by dingrui on 25-7-23.
//

#include "glframework/material/PhongMaterial.h"

#include "glframework/Texture.h"

PhongMaterial::PhongMaterial()
{
    m_type = MaterialType::PhoneMaterial;
}

PhongMaterial::~PhongMaterial()
{
    delete m_diffuse;
    delete m_specularMask;
}
