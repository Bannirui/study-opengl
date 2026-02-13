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
#include "glframework/Scene.h"

class App : public Application
{
public:
    App() = default;

public:
    void OnInit() override
    {
        m_renderer     = std::make_unique<Renderer>();
        m_scene        = std::make_unique<Scene>();
        auto geometry1 = std::make_unique<Box>(5.0f);
        auto material1 = std::make_unique<PhongMaterial>();
        auto texture1  = std::make_shared<Texture>("asset/texture/box.png", 0);
        material1->set_diffuse(texture1);
        material1->set_shines(32.0f);
        auto texture2 = std::make_shared<Texture>("asset/texture/sp_mask.png", 1);
        material1->set_specular_mask(texture2);
        auto mesh1 = std::make_unique<Mesh>(std::move(geometry1), std::move(material1));
        m_scene->AddChild(std::move(mesh1));

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
            m_scene->get_children()[0]->set_rotationX(0.1f);
            m_scene->get_children()[0]->set_rotationY(0.05f);
            m_scene->get_children()[0]->set_rotationZ(0.01f);
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
