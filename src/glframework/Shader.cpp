//
// Created by rui ding on 2025/6/26.
//

#include "glframework/Shader.h"

#include <fstream>
#include <sstream>
#include <cstring>
#include <array>
#include <vector>

#include "err_check.h"
#include "x_log.h"
#include "glframework/x_config.h"


static GLenum ShaderTypeFromString(const std::string &type) {
    if (type == "vertex") return GL_VERTEX_SHADER;
    if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;
    return 0;
}

Shader::Shader(const std::string &path) {
    std::string source = ReadFile(path);
    auto shaderSources = preProcess(source);
    compile(shaderSources);
    // 从shader源码文件名中提取名字
    auto lastSlash = path.find_last_of("/\\");
    lastSlash = lastSlash == std::string::npos ? 0 : lastSlash + 1;
    auto lastDot = path.rfind('.');
    auto count = lastDot == std::string::npos ? path.size() - lastSlash : lastDot - lastSlash;
    auto name = path.substr(lastSlash, count);
    (void) name;
}

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
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
    } catch (std::ifstream::failure &e) {
        XLOG_ERROR("SHADER::FILE_NOT_SUCCESS: {}", e.what());
        exit(EXIT_FAILURE);
    }
    const char *vShaderCode = vertexCode.c_str();
    const char *fShaderCode = fragmentCode.c_str();
    // shader都会分配唯一id
    unsigned int vertex, fragment;
    // 创建shader程序
    vertex = glCreateShader(GL_VERTEX_SHADER);
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    // 为shader程序输入代码
    GL_CALL_AND_CHECK_ERR(glShaderSource(vertex, 1, &vShaderCode, nullptr));
    glShaderSource(fragment, 1, &fShaderCode, nullptr);
    // 编译shader程序
    glCompileShader(vertex);
    glCompileShader(fragment);
    checkCompileErrors(vertex, vs_compile);
    checkCompileErrors(fragment, fs_compile);
    // program shader
    m_Program = glCreateProgram();
    // 把编译好的vertex shader和fragment shader放到程序容器中然后进行链接
    GL_CALL_AND_CHECK_ERR(glAttachShader(m_Program, vertex));
    glAttachShader(m_Program, fragment);
    glLinkProgram(m_Program);
    checkCompileErrors(m_Program, program_link);
    // 回收vertex和fragment
    GL_CALL_AND_CHECK_ERR(glDeleteShader(vertex));
    GL_CALL_AND_CHECK_ERR(glDeleteShader(fragment));
}

Shader::~Shader() {
    GL_CALL_AND_CHECK_ERR(glDeleteProgram(m_Program));
}

void Shader::Bind() {
    GL_CALL_AND_CHECK_ERR(glUseProgram(m_Program));
}

void Shader::Unbind() {
    // 解绑
    GL_CALL_AND_CHECK_ERR(glUseProgram(0));
}

void Shader::setBool(const std::string &name, bool value) const {
    // 在shader程序中用uniform关键字声明了全局变量 获取到uniform变量在shader程序中的位置 更新uniform变量的值
    glUniform1i(glGetUniformLocation(m_Program, name.c_str()), (int) value);
}

void Shader::setInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(m_Program, name.c_str()), value);
}

void Shader::setFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(m_Program, name.c_str()), value);
}

void Shader::setFloatVec3(const std::string &name, const glm::vec3 value) const {
    glUniform3fv(glGetUniformLocation(m_Program, name.c_str()), 1, &value[0]);
}

void Shader::setMat3(const std::string &name, const float *values) const {
    // transpose 是否对传进去的数据进行转置 opengl中矩阵存储方式是列存储 glm中矩阵的存储方式是列存储 所以不需要转置
    glUniformMatrix3fv(glGetUniformLocation(m_Program, name.c_str()), 1, GL_FALSE, values);
}

void Shader::setMat4(const std::string &name, const float *values) const {
    // transpose 是否对传进去的数据进行转置 opengl中矩阵存储方式是列存储 glm中矩阵的存储方式是列存储 所以不需要转置
    glUniformMatrix4fv(glGetUniformLocation(m_Program, name.c_str()), 1, GL_FALSE, values);
}

void Shader::checkCompileErrors(unsigned int shader, ShaderType type) {
    int success = 0;
    char errInfo[1024];
    switch (type) {
        case vs_compile:
        case fs_compile:
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if (!success)
                glGetShaderInfoLog(shader, 1024, nullptr, errInfo);
            break;
        case program_link:
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if (!success)
                glGetProgramInfoLog(shader, 1024, nullptr, errInfo);
        default:
            break;
    }
    if (success) { return; }
    XLOG_ERROR("SHADER_ERR of type: {}, {}", static_cast<int>(type), errInfo);
    assert(false);
}

