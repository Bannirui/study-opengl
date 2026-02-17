//
// Created by dingrui on 25-7-23.
//

#pragma once

#ifdef __linux__
#include <algorithm>
#include <memory>
#endif

#include "glframework/Core.h"

class Renderer;
class Camera;
struct LightPack;

// 物体抽象 平移 缩放 转动
class Object
{
public:
    // 区分是Object还是Mesh 决定要不要渲染
    enum class ObjectType : uint8_t
    {
        kObject,
        kMesh,
        kScene,
    };

public:
    Object();
    virtual ~Object() = default;

    glm::vec3 get_position() const { return m_position; }
    void      set_position(glm::vec3 pos) { m_position = pos; }
    // 增量旋转 在上一次基础上再旋转多少度
    void set_rotationX(float angle) { m_angle_x += angle; }
    void set_rotationY(float angle) { m_angle_y += angle; }
    void set_rotationZ(float angle) { m_angle_z += angle; }
    void set_scale(glm::vec3 scale) { m_scale = scale; }
    // 设置旋转角度
    void SetAngleX(float angle) { m_angle_x = angle; }
    void SetAngleY(float angle) { m_angle_y = angle; }
    void SetAngleZ(float angle) { m_angle_z = angle; }

    // 计算出model matrix
    glm::mat4 GetModelMatrix() const;

    // 父子关系
    Object* get_parent() const { return m_parent; }

    void AddChild(std::unique_ptr<Object> child);

    const std::vector<std::unique_ptr<Object> >& get_children() const { return m_children; }
    ObjectType                                   get_type() const { return m_type; }

    void RemoveChild(Object* child);

    // Object是多态的 真正的渲染逻辑下沉到具体对象
    virtual void Render(const Renderer&, const Camera&, const LightPack&) {}

protected:
    explicit Object(ObjectType type);

protected:
    // 位置坐标 世界坐标系
    glm::vec3 m_position{0.0f};

    // 绕着3根轴旋转的角度 旋转顺序 pitch yaw roll
    float m_angle_x{0.0f};
    float m_angle_y{0.0f};
    float m_angle_z{0.0f};

    // 在xyz3个方向的缩放
    glm::vec3 m_scale{1.0f};

private:
    // 孩子节点
    std::vector<std::unique_ptr<Object> > m_children;
    // 父亲节点
    Object* m_parent{nullptr};

    // 类型标识 将来渲染的时候看这个类型决定是不是需要渲染
    // 用const修饰语义是在构造后类型不应该再发生改变
    const ObjectType m_type;
};
