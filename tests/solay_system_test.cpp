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
    if (!glApp->Init(1600, 800)) { return -1; }
    Renderer render;
    Scene scene;
    // 太阳
    auto sunGeo = std::make_unique<Sphere>(0.5f);
    auto sunMat = std::make_unique<PhongMaterial>();
    Texture sunTex("asset/texture/sun.jpg", 0);
    sunMat->set_diffuse(&sunTex);
    sunMat->set_shines(8.0f);
    auto sun = std::make_unique<Mesh>(std::move(sunGeo), std::move(sunMat));
    scene.AddChild(std::move(sun));

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
        render.setClearColor(glApp->get_clearColor());
        cameraCtl->OnUpdate();

        // 每一帧清一次屏
        Renderer::BeginFrame();
        render.Render(scene, camera, lights);

        glApp->RenderImGui();
    }
    return 0;
}
