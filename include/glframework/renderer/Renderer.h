//
// Created by dingrui on 25-7-23.
//

#pragma once

#include <vector>

class Mesh;
class Camera;
class DirectionalLight;
class AmbientLight;
class PointLight;
class SpotLight;
class Shader;
enum class MaterialType;

// 渲染相关
class Renderer
{
public:
    Renderer();
    ~Renderer();

public:
    // 平行光
    void render(const std::vector<Mesh*>& meshes, Camera* camera, DirectionalLight* directionalLight,
                AmbientLight* ambientLight) const;
    // 点光
    void render(const std::vector<Mesh*>& meshes, Camera* camera, PointLight* pointLight,
                AmbientLight* ambientLight) const;
    // 探照光
    void render(const std::vector<Mesh*>& meshes, Camera* camera, AmbientLight* ambientLight,
                SpotLight* spotLight) const;

private:
    /**
     * 根据不同材质类型选择不同的Shader
     * @param type 材质类型
     * @return 适配的shader
     */
    Shader* getShader(const MaterialType type) const;
    /**
     * 每次调用都会渲染一帧
     * @param meshes 要渲染的物体对象
     * @param camera 相机 需要知道从哪儿看的
     * @param directionalLight 平行光
     * @param pointLight 点光
     * @param ambientLight 环境光
     * @param spotLight 聚光光
     */
    void render(const std::vector<Mesh*>& meshes, Camera* camera, DirectionalLight* directionalLight,
                PointLight* pointLight, AmbientLight* ambientLight, SpotLight* spotLight) const;

private:
    // 生成多种不同的shader 根据材质类型挑选合适的shader
    // 冯氏光照 平行光
    Shader* m_phoneShader{nullptr};
    // 纯白色渲染
    Shader* m_whiteShader{nullptr};
    // 点光
    Shader* m_pointLightShader{nullptr};
    // 聚光灯
    Shader* m_spotLightShader{nullptr};
};
