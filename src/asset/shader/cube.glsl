#type vertex
// 占位用 OpenGL的真正版本在编译的时候根据平台动态替换掉
#version xxx

// 本地xyz坐标
layout (location = 0) in vec3 a_pos;

// 把顶点转换为世界坐标系
uniform mat4 u_model;
// 摄像机矩阵 转换成摄像机坐标系
uniform mat4 u_view;
// 透视投影矩阵 将摄像机坐标转换为剪裁坐标
uniform mat4 u_projection;

out vec3 uvw;

void main()
{
    // 世界坐标
    vec4 transformPos = u_model * vec4(a_pos, 1.0);
    // 剪裁坐标
    gl_Position = u_projection * u_view * transformPos;
    uvw = a_pos;
}

#type fragment
// 占位用 OpenGL的真正版本在编译的时候根据平台动态替换掉
#version xxx

// uvw坐标
in vec3 uvw;

uniform samplerCube u_cubeSampler;

out vec4 fragmentColor;

void main()
{
    fragmentColor = texture(u_cubeSampler, uvw);
}