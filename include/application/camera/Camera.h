//
// Created by rui ding on 2025/7/1.
//

#pragma once

#include <glm/glm.hpp>

// 默认值
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
// 透视投影时y轴视张角
const float ZOOM = 45.0f;

class Camera {
public:
    virtual ~Camera() = default;

    glm::mat4 GetViewMatrix() const;

    virtual glm::mat4 GetProjectionMatrix() const = 0;

    virtual void scale(float scale) = 0;

    glm::vec3 get_position() const { return m_Position; }
    void set_position(const glm::vec3 &position) { m_Position = position; }
    glm::vec3 get_up() const { return m_Up; }
    void set_up(const glm::vec3 &up) { m_Up = up; }
    glm::vec3 get_right() const { return m_Right; }
    void set_right(const glm::vec3 &right) { m_Right = right; }
    float get_near() const { return m_Near; }
    void set_near(const float near) { m_Near = near; }
    float get_far() const { return m_Far; }
    void set_far(const float far) { m_Far = far; }

protected:
    Camera() = default;

protected:
    // 摄像机位置
    glm::vec3 m_Position{0.0f, 0.0f, 5.0f};
    // 摄像机的3个向量
    glm::vec3 m_Up{0.0f, 1.0f, 0.0f};
    glm::vec3 m_Right{1.0f, 0.0f, 0.0f};

    float m_Near{0.0f};
    float m_Far{0.0f};
};
