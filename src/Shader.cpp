//
// Created by rui ding on 2025/6/26.
//

#include <fstream>
#include <sstream>
#include <iostream>
#include <assert.h>

#include <GLAD/glad.h>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

Shader::Shader(const char* vertexPath, const char* fragmentPath) {
    std::string vertexCode, fragmentCode;
    std::ifstream vShaderFile, fShaderFile;
    // 确保ifstream可以抛出异常
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
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
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    } catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESS: " << e.what() << std::endl;
        exit(EXIT_FAILURE);
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    // shader都会分配唯一id
    unsigned int vertex, fragment;
    // 编译vertex shader
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, nullptr);
    glCompileShader(vertex);
    checkCompileErrors(vertex, ShaderType::vs_compile);
    // 编译fragment shader
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    glCompileShader(fragment);
    checkCompileErrors(fragment, ShaderType::fs_compile);
    // 链接
    m_ID = glCreateProgram();
    // 把编译好的vertex shader和fragment shader放到程序容器中然后进行链接
    glAttachShader(m_ID, vertex);
    glAttachShader(m_ID, fragment);
    glLinkProgram(m_ID);
    checkCompileErrors(m_ID, ShaderType::program_link);
    // 回收vertex和fragment
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

Shader::~Shader() {}

void Shader::use() {
    glUseProgram(m_ID);
}

void Shader::setBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), (int)value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(m_ID, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(m_ID, name.c_str()), value);
}
void Shader::setMat4(const std::string &name, glm::mat4& mat) const {
    glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::checkCompileErrors(unsigned int shader, ShaderType type) {
    int success = 0;
    char errInfo[1024];
    switch (type) {
        case ShaderType::vs_compile:
        case ShaderType::fs_compile:
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success) glGetShaderInfoLog(shader, 1024, nullptr, errInfo);
            break;
        case ShaderType::program_link:
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success) glGetProgramInfoLog(shader, 1024, nullptr, errInfo);
        default: break;
    }
    if (success) return;
    std::cout << "ERROR::SHADER_ERROR of type: " << type << "\n" << errInfo << std::endl;
    assert(false);
}