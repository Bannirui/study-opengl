#pragma once

#include <vector>

#include "glframework/obj/mesh/Mesh.h"

class InstanceMeshByAttribute : public Mesh
{
public:
    /**
     * @param num how many instances
     */
    InstanceMeshByAttribute(std::unique_ptr<Geometry> geometry, std::unique_ptr<Material> material);
    ~InstanceMeshByAttribute() override = default;

    void Render(const Renderer& renderer, const Camera& camera, const LightPack& lights) override;

    void AddInstanceMatric(const glm::mat4& v) { m_instanceMatrices.push_back(v); }

private:
    void bindVBO() const;

private:
    std::vector<glm::mat4> m_instanceMatrices;
    int                    m_matrixVBOId{0};
};