//
// Created by dingrui on 25-7-23.
//

#pragma once

#include "glframework/Object.h"

class Geometry;
class Material;

/**
 * 网格体
 */
class Mesh : public Object
{
public:
    Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material);

    const Geometry& getGeometry() const noexcept { return *m_geometry; };
    const Material& getMaterial() const noexcept { return *m_material; };

    void render(const Renderer&, const Camera&, const LightPack&) const override;

private:
    // 形状和材质于Mesh而言 Mesh并不是拥有 而仅仅是挂载 Geometry可以被多个Mesh共享 Material可以被多个Mesh共享
    // 又因为是多态的 所以用智能指针修饰 形状
    std::shared_ptr<Geometry> m_geometry;
    // 材质
    std::shared_ptr<Material> m_material;
};
