//
// Created by dingrui on 25-7-23.
//

#pragma once
#include <memory>

#include "glframework/Shader.h"

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
// renderer只负责渲染 material负责选择shader/绑定纹理/上传uniform
class Material {
public:
    virtual ~Material() = default;

    // 用哪个shader交给具体材质去决定
    // 返回值设计成引用的考量
    // 1 不暴露资源所有权
    // 2 引用表示的是非空借用
    virtual Shader &get_shader() const { return *this->m_shader; }

    // 材质负责上传uniform给shader 渲染动作发起只由renderer负责
    virtual void ApplyUniforms(Shader &shader, const Mesh &mesh, const Camera &camera,
                               const LightPack &lights) const = 0;

    bool get_depth_test() const { return m_enableDepthTest; }
    void set_depth_test(const bool option) { m_enableDepthTest = option; }
    GLenum get_depth_func() const { return m_depthFunc; }
    void set_depth_func(const GLenum depth_func) { m_depthFunc = depth_func; }
    bool get_depth_write() const { return m_depthWrite; }
    void set_depth_write(const bool option) { m_depthWrite = option; }

    bool get_enablePolygonOffset() const { return m_enablePolygonOffset; }
    void set_enablePolygonOffset(bool option) { m_enablePolygonOffset = option; }
    uint32_t get_polygonOffsetType() const { return m_polygonOffsetType; }
    void set_polygonOffsetType(uint32_t type) { m_polygonOffsetType = type; }
    float get_polygonFactor() const { return m_polygonFactor; }
    void set_polygonFactor(float factor) { m_polygonFactor = factor; }
    float get_polygonUnit() const { return m_polygonUnit; }
    void set_polygonUnit(float unit) { m_polygonUnit = unit; }

protected:
    explicit Material(const std::shared_ptr<Shader> &shader) : m_shader(shader) {
    }

    // todo 考虑到单个材质可能不止一个shader 所以shader成员暂时放在基类里面 以后需要扩展了可能需要下沉下子类
    std::shared_ptr<Shader> m_shader;

    // 设置当前帧绘制的必要gl状态机参数 开启deep testing 不开启深度缓存的话后绘制的会覆盖先绘制的
    bool m_enableDepthTest{true};
    // 设置深度测试方法
    GLenum m_depthFunc{GL_FALSE};
    bool m_depthWrite{true};

    // polygon offset
    bool m_enablePolygonOffset{false};
    uint32_t m_polygonOffsetType{GL_POLYGON_OFFSET_FILL};
    float m_polygonFactor{1.0f};
    float m_polygonUnit{1.0f};
};
