#include <memory>

#include <glm/glm.hpp>

#include "application/Application.h"
#include "application/camera/CameraController.h"
#include "application/camera/PerspectiveCamera.h"
#include "application/camera/TrackballCameraController.h"
#include "glframework/Mesh.h"
#include "glframework/Scene.h"
#include "glframework/Texture.h"
#include "glframework/geo/Plane.h"
#include "glframework/light/AmbientLight.h"
#include "glframework/light/DirectionalLight.h"
#include "glframework/material/DepthMaterial.h"
#include "glframework/renderer/Renderer.h"
#include "glframework/renderer/light_pack.h"
#include "input/input.h"

int main() {
    if (!glApp->Init(1600, 800)) return -1;
    // 渲染器
    Renderer renderer;
    Scene scene;
    std::unique_ptr<Plane> geometryA = std::make_unique<Plane>(5.0f, 5.0f);
    std::unique_ptr<DepthMaterial> materialA = std::make_unique<DepthMaterial>();
    std::unique_ptr<Mesh> meshA = std::make_unique<Mesh>(std::move(geometryA), std::move(materialA));
    scene.AddChild(std::move(meshA));

    std::unique_ptr<Plane> geometryB = std::make_unique<Plane>(5.0f, 5.0f);
    std::unique_ptr<DepthMaterial> materialB = std::make_unique<DepthMaterial>();
    std::unique_ptr<Mesh> meshB = std::make_unique<Mesh>(std::move(geometryB), std::move(materialB));
    meshB->set_position(glm::vec3(2.0f, 0.5f, -1.0f));
    scene.AddChild(std::move(meshB));

    std::unique_ptr<Plane> geometryC = std::make_unique<Plane>(5.0f, 5.0f);
    std::unique_ptr<DepthMaterial> materialC = std::make_unique<DepthMaterial>();
    std::unique_ptr<Mesh> meshC = std::make_unique<Mesh>(std::move(geometryC), std::move(materialC));
    meshC->set_position(glm::vec3(4.0f, 1.0f, -2.0f));
    scene.AddChild(std::move(meshC));

    // 光线
    std::unique_ptr<DirectionalLight> directionalLight = std::make_unique<DirectionalLight>();
    // 光源从右后方
    directionalLight->set_direction(glm::vec3(-1.0f, -1.0f, -1.0f));
    directionalLight->set_specular_intensity(1.0f);
    std::unique_ptr<AmbientLight> ambientLight = std::make_unique<AmbientLight>();
    ambientLight->set_color(glm::vec3(0.2f));
    struct LightPack lights;
    lights.directional = std::move(directionalLight);
    lights.ambient = std::move(ambientLight);
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

        renderer.setClearColor(glApp->get_clearColor());
        // 每一帧清一次屏
        Renderer::BeginFrame();
        renderer.Render(scene, camera, lights);

        // imgui渲染
        glApp->RenderImGui();
    }
    return 0;
}
