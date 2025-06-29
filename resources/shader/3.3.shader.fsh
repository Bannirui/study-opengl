// fragment shader片元处理 计算每个像素片元应该着什么颜色
// 片元处理着色阶段比较耗时
#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, TexCoord), 0.2f);
}