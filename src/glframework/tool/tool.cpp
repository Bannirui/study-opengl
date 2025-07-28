//
// Created by dingrui on 25-7-28.
//

#include "glframework/tool/tool.h"

void Tool::decompose(glm::mat4 matrix, glm::vec3& position, glm::vec3& eulerAngle, glm::vec3& scale)
{
    // 四元数 用来表示旋转变换
    glm::quat quat;
    glm::vec3 skew;
    glm::vec4 perspective;
    glm::decompose(matrix, scale, quat, position, skew, perspective);
    // 四元数->欧拉角
    glm::mat4 rotation = glm::toMat4(quat);
    glm::extractEulerAngleXYZ(rotation, eulerAngle.x, eulerAngle.y, eulerAngle.z);
    // 解构出来的结果是弧度
    eulerAngle.x = glm::degrees(eulerAngle.x);
    eulerAngle.y = glm::degrees(eulerAngle.y);
    eulerAngle.z = glm::degrees(eulerAngle.z);
}
