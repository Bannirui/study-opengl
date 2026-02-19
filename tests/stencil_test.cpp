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
#include "glframework/geo/Sphere.h"
#include "glframework/light/AmbientLight.h"
#include "glframework/light/DirectionalLight.h"
#include "glframework/material/PhongMaterial.h"
#include "glframework/material/WhiteMaterial.h"
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
        auto geometry1 = std::make_unique<Box>(5.0f);
        auto material1 = std::make_unique<PhongMaterial>();
        auto texture1  = std::make_shared<Texture>("asset/texture/box.png", 0);
        material1->set_diffuse(texture1);
        material1->set_stencilTest(true);
        material1->set_sFail(GL_KEEP);
        material1->set_zFail(GL_KEEP);
        material1->set_zPass(GL_REPLACE);
        material1->set_stencilMask(0xff);
        material1->set_stencilFunc(GL_ALWAYS);
        material1->set_stencilRef(1);
        material1->set_stencilFuncMask(0xff);
        auto mesh1 = std::make_unique<Mesh>(std::move(geometry1), std::move(material1));
        m_scene->AddChild(std::move(mesh1));

        auto geometry2 = std::make_unique<Box>(5.0f);
        auto material2 = std::make_unique<WhiteMaterial>();
        material2->set_stencilTest(true);
        material2->set_sFail(GL_KEEP);
        material2->set_zFail(GL_KEEP);
        material2->set_zPass(GL_KEEP);
        material2->set_stencilMask(0);
        material2->set_stencilFunc(GL_NOTEQUAL);
        material2->set_stencilRef(1);
        material2->set_stencilFuncMask(0xff);
        auto mesh2 = std::make_unique<Mesh>(std::move(geometry2), std::move(material2));
        mesh2->set_position(m_scene->get_children()[0]->get_position());
        mesh2->set_scale(glm::vec3(1.2f));
        m_scene->AddChild(std::move(mesh2));

        auto geometry3 = std::make_unique<Sphere>(2.0f);
        auto material3 = std::make_unique<PhongMaterial>();
        auto texture3  = std::make_shared<Texture>("asset/texture/earth.jpg", 1);
        material3->set_diffuse(texture3);
        material3->set_stencilTest(true);
        material3->set_sFail(GL_KEEP);
        material3->set_zFail(GL_KEEP);
        material3->set_zPass(GL_REPLACE);
        material3->set_stencilMask(0xff);
        material3->set_stencilFunc(GL_ALWAYS);
        material3->set_stencilRef(1);
        material3->set_stencilFuncMask(0xff);
        auto mesh3 = std::make_unique<Mesh>(std::move(geometry3), std::move(material3));
        mesh3->set_position(glm::vec3(3.0f, 0.0f, 0.0f));
        m_scene->AddChild(std::move(mesh3));

        auto geometry4 = std::make_unique<Sphere>(2.0f);
        auto material4 = std::make_unique<WhiteMaterial>();
        material4->set_stencilTest(true);
        material4->set_sFail(GL_KEEP);
        material4->set_zFail(GL_KEEP);
        material4->set_zPass(GL_KEEP);
        material4->set_stencilMask(0);
        material4->set_stencilFunc(GL_NOTEQUAL);
        material4->set_stencilRef(1);
        material4->set_stencilFuncMask(0xff);
        auto mesh4 = std::make_unique<Mesh>(std::move(geometry4), std::move(material4));
        mesh4->set_position(m_scene->get_children()[2]->get_position());
        mesh4->set_scale(glm::vec3(1.2f));
        m_scene->AddChild(std::move(mesh4));

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
