//
// Created by dingrui on 25-7-23.
//

#pragma once

class Shader;
class Renderer;
class Mesh;
class Material;
class Camera;
struct LightPack;

/**
 * 材质描述了物体表面如何与光发生反应
 * <ul>
 *   <li>漫反射 可以代表本身的固有颜色 固有颜色分2种
 *     <ul>
 *       <li>物体顶点颜色+插值</li>
 *       <li>使用像素uv采样贴图</li>
 *     </ul>
 *   </li>
 *   <li>高光反射 光斑大小调整</li>
 *   <li>环境光效果 设置为物体贴图颜色</li>
 * </ul>
 */
enum class MaterialType
{
    // 冯氏光照模型
    PhoneMaterial,
    // 纯白 用于点光
    WhiteMaterial,
    // 点光
    // PointLightMaterial,
    // 聚光灯
    // SpotLightMaterial,
};

// renderer只负责渲染 material负责选择shader/绑定纹理/上传uniform
class Material
{
public:
    virtual ~Material() = default;

    MaterialType type() const noexcept { return m_type; };

    // 用哪个shader交给具体材质去决定
    virtual Shader& shader(const Renderer& renderer) const = 0;
    // 材质负责上传uniform renderer只负责渲染
    virtual void applyUniforms(Shader& shader, const Mesh& mesh, const Camera& camera,
                               const LightPack& lights) const = 0;

protected:
    explicit Material(MaterialType type) noexcept : m_type(type) {}

private:
    const MaterialType m_type;
};
