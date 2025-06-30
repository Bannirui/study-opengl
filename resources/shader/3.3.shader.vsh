// vertex shader顶点处理 将输入的数据转换为NDC坐标 向后续管线阶段输出NDC坐标
// 进行三维变换 屏幕投影等各类顶点操作
// 每个顶点运行一次 离散成多个像素片元给片元shader
#version 330 core
// in 关键字 表示这是一个输入的变量
// vec3 表示一个三维向量
// aPos 变量名字
// VBO告诉了顶点数据是哪些 VAO告诉了VBO数据怎么布局的
// location 告诉VertexShader去VAO数组中哪个脚标取数据
// VBO中定义了n个顶点数据 就会起n个GPU的核执行VertexShader程序
// 每个VertexShader都从VAO中拿对应属性 再根据属性描述到VBO中拿到顶点数据
// 从VAO的0号位置拿到位置属性
layout (location = 0) in vec3 aPos;
// 从VAO的1号位置拿到颜色属性
layout (location = 1) in vec3 aColor;
// 从VAO的2号位置拿到texture属性
layout (location = 2) in vec2 aTexCoord;

out vec3 color;
out vec2 texCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // gl_Position是GLSL的内置变量 负责向后续阶段输出顶点位置处理的结果
    // 输入的本身已经是NDC坐标 不需要转换 直接就输出给gl_Position向后传
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    color = aColor;
    texCoord = aTexCoord;
}