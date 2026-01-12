//
// Created by rui ding on 2025/6/26.
//

#pragma once

#include <string>
#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>

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
// main函数 跟C语言一样main是入口
// 通过location告诉输入数据怎么组织的
// vertex shader的出参是fragment shader的入参 类型是vec4
// 计算机中图像表示的4元组RGBA(红 绿 蓝 透明度)
// 每个值[0...1]

enum ShaderType
{
    vs_compile,
    fs_compile,
    program_link
};

class Shader
{
public:
    /**
     * 读取vs程序和fs程序编译链接成shader program
     * @param path程序的文件路径
     */
    Shader(const std::string& path);
    virtual ~Shader();

    // 告诉GPU接下来绘制图形使用的Shader程序是谁
    void Bind();
    // 结束使用program shader Bind和Unbind成对使用
    void Unbind();
    /**
     * 给shader设置uniform全局变量
     * 不同数据类型调用gl函数不一样 所以就给不同类型开放接口
     * 在调用glUniform之前一定要先调用glUseProgram
     * @param name uniform变量名
     * @param value 要传给uniform全局变量的内容
     */
    void setBool(const std::string& name, bool value) const;
    void setInt(const std::string& name, int value) const;
    void setFloat(const std::string& name, float value) const;
    void setFloatVec3(const std::string& name, const glm::vec3 value) const;
    // 3*3方阵
    void setMat3(const std::string& name, const float* values) const;
    // 4*4方阵
    void setMat4(const std::string& name, const float* values) const;

private:
    /**
     * @param vertexSrc vs程序
     * @param fragmentSrc fs程序
     */
    Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
    std::string ReadFile(const std::string& filepath);
    /**
     * @param source vs fs在同一个文件的源码
     * @return vs和fs源码
     */
    std::unordered_map<GLenum, std::string> preProcess(const std::string& source);
    void                                    compile(const std::unordered_map<GLenum, std::string>& shaderSources);

    /**
     * 根据平台动态替换掉shader的OpenGL版本
     * 找到源码中#version xxx关键字#version然后替换成#version 330 core
     * @param source shader的glsl源码
     * @param version 要替换成什么版本 比如330 core
     */
    static std::string replaceVersion(std::string source, const std::string& version);

private:
    // shader program的唯一id vertex shader和fragment shader编译链接之后最后要保留使用的就是program shader
    unsigned int m_Program;

private:
    static void checkCompileErrors(unsigned int shader, ShaderType type);
};