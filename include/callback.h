//
// Created by dingrui on 25-6-23.
//

#pragma once

// 前向声明 链接的时候到glfw中找
struct GLFWwindow;

// 窗体大小发生变化的回调 窗体大小发生变化了视口跟着变
// @Param window 哪个窗体对象发生了大小变化
// @Param width 变化之后的窗体宽度
// @Param height 变化之后的窗体高度
void framebuffer_size_callback(GLFWwindow* window, int width, int height);

// 键盘事件回调
// @Param window 哪个窗体
// @Param key 字母按键码
// @Param scancode 物理按键码
// @Param action 按下还是抬起
// @Param mods 是否有shift或者ctrl
void keyboard_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// 检测按键
// esc关闭窗口
void processInput(GLFWwindow* window);