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
#include "glframework/light/PointLight.h"
#include "glframework/light/SpotLight.h"
#include "glframework/material/Material.h"
#include "glframework/material/PhongMaterial.h"

Renderer::Renderer()
    : m_phoneShader(std::make_unique<Shader>("asset/shader/phone_shader.glsl")),
      m_whiteShader(std::make_unique<Shader>("asset/shader/white_shader.glsl"))
{
}

void Renderer::setClearColor(glm::vec3 color)
{
    glClearColor(color.r, color.g, color.b, 1.0f);
}

void Renderer::render(const std::vector<Mesh*>& meshes, Camera* camera, DirectionalLight* directionalLight,
                      AmbientLight* ambientLight) const
{
    render(meshes, camera, directionalLight, nullptr, ambientLight, nullptr);
}
void Renderer::render(const std::vector<Mesh*>& meshes, Camera* camera, PointLight* pointLight,
                      AmbientLight* ambientLight) const
{
    render(meshes, camera, nullptr, pointLight, ambientLight, nullptr);
}
void Renderer::render(const std::vector<Mesh*>& meshes, Camera* camera, AmbientLight* ambientLight,
                      SpotLight* spotLight) const
{
    render(meshes, camera, nullptr, nullptr, ambientLight, spotLight);
}
void Renderer::render(const std::vector<Mesh*>& meshes, Camera* camera, DirectionalLight* directionalLight,
                      PointLight* pointLight, AmbientLight* ambientLight, SpotLight* spotLight) const
{
    // 设置当前帧绘制的必要gl状态机参数 开启deep testing 不开启深度缓存的话
    // 后绘制的会覆盖先绘制的
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
        Shader& shader = getShader(material->m_type);
        // 更新shader的uniform变量
        shader.Bind();
        switch (material->m_type)
        {
            case MaterialType::PhoneMaterial:
            {
                PhongMaterial* curMaterial = (PhongMaterial*)material;
                if (curMaterial->m_diffuse)
                {
                    // 将纹理对象跟纹理单元绑定
                    curMaterial->m_diffuse->Bind();
                    // diffuse贴图 将纹理采样器跟纹理单元绑定
                    shader.setInt("u_sampler", curMaterial->m_diffuse->GetUnit());
                }
                // 高光蒙版贴图
                if (curMaterial->m_specularMask)
                {
                    curMaterial->m_specularMask->Bind();
                    shader.setInt("u_specularMaskSampler", curMaterial->m_specularMask->GetUnit());
                }
                // 模型变换矩阵 aPos模型->世界空间
                shader.setMat4("u_model", glm::value_ptr(mesh->GetModelMatrix()));
                // 视图矩阵 世界空间->摄影机空间
                shader.setMat4("u_view", glm::value_ptr(camera->GetViewMatrix()));
                shader.setMat4("u_projection", glm::value_ptr(camera->GetProjectionMatrix()));
                // normal matrix
                auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->GetModelMatrix())));
                shader.setMat3("u_normalMatrix", glm::value_ptr(normalMatrix));
                // 平行光
                if (directionalLight)
                {
                    shader.setBool("u_activeDirectionalLight", true);
                    shader.setFloatVec3("u_directionalLight.direction", directionalLight->m_direction);
                    shader.setFloatVec3("u_directionalLight.color", directionalLight->m_color);
                    // 高光反射强度
                    shader.setFloat("u_directionalLight.specularIntensity", directionalLight->m_specularIntensity);
                }
                // 点光
                if (pointLight)
                {
                    shader.setBool("u_activePointLight", true);
                    shader.setFloatVec3("u_pointLight.pos", pointLight->GetPosition());
                    shader.setFloatVec3("u_pointLight.color", pointLight->m_color);
                    // 高光反射强度
                    shader.setFloat("u_pointLight.specularIntensity", pointLight->m_specularIntensity);
                    shader.setFloat(".u_pointLight.k2", pointLight->m_k2);
                    shader.setFloat("u_pointLight.k1", pointLight->m_k1);
                    shader.setFloat("u_pointLight.kc", pointLight->m_kc);
                }
                // 聚光灯
                if (spotLight)
                {
                    shader.setBool("u_activeSpotLight", true);
                    shader.setFloatVec3("u_spotLight.pos", spotLight->GetPosition());
                    shader.setFloatVec3("u_spotLight.targetDirection", spotLight->m_targetDirection);
                    shader.setFloatVec3("u_spotLight.color", spotLight->m_color);
                    shader.setFloat("u_spotLight.innerCos", glm::cos(glm::radians(spotLight->m_innerAngle)));
                    shader.setFloat("u_spotLight.outerCos", glm::cos(glm::radians(spotLight->m_outerAngle)));
                    shader.setFloat("u_spotLight.specularIntensity", spotLight->m_specularIntensity);
                }
                // 环境光
                if (ambientLight)
                {
                    shader.setFloatVec3("u_ambientColor", ambientLight->m_color);
                }
                // 控制高光反射光斑大小
                shader.setFloat("u_shiness", curMaterial->m_shiness);
                // 相机位置
                shader.setFloatVec3("u_cameraPos", camera->m_Position);
            }
            break;
            case MaterialType::WhiteMaterial:
            {
                // mvp变换矩阵
                shader.setMat4("u_model", glm::value_ptr(mesh->GetModelMatrix()));
                shader.setMat4("u_view", glm::value_ptr(camera->GetViewMatrix()));
                shader.setMat4("u_projection", glm::value_ptr(camera->GetProjectionMatrix()));
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
        shader.Unbind();
    }
}
void Renderer::render(const Object* object, Camera* camera, DirectionalLight* directionalLight, PointLight* pointLight,
                      AmbientLight* ambientLight, SpotLight* spotLight) const
{
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderObject(object, camera, directionalLight, pointLight, ambientLight, spotLight);
}
void Renderer::renderObject(const Object* object, Camera* camera, DirectionalLight* directionalLight,
                            PointLight* pointLight, AmbientLight* ambientLight, SpotLight* spotLight) const
{
    // object是mesh还是object
    if (object->GetType() == ObjectType::Mesh)
    {
        auto mesh     = (Mesh*)object;
        auto geometry = mesh->m_geometry;
        auto material = mesh->m_material;
        // 用哪个shader
        Shader& shader = getShader(material->m_type);
        // 更新shader的uniform变量
        shader.Bind();
        switch (material->m_type)
        {
            case MaterialType::PhoneMaterial:
            {
                PhongMaterial* curMaterial = (PhongMaterial*)material;
                if (curMaterial->m_diffuse)
                {
                    // 将纹理对象跟纹理单元绑定
                    curMaterial->m_diffuse->Bind();
                    // diffuse贴图 将纹理采样器跟纹理单元绑定
                    shader.setInt("u_sampler", curMaterial->m_diffuse->GetUnit());
                }
                // 高光蒙版贴图
                if (curMaterial->m_specularMask)
                {
                    curMaterial->m_specularMask->Bind();
                    shader.setInt("u_specularMaskSampler", curMaterial->m_specularMask->GetUnit());
                }
                // 模型变换矩阵 aPos模型->世界空间
                shader.setMat4("u_model", glm::value_ptr(mesh->GetModelMatrix()));
                // 视图矩阵 世界空间->摄影机空间
                shader.setMat4("u_view", glm::value_ptr(camera->GetViewMatrix()));
                shader.setMat4("u_projection", glm::value_ptr(camera->GetProjectionMatrix()));
                // normal matrix
                auto normalMatrix = glm::mat3(glm::transpose(glm::inverse(mesh->GetModelMatrix())));
                shader.setMat3("u_normalMatrix", glm::value_ptr(normalMatrix));
                // 平行光
                if (directionalLight)
                {
                    shader.setBool("u_activeDirectionalLight", true);
                    shader.setFloatVec3("u_directionalLight.direction", directionalLight->m_direction);
                    shader.setFloatVec3("u_directionalLight.color", directionalLight->m_color);
                    // 高光反射强度
                    shader.setFloat("u_directionalLight.specularIntensity", directionalLight->m_specularIntensity);
                }
                // 点光
                if (pointLight)
                {
                    shader.setBool("u_activePointLight", true);
                    shader.setFloatVec3("u_pointLight.pos", pointLight->GetPosition());
                    shader.setFloatVec3("u_pointLight.color", pointLight->m_color);
                    // 高光反射强度
                    shader.setFloat("u_pointLight.specularIntensity", pointLight->m_specularIntensity);
                    shader.setFloat(".u_pointLight.k2", pointLight->m_k2);
                    shader.setFloat("u_pointLight.k1", pointLight->m_k1);
                    shader.setFloat("u_pointLight.kc", pointLight->m_kc);
                }
                // 聚光灯
                if (spotLight)
                {
                    shader.setBool("u_activeSpotLight", true);
                    shader.setFloatVec3("u_spotLight.pos", spotLight->GetPosition());
                    shader.setFloatVec3("u_spotLight.targetDirection", spotLight->m_targetDirection);
                    shader.setFloatVec3("u_spotLight.color", spotLight->m_color);
                    shader.setFloat("u_spotLight.innerCos", glm::cos(glm::radians(spotLight->m_innerAngle)));
                    shader.setFloat("u_spotLight.outerCos", glm::cos(glm::radians(spotLight->m_outerAngle)));
                    shader.setFloat("u_spotLight.specularIntensity", spotLight->m_specularIntensity);
                }
                // 环境光
                if (ambientLight)
                {
                    shader.setFloatVec3("u_ambientColor", ambientLight->m_color);
                }
                // 控制高光反射光斑大小
                shader.setFloat("u_shiness", curMaterial->m_shiness);
                // 相机位置
                shader.setFloatVec3("u_cameraPos", camera->m_Position);
            }
            break;
            case MaterialType::WhiteMaterial:
            {
                // mvp变换矩阵
                shader.setMat4("u_model", glm::value_ptr(mesh->GetModelMatrix()));
                shader.setMat4("u_view", glm::value_ptr(camera->GetViewMatrix()));
                shader.setMat4("u_projection", glm::value_ptr(camera->GetProjectionMatrix()));
            }
            break;
            default:
                break;
        }
        // 绑定VAO
        glBindVertexArray(geometry->GetVAO());
        // 执行绘制指令
        glDrawElements(GL_TRIANGLES, geometry->GetIndicesCnt(), GL_UNSIGNED_INT, 0);
        // 解绑VAO
        glBindVertexArray(0);
        shader.Unbind();
    }
    // 递归子节点
    auto children = object->GetChildren();
    for (int i = 0, sz = children.size(); i < sz; i++)
    {
        renderObject(children[i], camera, directionalLight, pointLight, ambientLight, spotLight);
    }
}
Shader& Renderer::getShader(const MaterialType type) const
{
    assert(m_phoneShader && m_whiteShader);
    switch (type)
    {
        case MaterialType::PhoneMaterial:
            return *m_phoneShader;
        case MaterialType::WhiteMaterial:
            return *m_whiteShader;
    }
    std::terminate();
}
