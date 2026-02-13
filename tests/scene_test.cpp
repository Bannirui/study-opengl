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
#include "glframework/geo/Box.h"
#include "glframework/geo/Sphere.h"
#include "glframework/light/AmbientLight.h"
#include "glframework/light/DirectionalLight.h"
#include "glframework/light/PointLight.h"
#include "glframework/light/SpotLight.h"
#include "glframework/material/PhongMaterial.h"
#include "glframework/material/WhiteMaterial.h"
#include "glframework/renderer/Renderer.h"
#include "glframework/renderer/light_pack.h"
#include "input/input.h"

int main() {
    if (!glApp->Init(1600, 800)) { return -1; }
    // 渲染器
    Renderer renderer;
    // 场景
    Scene scene;
    // 箱子
    std::unique_ptr<Box> geometryA = std::make_unique<Box>();
    std::unique_ptr<PhongMaterial> materialA = std::make_unique<PhongMaterial>();
    materialA->set_shines(32.0f);
    Texture boxDiffuse("asset/texture/box.png", 0);
    materialA->set_diffuse(&boxDiffuse);
    Texture boxSpecularMask("asset/texture/sp_mask.png", 1);
    materialA->set_specular_mask(&boxSpecularMask);
    std::unique_ptr<Mesh> meshA = std::make_unique<Mesh>(std::move(geometryA), std::move(materialA));
    meshA->set_rotationY(-15.0f);
    meshA->set_rotationX(15.0f);
    scene.AddChild(std::move(meshA));
    // 白色物体
    std::unique_ptr<Sphere> geometryB = std::make_unique<Sphere>(0.1f);
    std::unique_ptr<WhiteMaterial> materialB = std::make_unique<WhiteMaterial>();
    std::unique_ptr<Mesh> meshB = std::make_unique<Mesh>(std::move(geometryB), std::move(materialB));
    meshB->set_position(glm::vec3(2.0f, 0.0f, 0.0f));
    Object* meshBRaw = scene.AddChild(std::move(meshB));

    // 光线
    std::unique_ptr<SpotLight> spotLight = std::make_unique<SpotLight>();
    spotLight->m_innerAngle = 15.0f;
    spotLight->m_outerAngle = 30.0f;
    spotLight->set_position(meshBRaw->get_position());

    std::unique_ptr<DirectionalLight> directionalLight = std::make_unique<DirectionalLight>();
    directionalLight->set_direction(glm::vec3(1.0f, 0.0f, 0.0f));

    std::unique_ptr<PointLight> pointLight = std::make_unique<PointLight>();
    pointLight->set_position(glm::vec3(0.0f, 0.0f, 1.5f));
    pointLight->set_specular_intensity(0.5f);
    pointLight->m_k2 = 0.017f;
    pointLight->m_k1 = 0.07f;
    pointLight->m_kc = 1.0f;

    std::unique_ptr<AmbientLight> ambientLight = std::make_unique<AmbientLight>();
    ambientLight->set_color(glm::vec3(0.2f));

    struct LightPack lights;
    lights.directional = std::move(directionalLight);
    lights.point = std::move(pointLight);
    lights.spot = std::move(spotLight);
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

        renderer.setClearColor(glApp->get_clearColor());
        // 每一帧清一次屏
        Renderer::BeginFrame();
        renderer.Render(scene, camera, lights);

        // imgui渲染
        glApp->RenderImGui();
    }
    return 0;
}
