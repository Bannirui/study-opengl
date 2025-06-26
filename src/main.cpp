//
// Created by dingrui on 25-6-23.
//

#include <iostream>
#include <cmath>

// be sure to include GLAD before other header files that require OpenGL
#include <GLAD/glad.h>
#include <GLFW/glfw3.h>

#include "callback.h"

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

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
const char* vertexShaderSource = "#version 330 core\n"
                                 "layout (location=0) in vec3 aPos;\n"
                                 "layout (location=1) in vec3 aColor;\n"
                                 "out vec3 ourColor;\n"
                                 "void main()\n"
                                 "{\n"
                                 "  gl_Position = vec4(aPos, 1.0f);\n"
                                 "  ourColor = aColor;\n"
                                 "}\0";

// 计算机中图像表示的4元组RGBA(红 绿 蓝 透明度)
// 每个值[0...1]
const char* fragmentShaderSource = "#version 330 core\n"
                                   "out vec4 FragColor;\n"
                                   "in vec3 ourColor;\n"
                                   "void main()\n"
                                   "{\n"
                                   "  FragColor = vec4(ourColor, 1.0f);\n"
                                   "}\n\0";

int main()
{
    // initialize GLFW OpenGL的版本是3.3
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    // tell GLFW explicitly to use the core-profile
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    // create a window object
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (!window) {
        std::cout << "Failed to create GLFW window\n";
        glfwTerminate();
        return -1;
    }
    // 设置好上下文后就要初始化glad 后面才能安全使用glad的函数
    glfwMakeContextCurrent(window);
    // 窗口变化回调
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    // GLAD manages function pointers for OpenGL, initialize GLAD before calling any OpenGL function
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD\n";
        return -1;
    }
    // 创建shader实例
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    // 编译vertex shader程序
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    // 看看glCompileShader编译成功没有
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILE FAILED\n" << infoLog << std::endl;
        glfwTerminate();
        return -1;
    }
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    // check for shader compile error
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILE FAILED\n" << infoLog << std::endl;
        glfwTerminate();
        return -1;
    }
    // 把多个shader组合链接到shader program上 在render中调用这个shader progam
    // 创建program object
    unsigned int shaderProgram = glCreateProgram();
    // 添加上面编译过的shader
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    // 把shader链接到shader program上
    glLinkProgram(shaderProgram);
    // 看看链接执行结果
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK FAILED\n" << infoLog << std::endl;
        glfwTerminate();
        return -1;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    // 三角形的三维坐标 z被压扁 3个顶点
    float vertices[] = {
        // position        // color
        0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f, // 上
        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, // 在下
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, // 右下
    };
    // VBO(vertex buffer objects)管理顶点坐标 批量向显卡发送数据
    // OpenGL有许多buffer objects VBO是array buffer
    // VAO(vertex array object)
    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind vertex array object first, then buffer
    glBindVertexArray(VAO);
    // OpenGL开放这个函数把创建的buffer绑定到array buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // 用户态的数据拷贝到buffer
    // target 接收数据的buffer类型
    // size 要把多少byte数据传递给buffer
    // data 要传递的数据
    // usage 显卡怎么处理这些数据 GL_STREAM_DRAW数据只设置一次GPU有使用次数限制 GL_STATIC_DRAW数据只设置一次GPU可以多次使用 GL_DYNAMIC_DRAW数据会变化GPU可以多次使用
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    // 把所有坐标点传到了buffer里面 告诉shader怎么用这些数据
    // 位置
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 颜色
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // 显式调用shader program
    glUseProgram(shaderProgram);
    // render loop
    while (!glfwWindowShouldClose(window))
    {
        // check for specific key press and react accordingly every frame
        processInput(window);
        // 每一帧都要清屏 防止残留前一帧图像
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        // swap the color buffer
        glfwSwapBuffers(window);
        // check if any events are triggered
        glfwPollEvents();
    }
    // 回收资源
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    // 回收GLFW资源
    glfwTerminate();
    return 0;
}
