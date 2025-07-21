#type vertex
#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_uv;
layout (location = 2) in vec3 a_normal;

// 把顶点转换为世界坐标系
uniform mat4 u_model;
// 摄像机矩阵 转换成摄像机坐标系
uniform mat4 u_view;
// 透视投影矩阵 将摄像机坐标转换为剪裁坐标
uniform mat4 u_projection;

out vec2 uv;
out vec3 color;
out vec3 normal;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0f);
    uv = a_uv;
    normal = a_normal;
}

#type fragment
#version 330 core

in vec2 uv;
in vec3 normal;

// 采样器
uniform sampler2D u_sampler;
// 光源参数
uniform vec3 u_lightDirection;
uniform vec3 u_lightColor;

out vec4 fragColor;

void main()
{
    // 采样
    vec3 objColor = texture(u_sampler, uv).rgb;
    // 法线归一
    vec3 normalN = normalize(normal);
    vec3 lightDirN = normalize(u_lightDirection);
    // 计算漫反射 过滤负数 保证输出在0到1之间
    float diffuse = clamp(dot(-lightDirN, normalN), 0.0f, 1.0f);
    vec3 finalColor = u_lightColor * diffuse * objColor;
    fragColor = vec4(finalColor, 1.0f);
}