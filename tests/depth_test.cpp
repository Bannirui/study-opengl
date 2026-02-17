#include <memory>

#include <glm/glm.hpp>

#include "application/Application.h"
#include "../include/application/camera/controller/CameraController.h"
#include "application/camera/PerspectiveCamera.h"
#include "../include/application/camera/controller/TrackballCameraController.h"
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

class App : public Application
{
public:
    App() = default;

public:
    void OnInit() override
    {
        m_renderer     = std::make_unique<Renderer>();
        scene          = std::make_unique<Scene>();
        auto geometry1 = std::make_unique<Plane>(5.0f, 5.0f);
        auto material1 = std::make_unique<DepthMaterial>();
        auto mesh1     = std::make_unique<Mesh>(std::move(geometry1), std::move(material1));
        scene->AddChild(std::move(mesh1));

        auto geometry2 = std::make_unique<Plane>(5.0f, 5.0f);
        auto material2 = std::make_unique<DepthMaterial>();
        auto mesh2     = std::make_unique<Mesh>(std::move(geometry2), std::move(material2));
        mesh2->set_position(glm::vec3(2.0f, 0.5f, -1.0f));
        scene->AddChild(std::move(mesh2));

        auto geometry3 = std::make_unique<Plane>(5.0f, 5.0f);
        auto material3 = std::make_unique<DepthMaterial>();
        auto mesh3     = std::make_unique<Mesh>(std::move(geometry3), std::move(material3));
        mesh3->set_position(glm::vec3(4.0f, 1.0f, -2.0f));
        scene->AddChild(std::move(mesh3));

        // 光线
        m_lights.directional = std::make_unique<DirectionalLight>();
        // 光源从右后方
        m_lights.directional->set_direction(glm::vec3(-1.0f));
        m_lights.directional->set_specular_intensity(1.0f);
        m_lights.ambient = std::make_unique<AmbientLight>();
        m_lights.ambient->set_color(glm::vec3(0.2f));

        // 相机
        m_camera = std::make_unique<PerspectiveCamera>(static_cast<float>(m_Width) / static_cast<float>(m_Height));
        m_camera->set_position(glm::vec3(0.0f, 0.0f, 5.0f));

        // 相机控制器
        m_input = std::make_unique<Input>();
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

        Renderer::BeginFrame(m_Width, m_Height);
        m_renderer->Render(*scene, *m_camera, m_lights);
    }

private:
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<Scene>    scene;

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