std::string Shader::ReadFile(const std::string &filepath) {
    std::string result;
    std::ifstream in(filepath, std::ios::in | std::ios::binary);
    if (in) {
        in.seekg(0, std::ios::end);
        result.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&result[0], result.size());
        in.close();
    }
    return result;
}

std::unordered_map<GLenum, std::string> Shader::preProcess(const std::string &source) {
    std::string modifiedSource = source;
    // 动态替换#version占位符
    if (modifiedSource.find("#version") != std::string::npos) {
        XLOG_INFO("shader using OpenGL:{}", X_GL_VER_STR);
        modifiedSource = replaceVersion(modifiedSource, X_GL_VERSION_STR);
    }

    std::unordered_map<GLenum, std::string> shaderSources;
    const char *typeToken = "#type";
    size_t typeTokenLength = strlen(typeToken);
    size_t pos = modifiedSource.find(typeToken, 0);

    while (pos != std::string::npos) {
        size_t eol = modifiedSource.find_first_of("\r\n", pos);
        size_t begin = pos + typeTokenLength + 1;
        std::string type = modifiedSource.substr(begin, eol - begin);
        size_t nextLinePos = modifiedSource.find_first_not_of("\r\n", eol);
        pos = modifiedSource.find(typeToken, nextLinePos);
        shaderSources[ShaderTypeFromString(type)] = (pos == std::string::npos)
                                                        ? modifiedSource.substr(nextLinePos)
                                                        : modifiedSource.substr(nextLinePos, pos - nextLinePos);
    }
    return shaderSources;
}

void Shader::compile(const std::unordered_map<GLenum, std::string> &shaderSources) {
    GLuint program = glCreateProgram();
    std::array<GLenum, 2> glShaderIDs;
    int glShaderIDIndex = 0;
    for (auto &kv: shaderSources) {
        GLenum type = kv.first;
        const std::string &source = kv.second;
        // 创建shader对象
        GLuint shader = glCreateShader(type);
        const GLchar *srcCStr = source.c_str();
        // 源码给shader对象
        GL_CALL_AND_CHECK_ERR(glShaderSource(shader, 1, &srcCStr, 0));
        // 编译shader
        GL_CALL_AND_CHECK_ERR(glCompileShader(shader));
        // 检查编译结果
        GLint isCompiled = 0;
        GL_CALL_AND_CHECK_ERR(glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled));
        if (isCompiled == GL_FALSE) {
            GLint maxLength = 0;
            GL_CALL_AND_CHECK_ERR(glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength));
            std::vector<GLchar> infoLog(maxLength);
            GL_CALL_AND_CHECK_ERR(glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]));
            GL_CALL_AND_CHECK_ERR(glDeleteShader(shader));
            XLOG_ERROR("{}", infoLog.data());
        }
        GL_CALL_AND_CHECK_ERR(glAttachShader(program, shader));
        glShaderIDs[glShaderIDIndex++] = shader;
    }
    m_Program = program;
    // 链接成shader program
    GL_CALL_AND_CHECK_ERR(glLinkProgram(program));
    GLint isLinked = 0;
    GL_CALL_AND_CHECK_ERR(glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked));
    if (isLinked == GL_FALSE) {
        GLint maxLength = 0;
        GL_CALL_AND_CHECK_ERR(glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength));
        std::vector<GLchar> infoLog(maxLength);
        GL_CALL_AND_CHECK_ERR(glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]));
        GL_CALL_AND_CHECK_ERR(glDeleteProgram(program));
        for (auto id: glShaderIDs) {
            GL_CALL_AND_CHECK_ERR(glDeleteShader(id));
        }
        return;
    }
    for (auto id: glShaderIDs) {
        GL_CALL_AND_CHECK_ERR(glDetachShader(program, id));
        GL_CALL_AND_CHECK_ERR(glDeleteShader(id));
    }
}

std::string Shader::replaceVersion(std::string source, const std::string &versionStr) {
    const char *versionToken = "#version";
    size_t versionPos = source.find(versionToken, 0);
    while (versionPos != std::string::npos) {
        size_t eol = source.find_first_of("\r\n", versionPos);
        source.replace(versionPos, eol - versionPos, versionStr);
        size_t nextLinePos = source.find_first_not_of("\r\n", eol);
        versionPos = source.find(versionToken, nextLinePos);
    }
    return source;
}
