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
    virtual void ApplyUniforms(Shader &shader, Mesh &mesh, const Camera &camera,
                               const LightPack &lights) const = 0;

    bool get_depthTest() const { return m_enableDepthTest; }
    void set_depthTest(const bool option) { m_enableDepthTest = option; }
    GLenum get_depthFunc() const { return m_depthFunc; }
    void set_depthFunc(const GLenum depth_func) { m_depthFunc = depth_func; }
    bool get_depthWrite() const { return m_depthWrite; }
    void set_depthWrite(const bool option) { m_depthWrite = option; }

    bool get_enablePolygonOffset() const { return m_enablePolygonOffset; }
    void set_enablePolygonOffset(bool option) { m_enablePolygonOffset = option; }
    uint32_t get_polygonOffsetType() const { return m_polygonOffsetType; }
    void set_polygonOffsetType(uint32_t type) { m_polygonOffsetType = type; }
    float get_polygonFactor() const { return m_polygonFactor; }
    void set_polygonFactor(float factor) { m_polygonFactor = factor; }
    float get_polygonUnit() const { return m_polygonUnit; }
    void set_polygonUnit(float unit) { m_polygonUnit = unit; }

    bool get_stencilTest() const { return m_enableStencilTest; }
    void set_stencilTest(bool option) { m_enableStencilTest = option; }
    uint32_t get_sFail() const { return m_sFail; }
    void set_sFail(uint32_t val) { m_sFail = val; }
    uint32_t get_zFail() const { return m_zFail; }
    void set_zFail(uint32_t val) { m_zFail = val; }
    uint32_t get_zPass() const { return m_zPass; }
    void set_zPass(uint32_t val) { m_zPass = val; }
    uint32_t get_stencilMask() const { return m_stencilMask; }
    void set_stencilMask(uint32_t val) { m_stencilMask = val; }
    uint32_t get_stencilFunc() const { return m_stencilFunc; }
    void set_stencilFunc(uint32_t func) { m_stencilFunc = func; }
    int get_stencilRef() const { return m_stencilRef; }
    void set_stencilRef(int ref) { m_stencilRef = ref; }
    uint32_t get_stencilFuncMask() const { return m_stencilFuncMask; }
    void set_stencilFuncMask(uint32_t mask) { m_stencilFuncMask = mask; }

    bool get_enableBlend() const { return m_enableBlend; }
    void set_enableBlend(bool option) { m_enableBlend = option; }
    uint32_t get_srcBlendFactor() const { return m_sBlendFactor; }
    void set_srcBlendFactor(uint32_t factor) { m_sBlendFactor = factor; }
    uint32_t get_destBlendFactor() const { return m_dBlendFactor; }
    void set_destBlendFactor(uint32_t factor) { m_dBlendFactor = factor; }

    float get_opacity() const { return m_opacity; }
    void set_opacity(float val) { this->m_opacity = val; }

    bool get_enableFaceCull() const { return m_enableFaceCull; }
    void set_enableFaceCull(bool option) { m_enableFaceCull = option; }
    uint32_t get_faceCull_front() const { return m_faceCull_front; }
    void set_faceCull_front(uint32_t front) { m_faceCull_front = front; }
    uint32_t get_faceCull_cull() const { return m_faceCull_cull; }
    void set_faceCull_cull(uint32_t which) { m_faceCull_cull = which; }

protected:
    Material(const std::string &shaderPath) : m_shader(std::make_unique<Shader>(shaderPath)) {
    }

    // todo 考虑到单个材质可能不止一个shader 所以shader成员暂时放在基类里面 以后需要扩展了可能需要下沉下子类
    std::unique_ptr<Shader> m_shader;

    // 设置当前帧绘制的必要gl状态机参数 开启deep testing 不开启深度缓存的话后绘制的会覆盖先绘制的
    bool m_enableDepthTest{true};
    GLenum m_depthFunc{GL_FALSE}; // 设置深度测试方法
    bool m_depthWrite{true};

    // polygon offset
    bool m_enablePolygonOffset{false};
    uint32_t m_polygonOffsetType{GL_POLYGON_OFFSET_FILL};
    float m_polygonFactor{1.0f};
    float m_polygonUnit{1.0f};

    // stencil
    bool m_enableStencilTest{false};
    uint32_t m_sFail{GL_KEEP}; // stencil fail
    uint32_t m_zFail{GL_KEEP}; // stencil pass+depth fail
    uint32_t m_zPass{GL_KEEP}; // stencil pass+depth pass
    uint32_t m_stencilMask{0xff}; // control stencil write
    uint32_t m_stencilFunc{GL_ALWAYS};
    int m_stencilRef{0};
    uint32_t m_stencilFuncMask{0xff};

    // color blend
    bool m_enableBlend{false};
    uint32_t m_sBlendFactor{GL_SRC_ALPHA}; // src factor
    uint32_t m_dBlendFactor{GL_ONE_MINUS_SRC_ALPHA}; // dest factor

    // opacity
    float m_opacity{1.0f}; // 0 is opacity, 1 is non-opacity

    // face culling
    bool m_enableFaceCull{false};
    uint32_t m_faceCull_front{GL_CCW}; // define which is front, clock-wise or counter-clock-wise
    uint32_t m_faceCull_cull{GL_BACK}; // cull front or back
};
