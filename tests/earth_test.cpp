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
#include "glframework/Scene.h"
#include "glframework/Texture.h"
#include "glframework/geo/Sphere.h"
#include "glframework/light/AmbientLight.h"
#include "glframework/light/DirectionalLight.h"
#include "glframework/material/PhongMaterial.h"
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
        m_scene        = std::make_unique<Scene>();
        auto geometry1 = std::make_unique<Sphere>();
        auto material1 = std::make_unique<PhongMaterial>();
        material1->set_shines(10.0f);
        auto texture1 = std::make_shared<Texture>("asset/texture/wall.jpg", 0);
        material1->set_diffuse(texture1);
        auto mesh1 = std::make_unique<Mesh>(std::move(geometry1), std::move(material1));
        m_scene->AddChild(std::move(mesh1));

        auto geo2      = std::make_unique<Sphere>(1.0f);
        auto material2 = std::make_unique<PhongMaterial>();
        material2->set_shines(16.0f);
        auto texture2 = std::make_shared<Texture>("asset/texture/earth.jpg", 1);
        material2->set_diffuse(texture2);
        auto mesh2 = std::make_unique<Mesh>(std::move(geo2), std::move(material2));
        mesh2->set_position(glm::vec3(2.5f, 0.0f, 0.0f));
        m_scene->AddChild(std::move(mesh2));

        // 光线
        m_lights.directional = std::make_unique<DirectionalLight>();
        // 光源从右后方
        m_lights.directional->set_direction(glm::vec3(-1.0f, 0.0f, 0.0f));
        m_lights.ambient = std::make_unique<AmbientLight>();
        m_lights.ambient->set_color(glm::vec3(0.9f));

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
            m_scene->get_children()[1]->set_rotationY(0.05f);
        }
    }
    void OnRender() override
    {
        m_renderer->setClearColor(m_clearColor);

        Renderer::BeginFrame(m_Width, m_Height);
        m_renderer->Render(*m_scene, *m_camera, m_lights);
    }

private:
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<Scene>    m_scene;

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
