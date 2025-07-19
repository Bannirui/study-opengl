//
// Created by dingrui on 25-7-19.
//

#pragma once

#include "application/CameraController.h"

class TrackballCameraController : public CameraController
{
public:
    TrackballCameraController(Camera* camera);
    ~TrackballCameraController() override = default;

    void OnCursor(double x, double y) override;
    void OnScroll(float yOffset) override;

private:
    void pitch(float angle);
    void yaw(float angle);

private:
    float m_MoveSpeed{0.005f};
};
