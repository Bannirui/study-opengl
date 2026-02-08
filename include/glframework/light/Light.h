//
// Created by dingrui on 25-7-23.
//

#pragma once

#include <glm/glm.hpp>

class Light
{
public:
    virtual ~Light() = default;
    void      set_color(glm::vec3 color) { this->m_color = color; }
    glm::vec3 get_color() const { return this->m_color; }
    void      set_specular_intensity(float intensity) { this->m_specularIntensity = intensity; }
    float     get_specular_intensity() const { return this->m_specularIntensity; }

protected:
    Light() = default;

private:
    // 光的颜色强度
    glm::vec3 m_color{1.0f};
    // 镜面反射的强度
    float m_specularIntensity{1.0f};
};
