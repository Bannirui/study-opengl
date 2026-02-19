//
// Created by dingrui on 2026/2/18.
//

#pragma once

#include <vector>

#include "glframework/mesh/Mesh.h"

class InstanceMesh : public Mesh
{
public:
    /**
     * @param num how many instances
     */
    InstanceMesh(std::unique_ptr<Geometry> geometry, std::unique_ptr<Material> material);
    ~InstanceMesh() override = default;

    void Render(const Renderer& renderer, const Camera& camera, const LightPack& lights) override;

    void AddInstanceMatric(const glm::mat4& v) { m_instanceMatrices.push_back(v); }

protected:
    void BindInstanceData(Shader& shader) const override;

private:
    std::vector<glm::mat4> m_instanceMatrices;
};