//
// Created by dingrui on 2/7/26.
//

#pragma once

#include <memory>

#include "application/camera/CameraController.h"

class Input {
public:
    Input() = default;
    ~Input() = default;

    template<typename T, typename... Args>
    void CreateCameraController(Args &&... args) {
        m_cameraCtl = std::make_unique<T>(std::forward<Args>(args)...);
    }

    // 返回裸指针 unique_ptr负责声明周期管理 裸指针只是observer
    CameraController* get_CameraController() const {
        return m_cameraCtl.get();
    };

    // GLFW窗体事件->Application->Input 对事件的处理放在Input里面
	// 函数指针类型 窗体大小变化后回调的函数
    void OnResize(int w, int h) const;
	// 键盘事件
    void OnKey(int key, int scancode, int action, int mods) const;
	// 鼠标位置
    void OnCursor(double x, double y) const;
    /**
	 * 鼠标滚轮缩放 glfw的滚轮缩放有两个向量 常规的鼠标滚轮是yOffset 有的鼠标滚轮可以左右拨动 触摸板也可以左右 所以有x向量
	 * @Param yOffset 缩放量 >0表示向上滚动 放大 <0表示向下滚动 缩小
     */
    void OnScroll(double xOffset, double yOffset) const;
    // 鼠标点击
    void OnMouse(int button, int action, int mods) const;

private:
    std::unique_ptr<CameraController> m_cameraCtl;
};
