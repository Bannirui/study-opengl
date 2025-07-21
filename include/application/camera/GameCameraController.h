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

    void pitch(float angle) override;
    void yaw(float angle) override;

private:
    float m_Pitch{0.0f};
};
