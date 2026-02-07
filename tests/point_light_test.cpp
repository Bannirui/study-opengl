//
// Created by dingrui on 25-6-23.
//

#include <iostream>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "err_check.h"
#include "application/Application.h"
#include "application/camera/Camera.h"
#include "application/camera/CameraController.h"
#include "application/camera/PerspectiveCamera.h"
#include "application/camera/TrackballCameraController.h"
#include "glframework/Mesh.h"
#include "glframework/Texture.h"
#include "glframework/geo/Box.h"
#include "glframework/geo/Sphere.h"
#include "glframework/light/AmbientLight.h"
#include "glframework/light/DirectionalLight.h"
#include "glframework/light/PointLight.h"
#include "glframework/material/PhongMaterial.h"
#include "glframework/material/WhiteMaterial.h"
#include "glframework/renderer/Renderer.h"
#include "glframework/renderer/light_pack.h"

const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 800;

std::unique_ptr<CameraController> cameraCtl;

void framebuffer_size_callback(int width, int height) {
    std::cout << "新窗口大小 w:" << width << ", h:" << height << std::endl;
    // 视口 设置窗口中opengl负责渲染的区域
    // x y将相对窗口左下角的起始位置
    // width height渲染区域的长度 高度
    GL_CALL_AND_CHECK_ERR(glViewport(0, 0, width, height));
}

void keyboard_callback(int key, int scancode, int action, int mods) {
    std::cout << "键盘事件 键位" << static_cast<char>(key) << ", 操作" << action << ", 有没有ctrl/shift功能键" << mods
            << std::endl;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) Application::setShouldClose(true);
    if (cameraCtl) {
        cameraCtl->OnKey(key, action, mods);
    }
}

void cursor_position_callback(double x, double y) {
    std::cout << "鼠标位置发生了变化 现在的 x=" << x << ", y=" << y << std::endl;
    if (cameraCtl) {
        cameraCtl->OnCursor(x, y);
    }
}

void mouse_scroll_callback(double yoffset) {
    if (yoffset > 0) {
        std::cout << "鼠标滚轮放大 yoffset: " << yoffset << std::endl;
    } else {
        std::cout << "鼠标滚轮缩小 yoffset: " << yoffset << std::endl;
    }
    if (cameraCtl) {
        cameraCtl->OnScroll(yoffset);
    }
}

void mouse_btn_callback(int button, int action, int mods) {
    double x, y;
    glApp->GetMousePos(&x, &y);
    std::cout << "button=" << button << ", action=" << action << ", mods=" << mods << ", x=" << x << ", y=" << y
            << std::endl;
    if (cameraCtl) {
        cameraCtl->OnMouse(button, action, mods, x, y);
    }
}

// 点光跟着白球的位置 让白球运动起来 点光位置就会变化
void meshTransform(const std::vector<std::shared_ptr<Mesh> > &meshes, const struct LightPack &lights) {
    float xPos = glm::sin(glfwGetTime()) + 2.0f;
    meshes[1]->SetPosition(glm::vec3(xPos, 0.0f, 0.0f));
    if (lights.point) {
        lights.point->SetPosition(meshes[1]->GetPosition());
    }
    meshes[0]->SetRotationX(1.0f);
    meshes[0]->SetRotationY(0.2f);
}

int main() {
    if (!glApp->init(SCR_WIDTH, SCR_HEIGHT)) return -1;
    // 监听事件
    glApp->setResizeCallback(framebuffer_size_callback);
    glApp->setKeyboardCallback(keyboard_callback);
    glApp->setCursorPosCallback(cursor_position_callback);
    glApp->setScrollCallback(mouse_scroll_callback);
    glApp->setMouseBtnCallback(mouse_btn_callback);

    GL_CALL_AND_CHECK_ERR(glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT));
    // 清理画布的时候清成啥样
    GL_CALL_AND_CHECK_ERR(glClearColor(0.2f, 0.3f, 0.3f, 1.0f));

    // 给渲染器准备好的就只需要 实体 光 相机
    std::unique_ptr<Renderer> renderer = std::make_unique<Renderer>();
    // 实体
    std::vector<std::shared_ptr<Mesh> > meshes;
    // 箱子
    std::shared_ptr<Box> boxGeometry = std::make_shared<Box>();
    std::shared_ptr<PhongMaterial> boxMaterial = std::make_shared<PhongMaterial>();
    boxMaterial->set_shines(32.0f);
    boxMaterial->set_diffuse(new Texture("asset/texture/box.png", 0));
    boxMaterial->set_specular_mask(new Texture("asset/texture/sp_mask.png", 1));
    std::shared_ptr<Mesh> boxMesh = std::make_shared<Mesh>(boxGeometry, boxMaterial);
    meshes.push_back(boxMesh);
    // 白色物体
    std::shared_ptr<Sphere> whiteObjGeometry = std::make_shared<Sphere>(0.1f);
    std::shared_ptr<WhiteMaterial> whiteObjMaterial = std::make_shared<WhiteMaterial>();
    std::shared_ptr<Mesh> whiteObjMesh = std::make_shared<Mesh>(whiteObjGeometry, whiteObjMaterial);
    whiteObjMesh->SetPosition(glm::vec3(2.0f, 0.0f, 0.0f));
    meshes.push_back(whiteObjMesh);
    // 光线
    struct LightPack lights;
    std::shared_ptr<PointLight> point_light = std::make_shared<PointLight>();
    point_light->SetPosition(whiteObjMesh->GetPosition());
    std::shared_ptr<AmbientLight> ambient_light = std::make_shared<AmbientLight>();
    ambient_light->set_color(glm::vec3(0.2f));
    lights.point = point_light;
    lights.ambient = ambient_light;

    // 相机
    PerspectiveCamera camera(static_cast<float>(glApp->getWidth()) / static_cast<float>(glApp->getHeight()));
    camera.set_position(glm::vec3(0.0f, 0.0f, 5.0f));
    cameraCtl = std::make_unique<TrackballCameraController>(camera);

    // 窗体循环
    while (glApp->update()) {
        cameraCtl->OnUpdate();
        meshTransform(meshes, lights);
        // 每一帧清一次屏
        Renderer::BeginFrame();
        renderer->render(meshes, camera, lights);
    }
    // 回收资源
    glApp->destroy();
    return 0;
}
