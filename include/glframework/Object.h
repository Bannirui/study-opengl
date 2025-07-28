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

    glm::vec3 GetPosition() const { return m_position; }
    void      SetPosition(glm::vec3 pos);

    // 增量旋转 在上一次基础上再旋转多少度
    void SetRotationX(float angle);
    void SetRotationY(float angle);
    void SetRotationZ(float angle);

    void SetScale(glm::vec3 scale);

    // 计算出model matrix
    glm::mat4 GetModelMatrix() const;

    // 父子关系
    Object*              GetParent() const;
    void                 AddChild(Object* child);
    std::vector<Object*> GetChildren() const;

protected:
    // 位置坐标 世界坐标系
    glm::vec3 m_position{0.0f};

    // 绕着3根轴旋转的角度 旋转顺序 pitch yaw roll
    float m_rotation_x{0.0f};
    float m_rotation_y{0.0f};
    float m_rotation_z{0.0f};

    // 在xyz3个方向的缩放
    glm::vec3 m_scale{1.0f};

    // 孩子节点
    std::vector<Object*> m_children{};
    // 父亲节点
    Object* m_parent{nullptr};
};
