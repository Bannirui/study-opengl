//
// Created by dingrui on 25-7-23.
//

#include "glframework/renderer/Renderer.h"

#include "application/camera/Camera.h"
#include "glframework/Mesh.h"
#include "glframework/Shader.h"
#include "glframework/Texture.h"
#include "glframework/geo/Geometry.h"
#include "glframework/material/Material.h"
#include "glframework/material/PhongMaterial.h"
#include "glframework/renderer/light_pack.h"

Renderer::Renderer()
    : m_phoneShader(std::make_unique<Shader>("asset/shader/phone_shader.glsl")),
      m_whiteShader(std::make_unique<Shader>("asset/shader/white_shader.glsl"))
{
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

void Renderer::setClearColor(glm::vec3 color)
{
    glClearColor(color.r, color.g, color.b, 1.0f);
}

void Renderer::beginFrame()
{
    // 设置当前帧绘制的必要gl状态机参数 开启deep testing 不开启深度缓存的话
    // 后绘制的会覆盖先绘制的
    glEnable(GL_DEPTH_TEST);
    // 设置深度测试方法
    glDepthFunc(GL_LESS);
    // 清理画布 每一帧都要清屏 防止残留前一帧图像 清理的时候也要清理深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Renderer::render(const std::vector<Mesh*>& meshes, Camera* camera, const LightPack& lights) const
{
    // 遍历mesh绘制
    for (int i = 0, sz = meshes.size(); i < sz; i++)
    {
        Mesh*           mesh     = meshes[i];
        const Geometry& geometry = mesh->getGeometry();
        const Material& material = mesh->getMaterial();
        // 用哪个shader
        Shader& shader = material.shader(*this);
        // 更新shader的uniform变量
        shader.Bind();
        material.applyUniforms(shader, *mesh, *camera, lights);
        // 绑定VAO
        glBindVertexArray(geometry.GetVAO());
        // 执行绘制指令
        glDrawElements(GL_TRIANGLES, geometry.GetIndicesCnt(), GL_UNSIGNED_INT, 0);
        // 解绑VAO
        glBindVertexArray(0);
        shader.Unbind();
    }
}

void Renderer::render(const std::shared_ptr<Object>& object, Camera* camera, const LightPack& lights) const
{
    // object是mesh还是其他具体类型 renderer不再需要感知 直接把渲染动作转交给object就行
    object->render(*this, *camera, lights);
    if (object->GetType() == ObjectType::Mesh)
    {
    }
    // 递归子节点
    for (const auto& child : object->GetChildren())
    {
        render(child, camera, lights);
    }
}