//
// Created by dingrui on 25-7-23.
//

#pragma once

#include "glframework/Core.h"

// 物体抽象 平移 缩放 转动
class Object
{
public:
    Object()  = default;
    ~Object() = default;

    void SetPosition(glm::vec3 pos);

    // 增量旋转 在上一次基础上再旋转多少度
    void SetRotationX(float angle);
    void SetRotationY(float angle);
    void SetRotationZ(float angle);

    void SetScale(glm::vec3 scale);

    // 计算出model matrix
    glm::mat4 GetModelMatrix() const;

protected:
    // 位置坐标 世界坐标系
    glm::vec3 m_position{0.0f};

    // 绕着3根轴旋转的角度 旋转顺序 pitch yaw roll
    float m_rotation_x{0.0f};
    float m_rotation_y{0.0f};
    float m_rotation_z{0.0f};

    // 在xyz3个方向的缩放
    glm::vec3 m_scale{1.0f};
};
