#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "err_check.h"
#include "application/Application.h"
#include "application/assimpLoader.h"
#include "application/camera/CameraController.h"
#include "application/camera/PerspectiveCamera.h"
#include "application/camera/TrackballCameraController.h"
#include "glframework/Scene.h"
#include "glframework/Texture.h"
#include "glframework/light/AmbientLight.h"
#include "glframework/light/DirectionalLight.h"
#include "glframework/light/PointLight.h"
#include "glframework/renderer/Renderer.h"
#include "glframework/renderer/light_pack.h"
#include "input/input_dispatcher.h"

int main() {
    if (!glApp->Init(1600, 800)) return -1;
    // 监听事件
    glApp->RegisterCallback();

    // 渲染器
    Renderer renderer;
    // 渲染场景
    std::shared_ptr<Scene> scene = std::make_shared<Scene>();
    auto model = AssimpLoader::load("asset/fbx/backpack/backpack.obj");
    scene->AddChild(model);
    // 有个初始角度方便观察
    // todo 现在没有生效 应该是因为加载进来的模型自带了世界坐标 我在渲染的时候并没有把模型自己的坐标系因素也加进来
    scene->SetAngleX(30.0f);
    scene->SetAngleY(30.0f);
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
