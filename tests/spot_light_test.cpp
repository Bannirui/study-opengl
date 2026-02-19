//
// Created by dingrui on 25-6-23.
//

#include <memory>

#include <glm/glm.hpp>

#include "err_check.h"
#include "application/Application.h"
#include "application/camera/Camera.h"
#include "application/camera/PerspectiveCamera.h"
#include "application/camera/controller/TrackballCameraController.h"
#include "glframework/obj/mesh/Mesh.h"
#include "glframework/Texture.h"
#include "glframework/geo/Box.h"
#include "glframework/geo/Sphere.h"
#include "glframework/light/AmbientLight.h"
#include "glframework/light/DirectionalLight.h"
#include "glframework/light/PointLight.h"
#include "glframework/light/SpotLight.h"
#include "glframework/material/PhongMaterial.h"
#include "glframework/material/WhiteMaterial.h"
#include "glframework/renderer/Renderer.h"
#include "glframework/renderer/light_pack.h"
#include "input/input.h"
#include "glframework/obj/Scene.h"

class App : public Application
{
public:
    App() = default;

    void OnInit() override
    {
        m_renderer     = std::make_unique<Renderer>();
        m_scene        = std::make_unique<Scene>();
        auto geometry1 = std::make_unique<Box>();
        auto material1 = std::make_unique<PhongMaterial>();
        material1->set_shines(32.0f);
        auto texture1 = std::make_shared<Texture>("asset/texture/box.png", 0);
        material1->set_diffuse(texture1);
        auto texture2 = std::make_shared<Texture>("asset/texture/sp_mask.png", 1);
        material1->set_specular_mask(texture2);
        auto mesh1 = std::make_unique<Mesh>(std::move(geometry1), std::move(material1));
        // 缩小箱子 让它跟白球大小相对差别不那么大
        mesh1->set_scale(glm::vec3(0.8f));
        // 初始的时候让箱子有个偏角观察全貌
        mesh1->SetAngleX(30.0f);
        mesh1->SetAngleY(45.0f);
        m_scene->AddChild(std::move(mesh1));
        // 白色物体
        auto geometry2 = std::make_unique<Sphere>(0.1f);
        auto material2 = std::make_unique<WhiteMaterial>();
        auto mesh2     = std::make_unique<Mesh>(std::move(geometry2), std::move(material2));
        mesh2->set_position(glm::vec3(2.0f, 0.0f, 0.0f));
        m_scene->AddChild(std::move(mesh2));
        // 光线
        m_lights.spot               = std::make_unique<SpotLight>();
        m_lights.spot->m_innerAngle = 15.0f;
        m_lights.spot->m_outerAngle = 30.0f;
        m_lights.spot->set_position(m_scene->get_children()[1]->get_position());

        m_lights.directional = std::make_unique<DirectionalLight>();
        m_lights.directional->set_direction(glm::vec3(1.0f, 0.0f, 0.0f));

        m_lights.point = std::make_unique<PointLight>();
        m_lights.point->set_position(glm::vec3(0.0f, 0.0f, 1.5f));
        m_lights.point->set_specular_intensity(0.5f);
        m_lights.point->m_k2 = 0.017f;
        m_lights.point->m_k1 = 0.07f;
        m_lights.point->m_kc = 1.0f;

        m_lights.ambient = std::make_unique<AmbientLight>();
        m_lights.ambient->set_color(glm::vec3(0.2f));

        m_camera = std::make_unique<PerspectiveCamera>(static_cast<float>(m_Width) / static_cast<float>(m_Height));
        m_camera->set_position(glm::vec3(0.0f, 0.0f, 5.0f));
        // 相机控制器
        m_input = std::make_unique<Input>();
        m_input->CreateCameraController<TrackballCameraController>(*m_camera);
        m_cameraController = m_input->get_CameraController();
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
        // 点光跟着白球的位置 让白球运动起来 点光位置就会变化
        float xPos = glm::sin(glfwGetTime()) + 2.0f;
        m_scene->get_children()[1]->set_position(glm::vec3(xPos, 0.0f, 0.0f));
        if (m_lights.spot)
        {
            m_lights.spot->set_position(m_scene->get_children()[1]->get_position());
        }
        // 每个帧在x轴上旋转
        m_scene->get_children()[0]->set_rotationY(0.5f);

        m_renderer->setClearColor(m_clearColor);

        Renderer::BeginFrame();
        m_renderer->Render(*m_scene, *m_camera, m_lights);
    }

private:
    std::unique_ptr<Renderer> m_renderer;
    std::unique_ptr<Scene>    m_scene;

    std::unique_ptr<PerspectiveCamera> m_camera;
    CameraController*                  m_cameraController{nullptr};

    LightPack m_lights{};
};

// 聚光灯 平行光 点光
int main()
{
    App app;
    if (!app.Init(1600, 800))
    {
        return -1;
    }
    app.Run();
    return 0;
}
