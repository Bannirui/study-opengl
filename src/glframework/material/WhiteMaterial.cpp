//
// Created by dingrui on 25-7-24.
//

#include "glframework/material/WhiteMaterial.h"

#include "glframework/Mesh.h"
#include "glframework/renderer/Renderer.h"

#include "application/camera/Camera.h"

WhiteMaterial::WhiteMaterial() : Material(MaterialType::WhiteMaterial) {}

Shader& WhiteMaterial::GetShader(const Renderer& renderer) const
{
    return renderer.getShader(this->get_type());
}

void WhiteMaterial::ApplyUniforms(Shader& shader, const Mesh& mesh, const Camera& camera, const LightPack& lights) const
{
    // mvp变换矩阵
    shader.setMat4("u_model", glm::value_ptr(mesh.GetModelMatrix()));
    shader.setMat4("u_view", glm::value_ptr(camera.GetViewMatrix()));
    shader.setMat4("u_projection", glm::value_ptr(camera.GetProjectionMatrix()));
}
