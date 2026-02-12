#include "glframework/framebuffer/frame_buffer.h"

#include <memory>

#include <glm/glm.hpp>

#include "application/Application.h"
#include "application/camera/CameraController.h"
#include "application/camera/PerspectiveCamera.h"
#include "application/camera/TrackballCameraController.h"
#include "glframework/Mesh.h"
#include "glframework/Scene.h"
#include "glframework/Texture.h"
#include "glframework/geo/Box.h"
#include "glframework/geo/ScreenPlane.h"
#include "glframework/light/AmbientLight.h"
#include "glframework/light/DirectionalLight.h"
#include "glframework/material/PhongMaterial.h"
#include "glframework/material/screen_material.h"
#include "glframework/renderer/Renderer.h"
#include "glframework/renderer/light_pack.h"
#include "input/input.h"

int main() {
    if (!glApp->Init(1200, 800)) return -1;
    glApp->set_clearColor(glm::vec4(1.0f, 0.5f, 0.2f, 1.0f));
    // 渲染器
    Renderer renderer;
    Scene sceneOffline;
    Scene sceneDisplay;

    FrameBuffer frameBuffer(glApp->get_width(), glApp->get_height());

    std::unique_ptr<Box> geometry1 = std::make_unique<Box>();
    std::unique_ptr<PhongMaterial> material1 = std::make_unique<PhongMaterial>();
    Texture texture1("asset/texture/grass.jpg", 1);
    material1->set_diffuse(&texture1);
    std::unique_ptr<Mesh> mesh1 = std::make_unique<Mesh>(std::move(geometry1), std::move(material1));
    sceneOffline.AddChild(std::move(mesh1));

    std::unique_ptr<ScreenPlane> geometry2 = std::make_unique<ScreenPlane>();
    std::unique_ptr<ScreenMaterial> material2 = std::make_unique<ScreenMaterial>();
    material2->set_screenTexture(frameBuffer.get_colorAttach());
    std::unique_ptr<Mesh> mesh2 = std::make_unique<Mesh>(std::move(geometry2), std::move(material2));
    sceneDisplay.AddChild(std::move(mesh2));

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
        // pass1, renderer to FBO 渲染到我自己的Frame Buffer
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer.get_FBO());
        // 每一帧清一次屏
        Renderer::BeginFrame(frameBuffer.get_width(), frameBuffer.get_height());
        renderer.Render(sceneOffline, camera, lights);

        // pass2, renderer to display 渲染到默认Frame Buffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        Renderer::BeginFrame(glApp->get_width(), glApp->get_height());
        renderer.Render(sceneDisplay, camera, lights);

        // imgui渲染
        glApp->RenderImGui();
    }
    return 0;
}
