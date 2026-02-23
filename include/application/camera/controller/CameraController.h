//
// Created by dingrui on 25-7-19.
//

#pragma once

#include "application/camera/Camera.h"

#include <unordered_map>

class CameraController
{
public:
    CameraController(Camera& camera);
    virtual ~CameraController() = default;

    /**
     * 鼠标点击事件
     * @param button 鼠标哪个按键
     * @param action 按下还是抬起
     * @param x 鼠标位置
     * @param y 鼠标位置
     */
    virtual void OnMouse(int button, int action, int mods, double x, double y);
    // 鼠标滚轮
    virtual void OnScroll(float yOffset);
    /**
     * 鼠标移动事件
     * @param x 鼠标位置
     * @param y 鼠标位置
     */
    virtual void OnCursor(double x, double y);
    /**
     * 键盘事件
     * @param key 键盘键位
     * @param action 按下还是抬起
     * @param mods 有没有ctl shift功能组合
     */
    virtual void OnKey(int key, int action, int mods);
    // 每一帧调用
    virtual void OnUpdate();

    void set_sensitivity(float sensitivity) { m_Sensitivity = sensitivity; }
    void set_scaleSpeed(float speed) { m_ScaleSpeed = speed; }

protected:
    // 相机前后转动 绕着x轴
    virtual void pitch(float angle) {};
    // 相机左右转动 绕着z轴
    virtual void yaw(float angle) {};

protected:
    // 鼠标左键被按下
    bool m_LeftButtonPressed = false;
    // 鼠标中键被按下
    bool m_MidButtonPressed = false;
    // 鼠标右键被按下
    bool m_RightButtonPressed = false;

    // 鼠标位置
    float m_CurX = 0.0f, m_CurY = 0.0f;

    // 缓存按键被按下还是抬起
    std::unordered_map<int, bool> m_KeyPressedMap;
    // 相机pitch前后转角或者yaw左右转角的敏感度
    float m_Sensitivity = 0.1f;
    // 控制的相机 Controller一生只控制一个相机 设计成引用类型
    Camera& m_Camera;
    // 相机缩放速度
    float m_ScaleSpeed = 0.2f;
    // 移动速度
    float m_MovSpeed = 1.0f;
};