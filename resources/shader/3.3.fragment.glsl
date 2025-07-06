// fragment shader片元处理 计算每个像素片元应该着什么颜色
// 片元处理着色阶段比较耗时
#version 330 core

uniform sampler2D texture1;
uniform sampler2D texture2;

in vec2 texPos;
// fragment收到的color并不是严格意义上的ertex shader发过来的 fragment的颜色是每个片元的颜色 是经过重心插值算法计算的结果
in vec3 color;

// out 表示输出
// vec4 输出的变量类型 四维向量 的fragment shader中向后面管线输出的是RGBA这样的四维数据
// fragColor 变量名
out vec4 fragColor;

// 系统时间 呼吸效果
uniform float systime;

void main()
{
    vec4 texColor = mix(texture(texture1, texPos), texture(texture2, texPos), 0.2f);
    fragColor = texColor * vec4(color * (sin(systime) + 1.0) / 2.0, 1.0f);
}