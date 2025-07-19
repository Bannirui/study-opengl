#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec2 a_uv;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float systime;
uniform float movSpeed;

out vec2 uv;
out vec3 color;

void main()
{
    gl_Position = projection * view * model * vec4(a_pos, 1.0f);
    uv = a_uv;
}