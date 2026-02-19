//
// Created by dingrui on 25-7-23.
//

#pragma once

#include "glframework/obj/Object.h"
#include "glframework/Shader.h"

class Geometry;
class Material;

/**
 * 网格体
 */
class Mesh : public Object
{
public:
    Mesh(std::unique_ptr<Geometry> geometry, std::unique_ptr<Material> material);

    const Geometry& getGeometry() const noexcept { return *m_geometry; };
    const Material& getMaterial() const noexcept { return *m_material; };

    void Render(const Renderer&, const Camera&, const LightPack&) override;

protected:
    virtual void BindInstanceData(Shader& shader) const {};

protected:
    // 形状和材质于Mesh而言 Mesh并不是拥有 而仅仅是挂载
    // 形状
    std::unique_ptr<Geometry> m_geometry;
    // 材质
    std::unique_ptr<Material> m_material;
};
