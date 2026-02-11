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
#include "glframework/material/WhiteMaterial.h"
#include "glframework/renderer/Renderer.h"
#include "glframework/renderer/light_pack.h"
#include "input/input.h"

// 点光跟着白球的位置 让白球运动起来 点光位置就会变化
void meshTransform(const Object &root, const struct LightPack &lights) {
    float xPos = glm::sin(glfwGetTime()) + 2.0f;
    root.get_children()[1]->set_position(glm::vec3(xPos, 0.0f, 0.0f));
    if (lights.point) {
        lights.point->set_position(root.get_children()[1]->get_position());
    }
    root.get_children()[0]->set_rotationX(1.0f);
    root.get_children()[0]->set_rotationY(0.2f);
}

int main() {
    if (!glApp->Init(1600, 800)) return -1;
    // 给渲染器准备好的就只需要 实体 光 相机
    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();
    // 实体
    Object root(ObjectType::Object);
    // 箱子
    std::shared_ptr<Box> boxGeometry = std::make_shared<Box>();
    std::shared_ptr<PhongMaterial> boxMaterial = std::make_shared<PhongMaterial>();
    boxMaterial->set_shines(32.0f);
    boxMaterial->set_diffuse(new Texture("asset/texture/box.png", 0));
    boxMaterial->set_specular_mask(new Texture("asset/texture/sp_mask.png", 1));
    std::unique_ptr<Mesh> boxMesh = std::make_unique<Mesh>(boxGeometry, boxMaterial);
    root.AddChild(std::move(boxMesh));
    // 白色物体
    std::shared_ptr<Sphere> whiteObjGeometry = std::make_shared<Sphere>(0.1f);
    std::shared_ptr<WhiteMaterial> whiteObjMaterial = std::make_shared<WhiteMaterial>();
    std::unique_ptr<Mesh> whiteObjMesh = std::make_unique<Mesh>(whiteObjGeometry, whiteObjMaterial);
    whiteObjMesh->set_position(glm::vec3(2.0f, 0.0f, 0.0f));
    Object *whiteObjMeshPtr = root.AddChild(std::move(whiteObjMesh));
    // 光线
    struct LightPack lights;
    std::shared_ptr<PointLight> point_light = std::make_shared<PointLight>();
    point_light->set_position(whiteObjMeshPtr->get_position());
    std::shared_ptr<AmbientLight> ambient_light = std::make_shared<AmbientLight>();
    ambient_light->set_color(glm::vec3(0.2f));
    lights.point = point_light;
    lights.ambient = ambient_light;

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
        meshTransform(root, lights);
        // 每一帧清一次屏
        Renderer::BeginFrame();
        renderer->render(root, camera, lights);
    }
    return 0;
}
