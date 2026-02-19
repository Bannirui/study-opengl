#include "glframework/framebuffer/frame_buffer.h"

#include <memory>

#include <glm/glm.hpp>

#include "application/Application.h"
#include "application/camera/controller/CameraController.h"
#include "application/camera/PerspectiveCamera.h"
#include "application/camera/controller/TrackballCameraController.h"
#include "glframework/mesh/Mesh.h"
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

class App : public Application
{
public:
    App() = default;

public:
    void OnInit() override
    {
        m_renderer     = std::make_unique<Renderer>();
        m_sceneOffline = std::make_unique<Scene>();
        m_sceneDisplay = std::make_unique<Scene>();

        m_frameBuffer = std::make_unique<FrameBuffer>(m_Width, m_Height);

        auto geometry1 = std::make_unique<Box>();
        auto material1 = std::make_unique<PhongMaterial>();
        auto texture1  = std::make_shared<Texture>("asset/texture/wall.jpg", 1);
        material1->set_diffuse(texture1);
        auto mesh1 = std::make_unique<Mesh>(std::move(geometry1), std::move(material1));
        m_sceneOffline->AddChild(std::move(mesh1));

        auto geometry2 = std::make_unique<ScreenPlane>();
        auto material2 = std::make_unique<ScreenMaterial>();
        auto texture2  = m_frameBuffer->get_colorAttach();
        material2->set_screenTexture(texture2);
        auto mesh2 = std::make_unique<Mesh>(std::move(geometry2), std::move(material2));
        m_sceneDisplay->AddChild(std::move(mesh2));

        // 光线
        // 光源从右后方
        m_lights.directional = std::make_unique<DirectionalLight>();
        m_lights.directional->set_direction(glm::vec3(-1.0f));
        m_lights.directional->set_specular_intensity(0.1f);

        m_lights.ambient = std::make_unique<AmbientLight>();
        m_lights.ambient->set_color(glm::vec3(0.1f));

        // 相机
        m_camera = std::make_unique<PerspectiveCamera>(static_cast<float>(m_Width) / static_cast<float>(m_Height));
        m_camera->set_position(glm::vec3(0.0f, 0.0f, 5.0f));

        // 相机控制器
        m_input->CreateCameraController<TrackballCameraController>(*m_camera);
        m_cameraController = m_input->get_CameraController();
        m_cameraController->SetScaleSpeed(1.0f);
    }
    void OnUpdate(float dt) override
    {
        if (m_cameraController)
        {
            m_cameraController->OnUpdate();
        }
    }
    void OnRender() override
    {
        m_renderer->setClearColor(m_clearColor);

        // pass1, renderer to FBO 渲染到我自己的Frame Buffer
        glBindFramebuffer(GL_FRAMEBUFFER, m_frameBuffer->get_FBO());
        Renderer::BeginFrame(m_frameBuffer->get_width(), m_frameBuffer->get_height());
        m_renderer->Render(*m_sceneOffline, *m_camera, m_lights);

        // pass2, renderer to display 渲染到默认Frame Buffer
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        Renderer::BeginFrame(m_Width, m_Height);
        m_renderer->Render(*m_sceneDisplay, *m_camera, m_lights);
    }

private:
    std::unique_ptr<Renderer>    m_renderer;
    std::unique_ptr<Scene>       m_sceneOffline;
    std::unique_ptr<Scene>       m_sceneDisplay;
    std::unique_ptr<FrameBuffer> m_frameBuffer;

    std::unique_ptr<PerspectiveCamera> m_camera;
    CameraController*                  m_cameraController{nullptr};

    LightPack m_lights{};
};

int main()
{
    App app;
    if (!app.Init(1200, 800))
    {
        return -1;
    }
    app.Run();
    return 0;
}
