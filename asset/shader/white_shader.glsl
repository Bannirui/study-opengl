#type vertex
#version 330 core

// 本地xyz坐标
layout (location = 0) in vec3 a_pos;
// uv坐标
layout (location = 1) in vec2 a_uv;
// 法线
layout (location = 2) in vec3 a_normal;

// 把顶点转换为世界坐标系
uniform mat4 u_model;
// 摄像机矩阵 转换成摄像机坐标系
uniform mat4 u_view;
// 透视投影矩阵 将摄像机坐标转换为剪裁坐标
uniform mat4 u_projection;

out vec2 uv;
out vec3 normal;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0f);
    uv = a_uv;
    normal = a_normal;
}

#type fragment
#version 330 core

// uv坐标
in vec2 uv;
// 法线
in vec3 normal;

out vec4 fragColor;

void main()
{
    fragColor = vec4(vec3(1.0f, 1.0f, 1.0f), 1.0f);
}