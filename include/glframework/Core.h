//
// Created by dingrui on 25-7-2.
//

#pragma once

// glad是加载显卡驱动对opengl函数的实现的 所以一定要在glfw之前引用进来
// glfw实现了窗体 在窗体创建好后就用glad加载显卡驱动函数
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// glm
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// glm提供的向量矩阵to_string()方法
#ifndef GLM_ENABLE_EXPERIMENTAL
#define GLM_ENABLE_EXPERIMENTAL
#endif
#include <glm/gtx/string_cast.hpp>