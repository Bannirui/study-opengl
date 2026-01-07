//
// Created by dingrui on 25-7-23.
//

#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "glframework/Object.h"
// Shader资源用了智能指针管理 就不能简单用前向声明了 必须引用头文件 让编译器明确知道类的结构
#include "light_pack.h"
#include "glframework/Shader.h"

class Mesh;
class Camera;
enum class MaterialType;
struct LightPack;

// 渲染相关
class Renderer
{
public:
    Renderer();
    ~Renderer() = default;

    /**
     * 根据不同材质类型选择不同的Shader
     * @param type 材质类型
     * @return 适配的shader
     * 接口的返回值类型设计成引用比较合理
     * 1 引用表示的是非空借用
     * 2 不能是智能指针 会导致资源所有权转移
     * 3 指针类型什么时候用 当可能不存在的时候用指针的nullptr语义
     */
    Shader& getShader(const MaterialType type) const;

    void setClearColor(glm::vec3 color);

    void beginFrame();
    /**
     * 每次调用都会渲染一帧
     * 只要给renderer相机和光源就行
     * @param meshes 要渲染的物体对象
     * @param camera 相机 需要知道从哪儿看的
     * @param lights 光源 至于具体的光源是啥不用关注
     */
    void render(const std::vector<Mesh*>& meshes, Camera* camera, const LightPack& lights) const;
    void render(const std::shared_ptr<Object>& object, Camera* camera, const LightPack& lights) const;

private:
    // 生成多种不同的shader 根据材质类型挑选合适的shader
    // 冯氏光照 平行光
    std::unique_ptr<Shader> m_phoneShader;
    // 纯白色渲染
    std::unique_ptr<Shader> m_whiteShader;
};
