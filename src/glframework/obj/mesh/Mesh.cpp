//
// Created by dingrui on 25-7-23.
//

#include "glframework/obj/mesh/Mesh.h"

#include "glframework/Shader.h"
#include "glframework/geo/Geometry.h"
#include "glframework/material/Material.h"

Mesh::Mesh(std::unique_ptr<Geometry> geometry, std::unique_ptr<Material> material)
    : Object(ObjectType::kMesh), m_geometry(std::move(geometry)), m_material(std::move(material))
{
}

void Mesh::Render(const Renderer& renderer, const Camera& camera, const LightPack& lights)
{
    // depth
    if (this->m_material->get_depthTest())
    {
        // 设置当前帧绘制的必要gl状态机参数 开启deep testing 不开启深度缓存的话后绘制的会覆盖先绘制的
        glEnable(GL_DEPTH_TEST);
        // 设置深度测试方法
        glDepthFunc(this->m_material->get_depthFunc());
    }
    else
    {
        glDisable(GL_DEPTH_TEST);
    }
    if (this->m_material->get_depthWrite())
    {
        glDepthMask(GL_TRUE);
    }
    else
    {
        glDepthMask(GL_FALSE);
    }
    // polygon offset
    if (m_material->get_enablePolygonOffset())
    {
        glEnable(m_material->get_polygonOffsetType());
        glPolygonOffset(m_material->get_polygonFactor(), m_material->get_polygonUnit());
    }
    else
    {
        glDisable(GL_POLYGON_OFFSET_FILL);
        glDisable(GL_POLYGON_OFFSET_LINE);
    }
    // stencil
    if (m_material->get_stencilTest())
    {
        glEnable(GL_STENCIL_TEST);
        glStencilOp(m_material->get_sFail(), m_material->get_zFail(), m_material->get_zPass());
        glStencilMask(m_material->get_stencilMask());
        glStencilFunc(m_material->get_stencilFunc(), m_material->get_stencilRef(), m_material->get_stencilFuncMask());
    }
    else
    {
        glDisable(GL_STENCIL_TEST);
    }
    // color blend
    if (m_material->get_enableBlend())
    {
        glEnable(GL_BLEND);
        glBlendFunc(m_material->get_srcBlendFactor(), m_material->get_destBlendFactor());
    }
    else
    {
        glDisable(GL_BLEND);
    }
    // face culling
    if (m_material->get_enableFaceCull())
    {
        glEnable(GL_CULL_FACE);
        glFrontFace(m_material->get_faceCull_front());
        glCullFace(m_material->get_faceCull_cull());
    }
    else
    {
        glDisable(GL_CULL_FACE);
    }

    // 用哪个shader
    Shader& shader = m_material->get_shader();
    // 更新shader的uniform变量
    shader.Bind();
    m_material->ApplyUniforms(shader, *this, camera, lights);
    // 绑定VAO
    glBindVertexArray(m_geometry->get_VAO());
    // 执行绘制指令
    glDrawElements(GL_TRIANGLES, m_geometry->get_IndicesCnt(), GL_UNSIGNED_INT, 0);
    // 解绑VAO
    glBindVertexArray(0);
    shader.Unbind();
}
