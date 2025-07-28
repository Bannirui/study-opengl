//
// Created by dingrui on 25-7-28.
//

#pragma once

#include <glframework/Core.h>

class Tool
{
public:
    /**
     * 外界传进来一个变换矩阵 解构出来位置 xyz的旋转 缩放
     * @param matrix 外界传进来的变换矩阵
     * @param position 解构出来的位置
     * @param rotation 解构出来的旋转角度
     * @param scale 解构出来的缩放
     */
    static void decompose(glm::mat4 matrix, glm::vec3& position, glm::vec3& eulerAngle, glm::vec3& scale);
};
