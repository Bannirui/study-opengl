#type vertex
// 占位用 OpenGL的真正版本在编译的时候根据平台动态替换掉
#version xxx

// 本地xyz坐标
layout (location = 0) in vec2 a_pos;
// uv坐标
layout (location = 1) in vec2 a_uv;

out vec2 uv;

void main()
{
    gl_Position = vec4(a_pos.x, a_pos.y, 0.0, 1.0);
    uv = a_uv;
}

#type fragment
// 占位用 OpenGL的真正版本在编译的时候根据平台动态替换掉
#version xxx

// uv坐标
in vec2 uv;

uniform sampler2D u_screenTextureSampler;

out vec4 fragColor;

void main()
{
    fragColor = texture(u_screenTextureSampler, uv);
}