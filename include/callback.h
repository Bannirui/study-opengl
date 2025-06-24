//
// Created by dingrui on 25-6-23.
//

#pragma once

// forward declare
struct GLFWwindow;
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// 检测按键
// esc关闭窗口
void processInput(GLFWwindow* window);