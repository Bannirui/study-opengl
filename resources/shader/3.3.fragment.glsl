#version 330 core

in vec2 uv;

// 采样器
uniform sampler2D sampler1;
uniform sampler2D sampler2;

uniform float systime;

out vec4 fragColor;

void main()
{
    // texture是内置函数 uv发给采样器sampler请求 返回的是四维数据
    // 混合 权重是第2个参数的权重
    fragColor = mix(texture(sampler1, uv), texture(sampler2, uv), 0.4f);
}