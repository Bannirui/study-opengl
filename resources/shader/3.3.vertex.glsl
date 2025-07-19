#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_uv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform float u_systime;
uniform float u_movSpeed;

out vec2 uv;
out vec3 color;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0f);
    uv = a_uv;
}