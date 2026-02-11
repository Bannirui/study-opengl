#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "application/Application.h"
#include "application/camera/CameraController.h"
#include "application/camera/PerspectiveCamera.h"
#include "application/camera/TrackballCameraController.h"
#include "glframework/Mesh.h"
#include "glframework/Scene.h"
#include "glframework/Texture.h"
#include "glframework/geo/Box.h"
#include "glframework/geo/Plane.h"
#include "glframework/geo/Sphere.h"
#include "glframework/light/AmbientLight.h"
#include "glframework/light/DirectionalLight.h"
#include "glframework/material/PhongMaterial.h"
#include "glframework/material/WhiteMaterial.h"
#include "glframework/renderer/Renderer.h"
#include "glframework/renderer/light_pack.h"
#include "input/input.h"

int main() {
    if (!glApp->Init(1200, 800)) return -1;
    glApp->set_clearColor(glm::vec4(1.0f, 0.5f, 0.2f, 1.0f));
    // 渲染器
    Renderer renderer;

    Scene scene;
    std::shared_ptr<Box> geometryA = std::make_shared<Box>(4.0f);
    std::shared_ptr<PhongMaterial> materialA = std::make_shared<PhongMaterial>();
    materialA->set_diffuse(new Texture("asset/texture/box.png", 0));
    materialA->set_enableBlend(true);
    materialA->set_depthWrite(false);
    materialA->set_opacity(0.3f);
    std::unique_ptr<Mesh> meshA = std::make_unique<Mesh>(geometryA, materialA);
    scene.AddChild(std::move(meshA));

    std::shared_ptr<Sphere> geometryB = std::make_shared<Sphere>(2.0f);
    std::shared_ptr<PhongMaterial> materialB = std::make_shared<PhongMaterial>();
    materialB->set_diffuse(new Texture("asset/texture/earth.jpg", 1));
    std::unique_ptr<Mesh> meshB = std::make_unique<Mesh>(geometryB, materialB);
    meshB->set_position(glm::vec3(5.0f, 0.0f, 0.0f));
    scene.AddChild(std::move(meshB));

    // 光线
    std::shared_ptr<DirectionalLight> directionalLight = std::make_shared<DirectionalLight>();
    // 光源从右后方
    directionalLight->m_direction = glm::vec3(-1.0f, -1.0f, -1.0f);
    directionalLight->set_specular_intensity(1.0f);
    std::shared_ptr<AmbientLight> ambientLight = std::make_shared<AmbientLight>();
    ambientLight->set_color(glm::vec3(0.2f));
    struct LightPack lights;
    lights.directional = directionalLight;
    lights.ambient = ambientLight;
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
