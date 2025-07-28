//
// Created by dingrui on 25-7-23.
//

#include "glframework/Object.h"

#include <iostream>
#include <ostream>

Object::Object() : m_type(ObjectType::Object) {}
Object::~Object() {}
void Object::SetPosition(glm::vec3 pos)
{
    m_position = pos;
}
void Object::SetRotationX(float angle)
{
    m_rotation_x += angle;
}
void Object::SetRotationY(float angle)
{
    m_rotation_y += angle;
}
void Object::SetRotationZ(float angle)
{
    m_rotation_z += angle;
}
void Object::SetScale(glm::vec3 scale)
{
    m_scale = scale;
}
glm::mat4 Object::GetModelMatrix() const
{
    // 先缩放 再旋转 最后平移
    // 在本地坐标系下缩放
    glm::mat4 transform = glm::mat4(1.0f);
    transform           = glm::scale(transform, m_scale);
    // 在本地坐标系下旋转 先pitch绕着x上下点头 再yaw绕着y左右 最后roll绕着z翻滚
    transform = glm::rotate(transform, glm::radians(m_rotation_x), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(m_rotation_y), glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(m_rotation_z), glm::vec3(0.0f, 0.0f, 1.0f));
    // 在世界坐标系下平移
    transform = glm::translate(glm::mat4(1.0f), m_position) * transform;
    return transform;
}
Object* Object::GetParent() const
{
    return m_parent;
}
void Object::AddChild(Object* child)
{
    // 检查是否加入过
    auto iter = std::find(m_children.begin(), m_children.end(), child);
    if (iter != m_children.end())
    {
        std::cerr << "重复添加" << std::endl;
        return;
    }
    // 加入
    m_children.push_back(child);
    // 告诉孩子 父亲是谁
    child->m_parent = this;
}
std::vector<Object*> Object::GetChildren() const
{
    return m_children;
}
