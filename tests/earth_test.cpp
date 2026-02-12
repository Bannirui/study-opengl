//
// Created by dingrui on 25-6-23.
//

#include <memory>

#include <glm/glm.hpp>

#include "err_check.h"
#include "application/Application.h"
#include "application/camera/Camera.h"
#include "application/camera/CameraController.h"
#include "application/camera/PerspectiveCamera.h"
#include "application/camera/TrackballCameraController.h"
#include "glframework/Mesh.h"
#include "glframework/Scene.h"
#include "glframework/Texture.h"
#include "glframework/geo/Sphere.h"
#include "glframework/light/AmbientLight.h"
#include "glframework/light/DirectionalLight.h"
#include "glframework/material/PhongMaterial.h"
#include "glframework/renderer/Renderer.h"
#include "glframework/renderer/light_pack.h"
#include "input/input.h"

int main() {
    if (!glApp->Init(1600, 800)) return -1;
    // 负责渲染
    Renderer render;
    // 物体
    Scene scene;
    // 球体
    std::unique_ptr<Sphere> ballGeometry = std::make_unique<Sphere>();
    std::unique_ptr<PhongMaterial> ballMaterial = std::make_unique<PhongMaterial>();
    ballMaterial->set_shines(10.0f);
    Texture ballDiffuse("asset/texture/wall.jpg", 0);
    ballMaterial->set_diffuse(&ballDiffuse);
    std::unique_ptr<Mesh> ballMesh = std::make_unique<Mesh>(std::move(ballGeometry), std::move(ballMaterial));
    scene.AddChild(std::move(ballMesh));
    // 地球
    std::unique_ptr<Sphere> earthGeometry = std::make_unique<Sphere>(1.0f);
    std::unique_ptr<PhongMaterial> earthMaterial = std::make_unique<PhongMaterial>();
    earthMaterial->set_shines(16.0f);
    Texture earthDiffuse("asset/texture/earth.jpg", 1);
    earthMaterial->set_diffuse(&earthDiffuse);
    std::unique_ptr<Mesh> earthMesh = std::make_unique<Mesh>(std::move(earthGeometry), std::move(earthMaterial));
    earthMesh->set_position(glm::vec3(2.5f, 0.0f, 0.0f));
    scene.AddChild(std::move(earthMesh));
    // 光线
    std::unique_ptr<DirectionalLight> directionalLight = std::make_unique<DirectionalLight>();
    directionalLight->set_direction(glm::vec3(-1.0f, -1.0f, -1.0f));
    std::unique_ptr<AmbientLight> ambientLight = std::make_unique<AmbientLight>();
    ambientLight->set_color(glm::vec3(0.2f));
    struct LightPack lights;
    lights.directional = std::move(directionalLight);
    lights.ambient = std::move(ambientLight);
    // 相机
    PerspectiveCamera camera(static_cast<float>(glApp->get_width()) / static_cast<float>(glApp->get_height()));
    camera.set_position(glm::vec3(0.0f, 0.0f, 5.0f));
    // 相机控制器
    Input *input = glApp->get_input();
    input->CreateCameraController<TrackballCameraController>(camera);
    auto cameraCtl = input->get_CameraController();

    // 窗体循环
    while (glApp->Update()) {
        cameraCtl->OnUpdate();
        scene.get_children()[1]->set_rotationY(0.2f);

        // 每一帧清一次屏
        Renderer::BeginFrame();
        render.Render(scene, camera, lights);
    }
    return 0;
}
