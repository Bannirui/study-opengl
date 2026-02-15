//
// Created by dingrui on 25-7-23.
//

#include "glframework/Object.h"

#include "x_log.h"

Object::Object()
    :m_type(ObjectType::Object)
{

}

Object::Object(ObjectType type) : m_type(type) {
}

glm::mat4 Object::GetModelMatrix() const {
    // 先缩放 再旋转 最后平移
    // 在本地坐标系下缩放
    glm::mat4 transform = glm::mat4(1.0f);
    transform = glm::scale(transform, m_scale);
    // 在本地坐标系下旋转 先pitch绕着x上下点头 再yaw绕着y左右 最后roll绕着z翻滚
    transform = glm::rotate(transform, glm::radians(m_angle_x), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(m_angle_y), glm::vec3(0.0f, 1.0f, 0.0f));
    transform = glm::rotate(transform, glm::radians(m_angle_z), glm::vec3(0.0f, 0.0f, 1.0f));
    // 在世界坐标系下平移
    transform = glm::translate(glm::mat4(1.0f), m_position) * transform;
    return transform;
}

void Object::AddChild(std::unique_ptr<Object> child) {
    if (!child) { return; }
    // 不能加自己
    if (child.get() == this) { return; }
    // 可能已经是我的孩子了 先从它父节点断开关系
    if (auto oldParent = child->m_parent) { oldParent->RemoveChild(child.get()); }
    // 建立联系
    child->m_parent = this;
    m_children.push_back(std::move(child));
}

void Object::RemoveChild(Object *child) {
    auto it = std::remove_if(m_children.begin(), m_children.end(),
                             [child](const std::unique_ptr<Object> &ptr) { return ptr.get() == child; });
    if (it != m_children.end()) {
        m_children.erase(it, m_children.end());
    }
}
