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
#include "glframework/Scene.h"

// 聚光灯 平行光 点光
int main() {
    if (!glApp->Init(1600, 800)) { return -1; }
    // 渲染器
    Renderer renderer;
    // 渲染列表
    Scene scene;
    // 箱子
    std::unique_ptr<Box> boxGeometry = std::make_unique<Box>();
    std::unique_ptr<PhongMaterial> boxMaterial = std::make_unique<PhongMaterial>();
    boxMaterial->set_shines(32.0f);
    Texture boxDiffuse("asset/texture/box.png", 0);
    boxMaterial->set_diffuse(&boxDiffuse);
    Texture boxSpecularMask("asset/texture/sp_mask.png", 1);
    boxMaterial->set_specular_mask(&boxSpecularMask);
    std::unique_ptr<Mesh> boxMesh = std::make_unique<Mesh>(std::move(boxGeometry), std::move(boxMaterial));
    // 缩小箱子 让它跟白球大小相对差别不那么大
    boxMesh->set_scale(glm::vec3(0.8f));
    // 初始的时候让箱子有个偏角观察全貌
    boxMesh->SetAngleX(30.0f);
    boxMesh->SetAngleY(45.0f);
    scene.AddChild(std::move(boxMesh));
    // 白色物体
    std::unique_ptr<Sphere> whiteObjGeometry = std::make_unique<Sphere>(0.1f);
    std::unique_ptr<WhiteMaterial> whiteObjMaterial = std::make_unique<WhiteMaterial>();
    std::unique_ptr<Mesh> whiteObjMesh = std::make_unique<Mesh>(std::move(whiteObjGeometry),
                                                                std::move(whiteObjMaterial));
    whiteObjMesh->set_position(glm::vec3(2.0f, 0.0f, 0.0f));
    Object *whiteMeshPtr = scene.AddChild(std::move(whiteObjMesh));
    // 光线
    std::unique_ptr<SpotLight> spotLight = std::make_unique<SpotLight>();
    spotLight->m_innerAngle = 15.0f;
    spotLight->m_outerAngle = 30.0f;
    spotLight->set_position(whiteMeshPtr->get_position());

    std::unique_ptr<DirectionalLight> directionalLight = std::make_unique<DirectionalLight>();
    directionalLight->set_direction(glm::vec3(1.0f, 0.0f, 0.0f));

    std::unique_ptr<PointLight> pointLight = std::make_unique<PointLight>();
    pointLight->set_position(glm::vec3(0.0f, 0.0f, 1.5f));
    pointLight->set_specular_intensity(0.5f);
    pointLight->m_k2 = 0.017f;
    pointLight->m_k1 = 0.07f;
    pointLight->m_kc = 1.0f;
    struct LightPack lights;
    lights.directional = std::move(directionalLight);
    lights.point = std::move(pointLight);
    lights.spot = std::move(spotLight);

    std::unique_ptr<AmbientLight> ambientLight = std::make_unique<AmbientLight>();
    ambientLight->set_color(glm::vec3(0.2f));
    PerspectiveCamera camera(static_cast<float>(glApp->get_width()) / static_cast<float>(glApp->get_height()));
    camera.set_position(glm::vec3(0.0f, 0.0f, 5.0f));
    // 相机控制器
    Input *input = glApp->get_input();
    input->CreateCameraController<TrackballCameraController>(camera);
    auto cameraCtl = input->get_CameraController();

    // 窗体循环
    while (glApp->Update()) {
        cameraCtl->OnUpdate();

        // 点光跟着白球的位置 让白球运动起来 点光位置就会变化
        float xPos = glm::sin(glfwGetTime()) + 2.0f;
        scene.get_children()[1]->set_position(glm::vec3(xPos, 0.0f, 0.0f));
        if (lights.spot) {
            lights.spot->set_position(scene.get_children()[1]->get_position());
        }
        // 每个帧在x轴上旋转
        scene.get_children()[0]->set_rotationY(0.5f);

        renderer.setClearColor(glApp->get_clearColor());
        // 每一帧清一次屏
        Renderer::BeginFrame();
        renderer.Render(scene, camera, lights);
        // imgui渲染
        glApp->RenderImGui();
    }
    return 0;
}
