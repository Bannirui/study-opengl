// vertex shader顶点处理
// 进行三维变换 屏幕投影等各类顶点操作
// 每个顶点运行一次 离散成多个像素片元给片元shader
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
out vec2 TexCoord;

void main()
{
    gl_Position = vec4(aPos, 1.0f);
    ourColor = aColor;
    TexCoord = aTexCoord;
}