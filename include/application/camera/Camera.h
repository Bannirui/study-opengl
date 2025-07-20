//
// Created by rui ding on 2025/7/1.
//

#pragma once

#include <memory>
#include <glad/glad.h>
#include <glm/glm.hpp>

// 默认值
const float YAW         = -90.0f;
const float PITCH       = 0.0f;
const float SPEED       = 2.5f;
const float SENSITIVITY = 0.1f;
// 透视投影时y轴视张角
const float ZOOM = 45.0f;

class Camera
{
public:
    // 摄像机位置
    glm::vec3 m_Position{0.0f, 0.0f, 5.0f};
    // 摄像机的3个向量
    glm::vec3 m_Front{0.0f, 0.0f, -1.0f};
    glm::vec3 m_Up{0.0f, 1.0f, 0.0f};
    glm::vec3 m_Right{1.0f, 0.0f, 0.0f};

    float m_Near{0.0f};
    float m_Far{0.0f};

public:
    Camera();
    virtual ~Camera();

    glm::mat4 GetViewMatrix();

    virtual glm::mat4 GetProjectionMatrix() = 0;
    virtual void      scale(float scale)    = 0;
};