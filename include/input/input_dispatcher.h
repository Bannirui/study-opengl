//
// Created by dingrui on 2/7/26.
//

#pragma once

#include <memory>

class Application;
class CameraController;

class InputDispatcher {
public:
    explicit InputDispatcher(Application *app);

    template<typename T, typename... Args>
    void CreateCameraController(Args &&... args) {
        m_cameraCtl = std::make_shared<T>(std::forward<Args>(args)...);
    }

    std::shared_ptr<CameraController> get_CameraController() const {
        return m_cameraCtl;
    };

    // callback
    static void OnResize(int w, int h);

    static void OnKey(int key, int scancode, int action, int mods);

    static void OnCursor(double x, double y);

    static void OnScroll(double xOffset, double yOffset);

    static void OnMouse(int button, int action, int mods);

private:
    void resize(int w, int h);

    void key(int key, int action, int mods);

    void cursor(double x, double y);

    void scroll(double x, double y);

    void mouse(int button, int action, int mods);

private:
    Application *m_app;
    std::shared_ptr<CameraController> m_cameraCtl;

    static InputDispatcher *s_instance;
};
