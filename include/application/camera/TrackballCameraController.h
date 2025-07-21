//
// Created by dingrui on 25-7-19.
//

#pragma once

#include "CameraController.h"

class TrackballCameraController : public CameraController
{
public:
    TrackballCameraController(Camera* camera);
    ~TrackballCameraController() override = default;

    void OnCursor(double x, double y) override;
    void OnScroll(float yOffset) override;

    void pitch(float angle) override;
    void yaw(float angle) override;
};
