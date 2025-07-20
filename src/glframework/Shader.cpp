//
// Created by rui ding on 2025/6/26.
//

#include "glframework/Shader.h"

#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

#include "err_check.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string   vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;
    // 确保ifstream可以抛出异常
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        // 打开源码文件
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        // 关闭文件
        vShaderFile.close();
        fShaderFile.close();
        // 转字符串
        vertexCode   = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESS: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // shader都会分配唯一id
    unsigned int vertex, fragment;
    // 创建shader程序
    vertex   = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    // 为shader程序输入代码
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    // 编译shader程序
    glCompileShader(vertex);
    glCompileShader(fragment);
    checkCompileErrors(vertex, vs_compile);
    checkCompileErrors(fragment, fs_compile);
    // program shader
    m_Program = glCreateProgram();
    // 把编译好的vertex shader和fragment shader放到程序容器中然后进行链接
    glAttachShader(m_Program, vertex);
    glAttachShader(m_Program, fragment);
    glLinkProgram(m_Program);
    checkCompileErrors(m_Program, program_link);
    // 回收vertex和fragment
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {}

void Shader::use()
{
    GL_CALL_AND_CHECK_ERR(glUseProgram(m_Program));
}

void Shader::end()
{
    // 解绑
    GL_CALL_AND_CHECK_ERR(glUseProgram(0));
}

void Shader::setBool(const std::string& name, bool value) const
{
    // 在shader程序中用uniform关键字声明了全局变量 获取到uniform变量在shader程序中的位置 更新uniform变量的值
    glUniform1i(glGetUniformLocation(m_Program, name.c_str()), (int)value);
}

void Shader::setInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(m_Program, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(m_Program, name.c_str()), value);
}

void Shader::setMat4(const std::string& name, const float* values) const
{
    // transpose 是否对传进去的数据进行转置 opengl中矩阵存储方式是列存储 glm中矩阵的存储方式是列存储 所以不需要转置
    glUniformMatrix4fv(glGetUniformLocation(m_Program, name.c_str()), 1, GL_FALSE, values);
}

void Shader::checkCompileErrors(unsigned int shader, ShaderType type)
{
    int  success = 0;
    char errInfo[1024];
    switch (type)
    {
        case vs_compile:
        case fs_compile:
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) glGetShaderInfoLog(shader, 1024, nullptr, errInfo);
            break;
        case program_link:
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) glGetProgramInfoLog(shader, 1024, nullptr, errInfo);
        default:
            break;
    }
    if (success) return;
    std::cout << "ERROR::SHADER_ERROR of type: " << type << "\n" << errInfo << std::endl;
    assert(false);
}