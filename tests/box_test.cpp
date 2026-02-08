//
// Created by dingrui on 25-6-23.
//

#include <iostream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "err_check.h"
#include "application/Application.h"
#include "application/camera/Camera.h"
#include "application/camera/CameraController.h"
#include "application/camera/PerspectiveCamera.h"
#include "application/camera/TrackballCameraController.h"
#include "glframework/Mesh.h"
#include "glframework/Texture.h"
#include "glframework/geo/Box.h"
#include "glframework/geo/Sphere.h"
#include "glframework/light/AmbientLight.h"
#include "glframework/light/DirectionalLight.h"
#include "glframework/light/PointLight.h"
#include "glframework/material/PhongMaterial.h"
#include "glframework/renderer/Renderer.h"
#include "glframework/renderer/light_pack.h"
#include "input/input_dispatcher.h"

int main() {
    if (!glApp->Init(1600, 800)) return -1;
    // 监听事件
    glApp->RegisterCallback();

    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();
    // 渲染列表
    std::vector<std::shared_ptr<Mesh> > meshes;
    // 箱子
    std::shared_ptr<Box> boxGeometry = std::make_shared<Box>();
    std::shared_ptr<PhongMaterial> boxMaterial = std::make_shared<PhongMaterial>();
    boxMaterial->set_shines(32.0f);
    boxMaterial->set_diffuse(new Texture("asset/texture/box.png", 0));
    boxMaterial->set_specular_mask(new Texture("asset/texture/sp_mask.png", 1));
    std::shared_ptr<Mesh> boxMesh = std::make_shared<Mesh>(boxGeometry, boxMaterial);
    meshes.push_back(boxMesh);
    // 光线
    std::shared_ptr<DirectionalLight> directionalLight = std::make_shared<DirectionalLight>();
    directionalLight->m_direction = glm::vec3(-1.0f, 0.0f, 0.0f);
    std::shared_ptr<AmbientLight> ambient_light = std::make_shared<AmbientLight>();
    ambient_light->set_color(glm::vec3(0.2f));
    struct LightPack lights;
    lights.directional = directionalLight;
    lights.ambient = ambient_light;
    // 相机
    PerspectiveCamera camera(static_cast<float>(glApp->get_width()) / static_cast<float>(glApp->get_height()));
    camera.set_position(glm::vec3(0.0f, 0.0f, 5.0f));
    // 相机控制器
    InputDispatcher inputDispatcher(glApp);
    inputDispatcher.CreateCameraController<TrackballCameraController>(camera);
    auto cameraCtl = inputDispatcher.get_CameraController();

    // 窗体循环
    while (glApp->Update()) {
        cameraCtl->OnUpdate();
        meshes[0]->SetRotationX(0.1f);
        meshes[0]->SetRotationY(0.05f);
        meshes[0]->SetRotationZ(0.01f);

        // 每一帧清一次屏
        Renderer::BeginFrame();
        renderer->render(meshes, camera, lights);
    }
    return 0;
}
