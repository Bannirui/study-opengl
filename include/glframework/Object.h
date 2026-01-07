//
// Created by dingrui on 25-7-23.
//

#pragma once

#ifdef __linux__
#include <algorithm>
#endif

#include "glframework/Core.h"

class Renderer;
class Camera;
struct LightPack;

// 区分是Object还是Mesh 决定要不要渲染
enum class ObjectType
{
    Object,
    Mesh,
    Scene,
};

// 物体抽象 平移 缩放 转动
class Object : public std::enable_shared_from_this<Object>
{
public:
    Object();
    Object(ObjectType type);
    virtual ~Object() = default;

    glm::vec3 GetPosition() const { return m_position; }
    void      SetPosition(glm::vec3 pos);

    // 增量旋转 在上一次基础上再旋转多少度
    void SetRotationX(float angle);
    void SetRotationY(float angle);
    void SetRotationZ(float angle);

    void SetScale(glm::vec3 scale);

    // 设置旋转角度
    void SetAngleX(float angle);
    void SetAngleY(float angle);
    void SetAngleZ(float angle);

    // 计算出model matrix
    glm::mat4 GetModelMatrix() const;

    // 父子关系
    std::shared_ptr<Object>                     GetParent() const;
    void                                        AddChild(const std::shared_ptr<Object> child);
    const std::vector<std::shared_ptr<Object>>& GetChildren() const;

    ObjectType GetType() const { return m_type; }
    void       RemoveChild(const std::shared_ptr<Object>& child);

    // Object是多态的 真正的渲染逻辑下沉到具体对象
    virtual void Render(const Renderer&, const Camera&, const LightPack&) const {}

protected:
    // 位置坐标 世界坐标系
    glm::vec3 m_position{0.0f};

    // 绕着3根轴旋转的角度 旋转顺序 pitch yaw roll
    float m_angle_x{0.0f};
    float m_angle_y{0.0f};
    float m_angle_z{0.0f};

    // 在xyz3个方向的缩放
    glm::vec3 m_scale{1.0f};

    // 父子关系不是所有权的拥有关系 所以就用裸指针 不要用智能指针
    // 孩子节点
    std::vector<std::shared_ptr<Object>> m_children;
    // 父亲节点
    std::weak_ptr<Object> m_parent;

    // 类型标识 将来渲染的时候看这个类型决定是不是需要渲染
    // 用const修饰语义是在构造后类型不应该再发生改变
    const ObjectType m_type;
};
