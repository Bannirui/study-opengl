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
    Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material);

    const Geometry& get_geometry() const noexcept { return *m_geometry; };
    const Material& get_material() const noexcept { return *m_material; };
    void            set_material(std::shared_ptr<Material> material) { m_material = std::move(material); }

    void Render(const Renderer&, const Camera&, const LightPack&) override;

protected:
    // 形状
    std::shared_ptr<Geometry> m_geometry;
    // 材质
    std::shared_ptr<Material> m_material;
};
