//
// Created by dingrui on 25-7-23.
//

#pragma once

#include <vector>

#include <glm/glm.hpp>

#include "glframework/Object.h"

class Mesh;
class Camera;
enum class MaterialType;
struct LightPack;

// 渲染相关
class Renderer {
public:
    Renderer() = default;

    ~Renderer() = default;

    void setClearColor(glm::vec3 color);

    // 绘画的时候只管一帧 在递归过程中误用会导致渲染不全
    static void BeginFrame();

    /**
     * 每次调用都会渲染一帧
     * 只要给renderer相机和光源就行
     * @param meshes 要渲染的物体对象
     * @param camera 相机 需要知道从哪儿看的
     * @param lights 光源 至于具体的光源是啥不用关注
     */
    void render(const std::vector<std::shared_ptr<Mesh> > &meshes, const Camera &camera, const LightPack &lights) const;

    // render entire scene from root, and the children
    void render(Object &object, const Camera &camera, const LightPack &lights) const;

    // render to specific FBO
    void Render(Object &object, const Camera &camera, const LightPack &lights, uint32_t fbo) const;
};
