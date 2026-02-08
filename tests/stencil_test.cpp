#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "application/Application.h"
#include "application/camera/CameraController.h"
#include "application/camera/PerspectiveCamera.h"
#include "application/camera/TrackballCameraController.h"
#include "glframework/Mesh.h"
#include "glframework/Scene.h"
#include "glframework/Texture.h"
#include "glframework/geo/Box.h"
#include "glframework/geo/Plane.h"
#include "glframework/geo/Sphere.h"
#include "glframework/light/AmbientLight.h"
#include "glframework/light/DirectionalLight.h"
#include "glframework/material/DepthMaterial.h"
#include "glframework/material/PhongMaterial.h"
#include "glframework/material/WhiteMaterial.h"
#include "glframework/renderer/Renderer.h"
#include "glframework/renderer/light_pack.h"
#include "input/input_dispatcher.h"

int main() {
    if (!glApp->Init(1200, 800)) return -1;
    glApp->set_clearColor(glm::vec4(1.0f, 0.5f, 0.2f, 1.0f));
    // 监听事件
    glApp->RegisterCallback();
    // 渲染器
    Renderer renderer;

    std::shared_ptr<Scene> scene = std::make_shared<Scene>();

    std::shared_ptr<Box> geometryA = std::make_shared<Box>(4.0f);
    std::shared_ptr<PhongMaterial> materialA = std::make_shared<PhongMaterial>();
    materialA->set_diffuse(new Texture("asset/texture/box.png", 0));
    materialA->set_stencilTest(true);
    materialA->set_sFail(GL_KEEP);
    materialA->set_zFail(GL_KEEP);
    materialA->set_zPass(GL_REPLACE);
    materialA->set_stencilMask(0xff);
    materialA->set_stencilFunc(GL_ALWAYS);
    materialA->set_stencilRef(1);
    materialA->set_stencilFuncMask(0xff);
    std::shared_ptr<Mesh> meshA = std::make_shared<Mesh>(geometryA, materialA);
    scene->AddChild(meshA);

    std::shared_ptr<WhiteMaterial> materialABound = std::make_shared<WhiteMaterial>();
    materialABound->set_stencilTest(true);
    materialABound->set_sFail(GL_KEEP);
    materialABound->set_zFail(GL_KEEP);
    materialABound->set_zPass(GL_KEEP);
    materialABound->set_stencilMask(0);
    materialABound->set_stencilFunc(GL_NOTEQUAL);
    materialABound->set_stencilRef(1);
    materialABound->set_stencilFuncMask(0xff);
    std::shared_ptr<Mesh> meshABound = std::make_shared<Mesh>(geometryA, materialABound);
    meshABound->SetPosition(meshA->GetPosition());
    meshABound->SetScale(glm::vec3(1.2f));
    scene->AddChild(meshABound);

    std::shared_ptr<Sphere> geometryB = std::make_shared<Sphere>(2.0f);
    std::shared_ptr<PhongMaterial> materialB = std::make_shared<PhongMaterial>();
    materialB->set_diffuse(new Texture("asset/texture/earth.jpg", 1));
    materialB->set_stencilTest(true);
    materialB->set_sFail(GL_KEEP);
    materialB->set_zFail(GL_KEEP);
    materialB->set_zPass(GL_REPLACE);
    materialB->set_stencilMask(0xff);
    materialB->set_stencilFunc(GL_ALWAYS);
    materialB->set_stencilRef(1);
    materialB->set_stencilFuncMask(0xff);
    std::shared_ptr<Mesh> meshB = std::make_shared<Mesh>(geometryB, materialB);
    meshB->SetPosition(glm::vec3(3.0f, 0.0f, 0.0f));
    scene->AddChild(meshB);

    std::shared_ptr<WhiteMaterial> materialBBound = std::make_shared<WhiteMaterial>();
    materialBBound->set_stencilTest(true);
    materialBBound->set_sFail(GL_KEEP);
    materialBBound->set_zFail(GL_KEEP);
    materialBBound->set_zPass(GL_KEEP);
    materialBBound->set_stencilMask(0);
    materialBBound->set_stencilFunc(GL_NOTEQUAL);
    materialBBound->set_stencilRef(1);
    materialBBound->set_stencilFuncMask(0xff);
    std::shared_ptr<Mesh> meshBBound = std::make_shared<Mesh>(geometryB, materialBBound);
    meshBBound->SetPosition(meshB->GetPosition());
    meshBBound->SetScale(glm::vec3(1.2f));
    scene->AddChild(meshBBound);

    // 光线
    std::shared_ptr<DirectionalLight> directionalLight = std::make_shared<DirectionalLight>();
    // 光源从右后方
    directionalLight->m_direction = glm::vec3(-1.0f, -1.0f, -1.0f);
    directionalLight->set_specular_intensity(1.0f);
    std::shared_ptr<AmbientLight> ambientLight = std::make_shared<AmbientLight>();
    ambientLight->set_color(glm::vec3(0.2f));
    struct LightPack lights;
    lights.directional = directionalLight;
    lights.ambient = ambientLight;
    PerspectiveCamera camera(static_cast<float>(glApp->get_width()) / static_cast<float>(glApp->get_height()));
    camera.set_position(glm::vec3(0.0f, 0.0f, 5.0f));
    // 相机控制器
    InputDispatcher inputDispatcher(glApp);
    inputDispatcher.CreateCameraController<TrackballCameraController>(camera);
    auto cameraCtl = inputDispatcher.get_CameraController();
    cameraCtl->SetScaleSpeed(1.0f);

    glApp->InitImGui();

    // 窗体循环
    while (glApp->Update()) {
        cameraCtl->OnUpdate();

        renderer.setClearColor(glApp->get_clearColor());
        // 每一帧清一次屏
        Renderer::BeginFrame();
        renderer.render(scene, camera, lights);

        // imgui渲染
        glApp->RenderImGui();
    }
    return 0;
}
