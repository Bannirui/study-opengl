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
#include "glframework/material/opacity_mask_material.h"
#include "glframework/material/PhongMaterial.h"
#include "glframework/renderer/Renderer.h"
#include "glframework/renderer/light_pack.h"
#include "input/input.h"

int main() {
    if (!glApp->Init(1200, 800)) return -1;
    glApp->set_clearColor(glm::vec4(1.0f, 0.5f, 0.2f, 1.0f));
    // 渲染器
    Renderer renderer;

    Scene scene;
    std::unique_ptr<Plane> geometry = std::make_unique<Plane>(4.0f, 4.0f);
    std::unique_ptr<OpacityMaskMaterial> material = std::make_unique<OpacityMaskMaterial>();
    Texture diffuse("asset/texture/grass.jpg", 0);
    material->set_diffuse(&diffuse);
    Texture opacityMask("asset/texture/grass_mask.png", 1);
    material->set_opacityMask(&opacityMask);
    material->set_enableBlend(true);
    std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>(std::move(geometry), std::move(material));
    scene.AddChild(std::move(mesh));

    // 光线
    std::unique_ptr<DirectionalLight> directionalLight = std::make_unique<DirectionalLight>();
    // 光源从右后方
    directionalLight->set_direction(glm::vec3(-1.0f));
    directionalLight->set_specular_intensity(0.1f);
    std::unique_ptr<AmbientLight> ambientLight = std::make_unique<AmbientLight>();
    ambientLight->set_color(glm::vec3(0.1f));
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
    cameraCtl->SetScaleSpeed(1.0f);

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
