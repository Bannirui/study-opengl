//
// Created by dingrui on 25-7-2.
//

#pragma once

// glad是加载显卡驱动对opengl函数的实现的 所以一定要在glfw之前引用进来
// glfw实现了窗体 在窗体创建好后就用glad加载显卡驱动函数
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>