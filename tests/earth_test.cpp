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

    // 负责渲染
    Renderer renderer;
    // 物体
    // 球体
    std::vector<std::shared_ptr<Mesh> > meshes;
    std::shared_ptr<Sphere> ballGeometry = std::make_shared<Sphere>();
    std::shared_ptr<PhongMaterial> ballMaterial = std::make_shared<PhongMaterial>();
    ballMaterial->set_shines(10.0f);
    ballMaterial->set_diffuse(new Texture("asset/texture/wall.jpg", 0));
    std::shared_ptr<Mesh> ballMesh = std::make_shared<Mesh>(ballGeometry, ballMaterial);
    meshes.push_back(ballMesh);
    // 地球
    std::shared_ptr<Sphere> earthGeometry = std::make_shared<Sphere>(1.0f);
    std::shared_ptr<PhongMaterial> earthMaterial = std::make_shared<PhongMaterial>();
    earthMaterial->set_shines(16.0f);
    earthMaterial->set_diffuse(new Texture("asset/texture/earth.jpg", 1));
    std::shared_ptr<Mesh> earthMesh = std::make_shared<Mesh>(earthGeometry, earthMaterial);
    earthMesh->SetPosition(glm::vec3(2.5f, 0.0f, 0.0f));
    meshes.push_back(earthMesh);
    // 光线
    std::shared_ptr<DirectionalLight> directionalLight = std::make_shared<DirectionalLight>();
    directionalLight->m_direction = glm::vec3(-1.0f, -1.0f, -1.0f);
    std::shared_ptr<AmbientLight> ambientLight = std::make_shared<AmbientLight>();
    ambientLight->set_color(glm::vec3(0.2f));
    struct LightPack lights;
    lights.directional = directionalLight;
    lights.ambient = ambientLight;
    // 相机
    PerspectiveCamera camera(static_cast<float>(glApp->get_width()) / static_cast<float>(glApp->get_height()));
    camera.set_position(glm::vec3(0.0f, 0.0f, 5.0f));

    InputDispatcher inputDispatcher(glApp);
    inputDispatcher.CreateCameraController<TrackballCameraController>(camera);
    auto cameraCtl = inputDispatcher.get_CameraController();

    // 窗体循环
    while (glApp->Update()) {
        cameraCtl->OnUpdate();
        meshes[1]->SetRotationY(0.2f);

        // 每一帧清一次屏
        Renderer::BeginFrame();
        renderer.render(meshes, camera, lights);
    }
    return 0;
}
