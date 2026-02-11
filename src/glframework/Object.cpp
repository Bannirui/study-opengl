//
// Created by dingrui on 25-7-23.
//

#include "glframework/Object.h"

#include "x_log.h"

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

Object *Object::AddChild(std::unique_ptr<Object> child) {
    if (!child) { return nullptr; }
    // 不能加自己
    if (child.get() == this) { return nullptr; }
    // 已经是我的孩子了
    if (child->m_parent == this) { return nullptr; }
    // 检查是否加入过
    auto iter = std::find(m_children.begin(), m_children.end(), child);
    if (iter != m_children.end()) {
        XLOG_ERROR("重复添加");
        return child.get();
    }
    // 如果child原来有父节点 先从原父节点移除
    if (auto oldParent = child->m_parent) {
        oldParent->RemoveChild(child.get());
    }
    // 建立联系
    child->m_parent = this;
    Object *raw = child.get();
    m_children.push_back(std::move(child));
    return raw;
}

void Object::RemoveChild(Object *child) {
    auto it = std::remove_if(m_children.begin(), m_children.end(),
                             [child](const std::unique_ptr<Object> &ptr) { return ptr.get() == child; });
    if (it != m_children.end()) {
        m_children.erase(it, m_children.end());
    }
}
