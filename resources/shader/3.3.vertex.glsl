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
layout (location = 1) in vec3 aColor;
// 从VAO的2号位置拿到texture坐标
layout (location = 2) in vec2 aTexPos;

out vec2 texPos;
out vec3 color;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float systime;
uniform float movSpeed;

void main()
{
    // gl_Position是GLSL的内置变量 负责向后续阶段输出顶点位置处理的结果
    // 输入的本身已经是NDC坐标 不需要转换 直接就输出给gl_Position向后传
    // 将顶点从模型空间转换到剪裁空间
    // projection view model是4*4矩阵 aPos是4维向量
    // aPos顶点本地坐标 gl_Position输出到光栅化阶段的顶点坐标
    // 计算顺序是从右往t commit工具栏左
    // model模型矩阵 模型->世界空间
    // view视图矩阵 世界空间->摄影机空间
    // projection投影矩阵 摄影机空间->剪裁空间
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
//     周期变化顶点的x坐标达到水平移动效果
//     float dx = 0.5;
//     float offsetX = sin(systime * movSpeed) * dx;
//     gl_Position = vec4(aPos.x + offsetX, aPos.y, aPos.z, 1.0f);

    texPos = aTexPos;
    // color往后传
    color = aColor;
}