// fragment shader片元处理 计算每个像素片元应该着什么颜色
// 片元处理着色阶段比较耗时
#version 330 core
// out 表示输出
// vec4 输出的变量类型 四维向量 的fragment shader中向后面管线输出的是RGBA这样的四维数据
// fragColor 变量名
out vec4 fragColor;

in vec3 color;
in vec2 texCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    fragColor = mix(texture(texture1, texCoord), texture(texture2, texCoord), 0.2f);
}