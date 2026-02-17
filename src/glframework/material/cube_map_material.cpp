//
// Created by dingrui on 2026/2/15.
//

#include "glframework/material/cube_map_material.h"

#include "glframework/Mesh.h"
#include "application/camera/Camera.h"
#include "glframework/Texture.h"

CubeMapMaterial::CubeMapMaterial() : Material("asset/shader/cube_map.glsl") {}

void CubeMapMaterial::ApplyUniforms(Shader& shader, Mesh& mesh, const Camera& camera, const LightPack& lights) const
{
    mesh.set_position(camera.get_position());
    // mvp变换矩阵
    shader.setMat4("u_model", glm::value_ptr(mesh.GetModelMatrix()));
    shader.setMat4("u_view", glm::value_ptr(camera.GetViewMatrix()));
    shader.setMat4("u_projection", glm::value_ptr(camera.GetProjectionMatrix()));
    if (m_diffuse) {
        // 将纹理对象跟纹理单元绑定
        m_diffuse->Bind();
        // diffuse贴图 将纹理采样器跟纹理单元绑定
        shader.setInt("u_cubeSampler", m_diffuse->get_unit());
    }
}
