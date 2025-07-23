//
// Created by dingrui on 25-7-23.
//

#pragma once

#include <vector>

#include "glframework/Core.h"

class Mesh;
class Camera;
class DirectionalLight;
class AmbientLight;
class Shader;
enum class MaterialType;

// 渲染相关
class Renderer
{
public:
    Renderer();
    ~Renderer();

    /**
     * 每次调用都会渲染一帧
     * @param meshes 要渲染的物体对象
     * @param camera 相机 需要知道从哪儿看的
     * @param directional 平行光
     * @param ambientLight 环境光
     */
    void render(const std::vector<Mesh*>& meshes, Camera* camera, DirectionalLight* directionalLight,
                AmbientLight* ambientLight);

private:
    /**
     * 根据不同材质类型选择不同的Shader
     * @param type 材质类型
     * @return 适配的shader
     */
    Shader* getShader(MaterialType type) const;

private:
    // 生成多种不同的shader 根据材质类型挑选合适的shader
    // 冯氏光照shader
    Shader* m_phoneShader{nullptr};
};
