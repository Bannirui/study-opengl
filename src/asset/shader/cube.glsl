#type vertex
// 占位用 OpenGL的真正版本在编译的时候根据平台动态替换掉
#version xxx

// 本地xyz坐标
layout (location = 0) in vec3 a_pos;
// uv坐标
layout (location = 1) in vec2 a_uv;

// 把顶点转换为世界坐标系
uniform mat4 u_model;
// 摄像机矩阵 转换成摄像机坐标系
uniform mat4 u_view;
// 透视投影矩阵 将摄像机坐标转换为剪裁坐标
uniform mat4 u_projection;

out vec2 uv;
out vec3 color;
// 世界坐标
out vec3 worldPos;

void main()
{
    // 世界坐标
    vec4 transformPos = u_model * vec4(a_pos, 1.0f);
    worldPos = transformPos.xyz;
    // 剪裁坐标
    gl_Position = u_projection * u_view * transformPos;
    uv = a_uv;
}

#type fragment
// 占位用 OpenGL的真正版本在编译的时候根据平台动态替换掉
#version xxx

// uv坐标
in vec2 uv;
// 世界坐标
in vec3 worldPos;

uniform sampler2D u_diffuse;

out vec4 fragmentColor;

void main()
{
    fragmentColor = vec4(texture(u_diffuse, uv).rgb, 1.0);
}