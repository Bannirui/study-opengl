#type vertex
#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_uv;

// 把顶点转换为世界坐标系
uniform mat4 u_model;
// 摄像机矩阵 转换成摄像机坐标系
uniform mat4 u_view;
// 透视投影矩阵 将摄像机坐标转换为剪裁坐标
uniform mat4 u_projection;

out vec2 uv;
out vec3 color;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0f);
    uv = a_uv;
}

#type fragment
#version 330 core

in vec2 uv;

// 采样器
uniform sampler2D sampler1;
uniform sampler2D sampler2;

out vec4 fragColor;

void main()
{
    // texture是内置函数 uv发给采样器sampler请求 返回的是四维数据
    // 混合 权重是第2个参数的权重
    fragColor = mix(texture(sampler1, uv), texture(sampler2, uv), 0.4f);
}