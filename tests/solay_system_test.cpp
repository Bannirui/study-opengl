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
    Object* sunPtr = scene.AddChild(std::move(sun));
    // 地球轨道
    auto earthOrbit = std::make_unique<Object>(ObjectType::Object);
    earthOrbit->set_position(glm::vec3(0.0f)); // 以太阳为中心
    Object* earthOrbitPtr = sunPtr->AddChild(std::move(earthOrbit));
    // 地球
    auto earthGeo = std::make_unique<Sphere>(0.2f);
    auto earthMat = std::make_unique<PhongMaterial>();
    Texture earthTex("asset/texture/earth.jpg", 0);
    earthMat->set_diffuse(&earthTex);
    earthMat->set_shines(32.0f);
    auto earth = std::make_unique<Mesh>(std::move(earthGeo), std::move(earthMat));
    earth->set_position(glm::vec3(1.5f, 0.0f, 0.0f)); // 距离太阳
    Object* earthPtr = earthOrbitPtr->AddChild(std::move(earth));
    // 月球轨道
    auto moonOrbit = std::make_unique<Object>(ObjectType::Object);
    Object* moonOrbitPtr = earthPtr->AddChild(std::move(moonOrbit));
    // 月球
    auto moonGeo = std::make_unique<Sphere>(0.05f);
    auto moonMat = std::make_unique<PhongMaterial>();
    Texture moonTex("asset/texture/moon.jpg", 0);
    moonMat->set_diffuse(&moonTex);
    auto moon = std::make_unique<Mesh>(std::move(moonGeo), std::move(moonMat));
    moon->set_position(glm::vec3(0.4f, 0.0f, 0.0f));
    moonOrbitPtr->AddChild(std::move(moon));

    // 光线
    struct LightPack lights;
    lights.directional = std::make_unique<DirectionalLight>();
    lights.directional->set_direction(glm::vec3(-1.0f, -1.0f, -10.f));
    lights.ambient = std::make_unique<AmbientLight>();
    lights.ambient->set_color(glm::vec3(0.2f));
    lights.point = std::make_unique<PointLight>();
    lights.point->set_position(glm::vec3(0.0f));
    lights.point->set_specular_intensity(1.0f);
    lights.point->m_kc = 1.0f;
    lights.point->m_k1 = 0.0f;
    lights.point->m_k2 = 0.0f; // 不衰减更像太阳

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

        // float time = glfwGetTime();
        // 地球公转
        earthOrbitPtr->set_rotationY(10.0f);
        // 地球自转
        earthPtr->set_rotationY(60.0f);
        // 月球公转
        moonOrbitPtr->set_rotationY(40.0f);
        // 太阳自转
        sunPtr->set_rotationY(5.0f);

        // 每一帧清一次屏
        Renderer::BeginFrame();
        render.Render(scene, camera, lights);

        glApp->RenderImGui();
    }
    return 0;
}
