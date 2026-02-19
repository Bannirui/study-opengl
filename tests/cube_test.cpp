#include <memory>

#include "application/Application.h"
#include "application/camera/controller/CameraController.h"
#include "application/camera/PerspectiveCamera.h"
#include "application/camera/controller/TrackballCameraController.h"
#include "glframework/obj/mesh/Mesh.h"
#include "glframework/obj/Scene.h"
#include "glframework/Texture.h"
#include "glframework/geo/Box.h"
#include "glframework/geo/Sphere.h"
#include "glframework/material/PhongMaterial.h"
#include "glframework/material/cube_map_material.h"
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
        m_renderer = std::make_unique<Renderer>();
        m_scene    = std::make_unique<Scene>();

        // clang-format off
        // right left up down rear front
        std::vector<std::string> texturePaths = {
            "asset/texture/skybox/right.jpg",
            "asset/texture/skybox/left.jpg",
            "asset/texture/skybox/top.jpg",
            "asset/texture/skybox/bottom.jpg",
            "asset/texture/skybox/back.jpg",
            "asset/texture/skybox/front.jpg"
        };
        // clang-format on
        auto geometry1 = std::make_unique<Box>(4.0f);
        auto material1 = std::make_unique<CubeMapMaterial>();
        auto texture1  = std::make_shared<Texture>(texturePaths, 0);
        material1->set_diffuse(texture1);
        auto mesh1 = std::make_unique<Mesh>(std::move(geometry1), std::move(material1));
        m_scene->AddChild(std::move(mesh1));

        auto geometry2 = std::make_unique<Sphere>(3.0f);
        auto material2 = std::make_unique<PhongMaterial>();
        auto texture2  = std::make_shared<Texture>("asset/texture/earth.jpg", 0);
        material2->set_diffuse(texture2);
        auto mesh2 = std::make_unique<Mesh>(std::move(geometry2), std::move(material2));
        m_scene->AddChild(std::move(mesh2));

        // 光线
        m_lights.directional = std::make_unique<DirectionalLight>();
        m_lights.directional->set_direction(glm::vec3(-1.0f, 0.0f, 0.0f));

        // 相机
        m_camera = std::make_unique<PerspectiveCamera>(static_cast<float>(m_Width) / static_cast<float>(m_Height));

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
