//
// Created by dingrui on 25-7-19.
//

#pragma once

#include "CameraController.h"

class GameCameraController : public CameraController
{
public:
    GameCameraController(Camera* camera);
    ~GameCameraController() override = default;

    void OnCursor(double x, double y) override;
    void OnUpdate() override;

    void SetSpeed(float s) { m_Speed = s; }

private:
    void pitch(float angle);
    void yaw(float angle);

private:
    float m_Speed{0.0f};
    float m_Pitch{0.0f};
};
