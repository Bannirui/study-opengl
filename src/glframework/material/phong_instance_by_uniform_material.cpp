#include "glframework/material/phong_instance_by_uniform_material.h"

#include "glframework/Texture.h"
#include "glframework/renderer/Renderer.h"
#include "glframework/renderer/light_pack.h"
#include "glframework/light/AmbientLight.h"
#include "glframework/light/SpotLight.h"
#include "glframework/light/PointLight.h"
#include "glframework/light/DirectionalLight.h"
#include "glframework/obj/mesh/Mesh.h"
#include "application/camera/Camera.h"

PhongInstanceMaterial::PhongInstanceMaterial() : Material("asset/shader/instance_by_uniform.glsl") {}

void PhongInstanceMaterial::ApplyUniforms(Shader& shader, Mesh& mesh, const Camera& camera,
                                          const LightPack& lights) const
{
    if (m_diffuse)
    {
        // 将纹理对象跟纹理单元绑定
        m_diffuse->Bind();
        // diffuse贴图 将纹理采样器跟纹理单元绑定
        shader.setInt("u_diffuseSampler", m_diffuse->get_unit());
    }
    // 高光蒙版贴图
    if (m_specularMask)
    {
        m_specularMask->Bind();
        shader.setInt("u_specularMaskSampler", m_specularMask->get_unit());
    }
    // 模型变换矩阵 aPos模型->世界空间
    shader.setMat4("u_model", glm::value_ptr(mesh.GetModelMatrix()));
    // 视图矩阵 世界空间->摄影机空间
    shader.setMat4("u_view", glm::value_ptr(camera.GetViewMatrix()));
    shader.setMat4("u_projection", glm::value_ptr(camera.GetProjectionMatrix()));
    // normal matrix
    auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh.GetModelMatrix())));
    shader.setMat3("u_normalMatrix", glm::value_ptr(normalMatrix));
    // 平行光
    if (lights.HasDirection())
    {
        shader.setBool("u_activeDirectionalLight", true);
        shader.setFloatVec3("u_directionalLight.direction", lights.directional->get_direction());
        shader.setFloatVec3("u_directionalLight.color", lights.directional->get_color());
        // 高光反射强度
        shader.setFloat("u_directionalLight.specularIntensity", lights.directional->get_specular_intensity());
    }
    // 点光
    if (lights.HasSpot())
    {
        shader.setBool("u_activePointLight", true);
        shader.setFloatVec3("u_pointLight.pos", lights.point->get_position());
        shader.setFloatVec3("u_pointLight.color", lights.point->get_color());
        // 高光反射强度
        shader.setFloat("u_pointLight.specularIntensity", lights.point->get_specular_intensity());
        shader.setFloat(".u_pointLight.k2", lights.point->m_k2);
        shader.setFloat("u_pointLight.k1", lights.point->m_k1);
        shader.setFloat("u_pointLight.kc", lights.point->m_kc);
    }
    // 聚光灯
    if (lights.HasSpot())
    {
        shader.setBool("u_activeSpotLight", true);
        shader.setFloatVec3("u_spotLight.pos", lights.spot->get_position());
        shader.setFloatVec3("u_spotLight.targetDirection", lights.spot->m_targetDirection);
        shader.setFloatVec3("u_spotLight.color", lights.spot->get_color());
        shader.setFloat("u_spotLight.innerCos", glm::cos(glm::radians(lights.spot->m_innerAngle)));
        shader.setFloat("u_spotLight.outerCos", glm::cos(glm::radians(lights.spot->m_outerAngle)));
        shader.setFloat("u_spotLight.specularIntensity", lights.spot->get_specular_intensity());
    }
    // 环境光
    if (lights.HasAmbient())
    {
        shader.setFloatVec3("u_ambientColor", lights.ambient->get_color());
    }
    // 控制高光反射光斑大小
    shader.setFloat("u_shiness", this->m_shines);
    // 相机位置
    shader.setFloatVec3("u_cameraPos", camera.get_position());
    // opacity
    shader.setFloat("u_opacity", this->m_opacity);
}
