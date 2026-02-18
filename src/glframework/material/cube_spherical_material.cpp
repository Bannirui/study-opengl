#include "glframework/material/cube_spherical_material.h"

#include "glframework/mesh/Mesh.h"
#include "application/camera/Camera.h"
#include "glframework/Texture.h"

CubeSphericalMaterial::CubeSphericalMaterial() : Material("asset/shader/cube_spherical.glsl") {}

void CubeSphericalMaterial::ApplyUniforms(Shader& shader, Mesh& mesh, const Camera& camera, const LightPack& lights) const
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
