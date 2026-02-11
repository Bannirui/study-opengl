//
// Created by dingrui on 25-6-23.
//

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
    if (!glApp->Init(1600, 800)) return -1;
    // 渲染器
    std::shared_ptr<Renderer> renderer = std::make_shared<Renderer>();
    // 场景
    Scene scene;
    // 箱子
    std::shared_ptr<Box> boxGeometry = std::make_shared<Box>();
    std::shared_ptr<PhongMaterial> boxMaterial = std::make_shared<PhongMaterial>();
    boxMaterial->set_shines(32.0f);
    boxMaterial->set_diffuse(new Texture("asset/texture/box.png", 0));
    boxMaterial->set_specular_mask(new Texture("asset/texture/sp_mask.png", 1));
    std::unique_ptr<Mesh> boxMesh = std::make_unique<Mesh>(boxGeometry, boxMaterial);
    boxMesh->set_rotationY(-15.0f);
    boxMesh->set_rotationX(15.0f);
    // 白色物体
    std::shared_ptr<Sphere> whiteObjGeometry = std::make_shared<Sphere>(0.1f);
    std::shared_ptr<WhiteMaterial> whiteObjMaterial = std::make_shared<WhiteMaterial>();
    std::unique_ptr<Mesh> whiteObjMesh = std::make_unique<Mesh>(std::move(whiteObjGeometry),
                                                                std::move(whiteObjMaterial));
    whiteObjMesh->set_position(glm::vec3(2.0f, 0.0f, 0.0f));

    // todo
    boxMesh->AddChild(std::move(whiteObjMesh));
    scene.AddChild(std::move(boxMesh));

    // 光线
    std::shared_ptr<SpotLight> spotLight = std::make_shared<SpotLight>();
    spotLight->m_innerAngle = 15.0f;
    spotLight->m_outerAngle = 30.0f;
    spotLight->set_position(whiteObjMesh->get_position());

    std::shared_ptr<DirectionalLight> directionalLight = std::make_shared<DirectionalLight>();
    directionalLight->m_direction = glm::vec3(1.0f, 0.0f, 0.0f);

    std::shared_ptr<PointLight> pointLight = std::make_shared<PointLight>();
    pointLight->set_position(glm::vec3(0.0f, 0.0f, 1.5f));
    pointLight->set_specular_intensity(0.5f);
    pointLight->m_k2 = 0.017f;
    pointLight->m_k1 = 0.07f;
    pointLight->m_kc = 1.0f;

    std::shared_ptr<AmbientLight> ambientLight = std::make_shared<AmbientLight>();
    ambientLight->set_color(glm::vec3(0.2f));

    struct LightPack lights;
    lights.directional = directionalLight;
    lights.point = pointLight;
    lights.spot = spotLight;
    lights.ambient = ambientLight;

    // 相机
    PerspectiveCamera camera(static_cast<float>(glApp->get_width()) / static_cast<float>(glApp->get_height()));
    camera.set_position(glm::vec3(0.0f, 0.0f, 5.0f));
    // 相机控制器
    Input *input = glApp->get_input();
    input->CreateCameraController<TrackballCameraController>(camera);
    auto cameraCtl = input->get_CameraController();

    glApp->InitImGui();

    // 窗体循环
    while (glApp->Update()) {
        cameraCtl->OnUpdate();

        renderer->setClearColor(glApp->get_clearColor());
        // 每一帧清一次屏
        Renderer::BeginFrame();
        renderer->Render(scene, camera, lights);

        // imgui渲染
        glApp->RenderImGui();
    }
    return 0;
}
