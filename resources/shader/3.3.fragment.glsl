// fragment shader片元处理 计算每个像素片元应该着什么颜色
// 片元处理着色阶段比较耗时
#version 330 core

// vs传过来的texture UV坐标
in vec2 uv;
// fragment收到的color并不是严格意义上的vertex shader发过来的 fragment的颜色是每个片元的颜色 是经过重心插值算法计算的结果
// in vec3 color;

// 采样器
uniform sampler2D sampler1;
uniform sampler2D sampler2;
// 系统时间 呼吸效果
uniform float systime;

// out 表示输出
// vec4 输出的变量类型 四维向量 的fragment shader中向后面管线输出的是RGBA这样的四维数据
// fragColor 变量名
out vec4 fragColor;

void main()
{
    // texture是内置函数 uv发给采样器sampler请求 返回的是四维数据
    vec4 textureColor1 = texture(sampler1, uv);
    vec4 textureColor2 = texture(sampler2, uv);
    // 混合 权重是第2个参数人权重
    vec4 texColor = mix(textureColor1, textureColor2, 0.2f);
    fragColor = texColor * vec4(0.1f, 0.2f, 0.7f, 1.0f);
}