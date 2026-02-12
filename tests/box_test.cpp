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
#include "glframework/light/AmbientLight.h"
#include "glframework/light/DirectionalLight.h"
#include "glframework/light/PointLight.h"
#include "glframework/material/PhongMaterial.h"
#include "glframework/renderer/Renderer.h"
#include "glframework/renderer/light_pack.h"
#include "input/input.h"

int main() {
    if (!glApp->Init(1600, 800)) return -1;

    Renderer renderer;
    // the root
    Object object(ObjectType::Object);
    // 渲染列表
    // 箱子
    Box boxGeometry;
    PhongMaterial boxMaterial;
    boxMaterial.set_shines(32.0f);
    Texture diffuseTexture("asset/texture/box.png", 0);
    boxMaterial.set_diffuse(&diffuseTexture);
    Texture specularMask("asset/texture/sp_mask.png", 1);
    boxMaterial.set_specular_mask(&specularMask);
    std::unique_ptr<Mesh> boxMesh = std::make_unique<Mesh>(std::unique_ptr<Box>(&boxGeometry), std::unique_ptr<PhongMaterial>(&boxMaterial));
    object.AddChild(std::move(boxMesh));
    // 光线
    std::unique_ptr<DirectionalLight> directionalLight = std::make_unique<DirectionalLight>();
    directionalLight->set_direction(glm::vec3(-1.0f, 0.0f, 0.0f));
    std::unique_ptr<AmbientLight> ambient_light = std::make_unique<AmbientLight>();
    ambient_light->set_color(glm::vec3(0.8f));
    struct LightPack lights;
    lights.directional = std::move(directionalLight);
    lights.ambient = std::move(ambient_light);
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
        object.get_children()[0]->set_rotationX(0.1f);
        object.get_children()[0]->set_rotationY(0.05f);
        object.get_children()[0]->set_rotationZ(0.01f);

        // 每一帧清一次屏
        Renderer::BeginFrame();
        renderer.Render(object, camera, lights);
    }
    return 0;
}
