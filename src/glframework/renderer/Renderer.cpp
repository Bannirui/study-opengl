//
// Created by dingrui on 25-7-23.
//

#include "glframework/renderer/Renderer.h"

#include "application/camera/Camera.h"
#include "glframework/Mesh.h"
#include "glframework/Shader.h"
#include "glframework/Texture.h"
#include "glframework/material/Material.h"

void Renderer::setClearColor(glm::vec3 color) {
    glClearColor(color.r, color.g, color.b, 1.0f);
}

void Renderer::BeginFrame() {
    // must open all the depth options in advance, guarantee glClean command execute succ
    // depth test
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glDepthMask(GL_TRUE);

    // polygon
    glDisable(GL_POLYGON_OFFSET_FILL);
    glDisable(GL_POLYGON_OFFSET_LINE);

    // stencil
    glEnable(GL_STENCIL_TEST);
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilMask(0xff);

    // color blend, default is closed
    glDisable(GL_BLEND);

    // 清理画布 每一帧都要清屏 防止残留前一帧图像 清理的时候也要清理深度缓存
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
}

void Renderer::Render(Object &object, const Camera &camera, const LightPack &lights) const {
    // object是mesh还是其他具体类型 renderer不再需要感知 直接把渲染动作转交给object就行
    object.Render(*this, camera, lights);
    // 递归子节点
    for (const auto &child: object.get_children()) {
        Render(*child, camera, lights);
    }
}

void Renderer::Render(Object &object, const Camera &camera, const LightPack &lights,
                      uint32_t fbo) const {
    // active FBO
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    Render(object, camera, lights);
    // de-active FBO
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
