//
// Created by dingrui on 25-7-23.
//

#include "glframework/renderer/Renderer.h"

#include "application/camera/Camera.h"
#include "glframework/Mesh.h"
#include "glframework/Shader.h"
#include "glframework/Texture.h"
#include "glframework/geo/Geometry.h"
#include "glframework/light/AmbientLight.h"
#include "glframework/light/DirectionalLight.h"
#include "glframework/material/Material.h"
#include "glframework/material/PhoneMaterial.h"

Renderer::Renderer()
{
    m_phoneShader = new Shader("resources/shader/phone_shader.glsl");
}
Renderer::~Renderer()
{
    delete m_phoneShader;
}
void Renderer::render(const std::vector<Mesh*>& meshes, Camera* camera, DirectionalLight* directionalLight,
                      AmbientLight* ambientLight)
{
    // 设置当前帧绘制的必要gl状态机参数 开启deep testing 不开启深度缓存的话 后绘制的会覆盖先绘制的
    glEnable(GL_DEPTH_TEST);
    // 设置深度测试方法
    glDepthFunc(GL_LESS);
    // 清理画布 每一帧都要清屏 防止残留前一帧图像 清理的时候也要清理深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // 遍历mesh绘制
    for (int i = 0, sz = meshes.size(); i < sz; i++)
    {
        auto mesh     = meshes[i];
        auto geometry = mesh->m_geometry;
        auto material = mesh->m_material;
        // 用哪个shader
        Shader* shader = getShader(material->m_type);
        // 更新shader的uniform变量
        shader->Bind();
        switch (material->m_type)
        {
            case MaterialType::PhoneMaterial:
            {
                PhoneMaterial* curMaterial = (PhoneMaterial*)material;
                // 将纹理对象跟纹理单元绑定
                curMaterial->m_diffuse->Bind();
                // diffuse贴图 将纹理采样器跟纹理单元绑定
                shader->setInt("u_sampler", curMaterial->m_diffuse->GetUnit());
                // 模型变换矩阵 aPos模型->世界空间
                shader->setMat4("u_model", glm::value_ptr(mesh->GetModelMatrix()));
                // 视图矩阵 世界空间->摄影机空间
                shader->setMat4("u_view", glm::value_ptr(camera->GetViewMatrix()));
                shader->setMat4("u_projection", glm::value_ptr(camera->GetProjectionMatrix()));
                // normal matrix
                auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->GetModelMatrix())));
                shader->setMat3("u_normalMatrix", glm::value_ptr(normalMatrix));
                // 光源参数
                shader->setFloatVec3("u_lightDirection", directionalLight->m_direction);
                shader->setFloatVec3("u_lightColor", directionalLight->m_color);
                // 高光反射强度
                shader->setFloat("u_specularIntensity", directionalLight->m_specularIntensity);
                // 环境光
                shader->setFloatVec3("u_ambientColor", ambientLight->m_color);
                // 控制高光反射光斑大小
                shader->setFloat("u_shiness", curMaterial->m_shiness);
                // 相机位置
                shader->setFloatVec3("u_cameraPos", camera->m_Position);
            }
            break;
            default:
                continue;
        }
        // 绑定VAO
        glBindVertexArray(geometry->GetVAO());
        // 执行绘制指令
        glDrawElements(GL_TRIANGLES, geometry->GetIndicesCnt(), GL_UNSIGNED_INT, 0);
        // 解绑VAO
        glBindVertexArray(0);
        shader->Unbind();
    }
}
Shader* Renderer::getShader(MaterialType type) const
{
    Shader* ret = nullptr;
    switch (type)
    {
        case MaterialType::PhoneMaterial:
            ret = m_phoneShader;
            break;
        default:
            break;
    }
    return ret;
}
