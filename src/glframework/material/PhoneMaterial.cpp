//
// Created by dingrui on 25-7-23.
//

#include "glframework/material/PhoneMaterial.h"

#include "glframework/Texture.h"

PhoneMaterial::PhoneMaterial()
{
    m_type = MaterialType::PhoneMaterial;
}

PhoneMaterial::~PhoneMaterial()
{
    delete m_diffuse;
}
