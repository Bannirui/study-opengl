// fragment shader片元处理 计算每个像素片元应该着什么颜色
// 片元处理着色阶段比较耗时
#version 330 core

// 采样器
uniform sampler2D sampler1;
uniform sampler2D sampler2;

// vs传过来的texture UV坐标
in vec2 uv;
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
    // texture是内置函数 uv发给采样器sampler请求 返回的是四维数据
    vec4 texColor = mix(texture(sampler1, uv), texture(sampler2, uv), 0.2f);
    fragColor = texColor * vec4(color * (sin(systime) + 1.0) / 2.0, 1.0f);
}