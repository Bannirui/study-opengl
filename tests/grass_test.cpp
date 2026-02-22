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
#include "glframework/material/cube_spherical_material.h"
#include "glframework/material/grass_instance_material.h"
#include "glframework/obj/mesh/instance_mesh_by_attribute.h"
#include "glframework/renderer/Renderer.h"
#include "glframework/renderer/light_pack.h"
#include "input/input.h"

void setInstanceMaterial(Object* obj, const std::shared_ptr<Material>& material)
{
    if (!obj)
    {
        return;
    }
    if (auto* mesh = dynamic_cast<InstanceMeshByAttribute*>(obj))
    {
        mesh->set_material(material);
    }
    for (auto& child : obj->get_children())
    {
        setInstanceMaterial(child.get(), material);
    }
}

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

        int                    r = 20, c = 20;
        std::vector<glm::mat4> transforms{};
        srand(glfwGetTime());
        for (int i = 0; i < r; ++i)
        {
            for (int j = 0; j < c; ++j)
            {
                glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(1.0 * r, 0.0f, 1.0 * c));
                glm::mat4 rotate    = glm::rotate(glm::radians((float)(rand() % 90)), glm::vec3(0.0, 1.0, 0.0));
                auto      transform = translate * rotate;
                transforms.push_back(transform);
            }
        }

        auto grassModel    = AssimpInstanceLoader::Load("asset/obj/grass.obj", transforms);
        auto grassMaterial = std::make_shared<GrassInstanceMaterial>();
        auto texture2      = std::make_shared<Texture>("asset/texture/grass.png", 1);
        grassMaterial->set_diffuse(texture2);
        auto texture3 = std::make_shared<Texture>("asset/texture/grass_mask.png", 2);
        grassMaterial->set_opacityMask(texture3);
        grassMaterial->set_enableBlend(true);
        setInstanceMaterial(grassModel.get(), grassMaterial);
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
