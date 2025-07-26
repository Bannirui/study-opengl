//
// Created by dingrui on 25-7-23.
//

#pragma once

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
    PointLightMaterial,
    // 聚光灯
    SpotLightMaterial,
};

class Material
{
public:
    Material() {}
    virtual ~Material() = default;

public:
    MaterialType m_type;
};
