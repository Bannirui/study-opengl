//
// Created by dingrui on 25-6-23.
//

#pragma once

// 前向声明 链接的时候到glfw中找
struct GLFWwindow;

// 窗体大小发生变化的回调 窗体大小发生变化了视口跟着变
// @Param width 变化之后的窗体宽度
// @Param height 变化之后的窗体高度
void framebuffer_size_callback(int width, int height);

// 键盘事件回调
// @Param key 字母按键码
// @Param scancode 物理按键码
// @Param action 按下还是抬起
// @Param mods 是否有shift或者ctrl
void keyboard_callback(int key, int scancode, int action, int mods);

// 监听鼠标位置变化
void cursor_position_callback(double x, double y);
// 监听鼠标滚轮缩放 glfw的缩放是两个向量必如触摸板或者侧轮 我们只关注正常滚轮
// @Param yoffset 缩放量 >0表示向上滚动 放大 <0表示向下滚动 缩小
void mouse_scroll_callback(double yoffset);

void mouse_btn_callback(int button, int action, int mods);
