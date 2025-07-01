//
// Created by rui ding on 2025/6/26.
//

#pragma once

#include "glframework/Core.h"
#include <string>
#include <glm/fwd.hpp>

// OpenGL中万物都是3维 但是在显示器上是2维 OpenGL负责把3维坐标转换到2维像素点 这个工作由graphics pipeline负责
// 分2部分
// 3维坐标->2维坐标
// 2维坐标->像素点
// graphics pipeline的入参是三维坐标集合
// GPU处理器跑的程序就是shader 编程shader的语言是GLSL(OpenGL Shading Language)
// Vertex Data就是数组 里面放的是三维坐标
// 给OpenGL传递参数要告诉OpenGL是什么类型的参数 primitives包括GL_POINTS GL_TRIANGLES GL_LINE_STRIP
// GPU没有默认的vertex和fragment shader 所以要手动指定
// OpenGL接收的坐标点是规范化设备坐标NDC(Normalized Device Coordinates) xyz值在-1到1之间

// GLSL编写的vertex shader
// 必须先声明版本 OpenGL3.3声明的版本是330 OpenGL4.2版本声明为420
// 随后是输入输出的变量 大部分情况场景float都足够使用了 因此vecn类型是最高频使用的变量类型
// uniform变量 uniform是CPU传递给GPU的全局只读变量
// mian函数 跟C语言一样main是入口
// 通过location告诉输入数据怎么组织的
// vertex shader的出参是fragment shader的入参 类型是vec4
// 计算机中图像表示的4元组RGBA(红 绿 蓝 透明度)
// 每个值[0...1]

enum ShaderType { vs_compile, fs_compile, program_link };

class Shader
{
    public:
        Shader(const char* vertexPath, const char* fragmentPath);
        virtual ~Shader();
        // 告诉GPU接下来绘制图形使用的Shader程序是谁
        void use();
        // 结束使用program shader
        void end();
        // 给shader设置uniform全局变量
        // @Param name uniform变量名
        void setBool(const std::string& name, bool value) const;
        // 给shader设置uniform全局变量
        // @Param name uniform变量名
        void setInt(const std::string& name, int value) const;
        // 给shader设置uniform全局变量
        // @Param name uniform变量名
        void setFloat(const std::string& name, float value) const;
        // 给shader设置uniform全局变量
        // @Param name uniform变量名
        void setMat4(const std::string& name, glm::mat4& mat) const;
    public:
        // shader program的唯一id vertex shader和fragment shader编译链接之后最后要保留使用的就是program shader
        unsigned int m_ID;
    private:
        void checkCompileErrors(unsigned int shader, ShaderType type);
};
