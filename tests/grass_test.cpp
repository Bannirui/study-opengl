#include <memory>

#include "application/Application.h"
#include "application/assimp_instance_loader.h"
#include "application/camera/controller/CameraController.h"
#include "application/camera/PerspectiveCamera.h"
#include "application/camera/controller/TrackballCameraController.h"
#include "glframework/obj/mesh/Mesh.h"
#include "glframework/obj/Scene.h"
#include "glframework/Texture.h"
#include "glframework/geo/Box.h"
#include "glframework/geo/Sphere.h"
#include "glframework/material/cube_spherical_material.h"
#include "glframework/material/phong_instance_by_attribute_material.h"
#include "glframework/obj/mesh/instance_mesh_by_attribute.h"
#include "glframework/obj/mesh/instance_mesh_by_uniform.h"
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

        auto geometry1 = std::make_unique<Box>(4.0f);
        auto material1 = std::make_unique<CubeSphericalMaterial>();
        auto texture1  = std::make_shared<Texture>("asset/texture/sphericalMap.png", 0, Texture::TextureType::kCube);
        material1->set_diffuse(texture1);
        auto mesh1 = std::make_unique<Mesh>(std::move(geometry1), std::move(material1));
        m_scene->AddChild(std::move(mesh1));

        glm::mat4 transform1 = glm::mat4(1.0f);
        glm::mat4 transform2 = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 0.0f));
        glm::mat4 transform3 = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, 8.0f, 0.0f));
        glm::mat4 transform4 = glm::translate(glm::mat4(1.0f), glm::vec3(12.0f, -2.0f, 4.0f));
        std::vector<glm::mat4> transforms = {transform1, transform2, transform3, transform4};
        auto grassModel = AssimpInstanceLoader::Load("asset/fbx/grass.fbx", transforms);
        m_scene->AddChild(std::move(grassModel));

        m_lights.directional = std::make_unique<DirectionalLight>();
        m_lights.directional->set_direction(glm::vec3(1.0f, 0.0f, 0.0f));

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
