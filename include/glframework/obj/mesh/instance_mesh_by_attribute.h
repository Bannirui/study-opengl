#pragma once

#include <vector>

#include "glframework/obj/mesh/Mesh.h"

class InstanceMeshByAttribute : public Mesh
{
public:
    /**
     * @param num how many instances
     */
    InstanceMeshByAttribute(std::unique_ptr<Geometry> geometry, std::unique_ptr<Material> material,
                            const std::vector<glm::mat4>& attributes);
    ~InstanceMeshByAttribute() override;

    void Render(const Renderer& renderer, const Camera& camera, const LightPack& lights) override;
    void UpdateVBO();

private:
    void bindVBO();

private:
    std::vector<glm::mat4> m_instanceMatrices;
    uint32_t               m_matrixVBOId{0};
};