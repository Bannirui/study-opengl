//
// Created by dingrui on 25-7-23.
//

#include "glframework/Mesh.h"

#include "glframework/Shader.h"
#include "glframework/geo/Geometry.h"

#include <glframework/material/Material.h>

Mesh::Mesh(std::shared_ptr<Geometry> geometry, std::shared_ptr<Material> material)
    : Object(ObjectType::Mesh), m_geometry(std::move(geometry)), m_material(std::move(material))
{
}

void Mesh::Render(const Renderer& renderer, const Camera& camera, const LightPack& lights) const
{
    // 用哪个shader
    Shader& shader = m_material->shader(renderer);
    // 更新shader的uniform变量
    shader.Bind();
    m_material->applyUniforms(shader, *this, camera, lights);
    // 绑定VAO
    glBindVertexArray(m_geometry->GetVAO());
    // 执行绘制指令
    glDrawElements(GL_TRIANGLES, m_geometry->GetIndicesCnt(), GL_UNSIGNED_INT, 0);
    // 解绑VAO
    glBindVertexArray(0);
    shader.Unbind();
}
