//
// Created by dingrui on 25-7-24.
//

#include "glframework/material/DepthMaterial.h"

#include "glframework/Mesh.h"
#include "glframework/renderer/Renderer.h"
#include "application/camera/Camera.h"

DepthMaterial::DepthMaterial() : Material("asset/shader/depth_shader.glsl") {
}

void DepthMaterial::ApplyUniforms(Shader &shader, Mesh &mesh, const Camera &camera,
                                  const LightPack &lights) const {
    // mvp变换矩阵
    shader.setMat4("u_model", glm::value_ptr(mesh.GetModelMatrix()));
    shader.setMat4("u_view", glm::value_ptr(camera.GetViewMatrix()));
    shader.setMat4("u_projection", glm::value_ptr(camera.GetProjectionMatrix()));
    shader.setFloat("u_near", camera.get_near());
    shader.setFloat("u_far", camera.get_far());
}
