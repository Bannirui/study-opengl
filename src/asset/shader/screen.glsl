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

uniform float u_textureWidth;
uniform float u_textureHeight;

out vec4 fragColor;

vec3 blur() {
    float du = 1.0 / u_textureWidth;
    float dv = 1.0 / u_textureHeight;
    vec2 offsets[9] = vec2[](
        vec2(-du, dv),  vec2(0.0, dv),  vec2(du, dv), // left-up up right-up
        vec2(-du, 0.0), vec2(0.0, 0.0), vec2(du, 0.0), // left myself right
        vec2(-du, -dv), vec2(0.0, -dv), vec2(du, -dv) // left-bottom bottom right-bottom
    );
    float kernel[9] = float[](
        1.0, 2.0, 1.0,
        2.0, 4.0, 2.0,
        1.0, 2.0, 1.0
    );
    vec3 sumColor = vec3(0.0);
    for(int i=0; i<9; ++i) {
        vec3 sampleColor = texture(u_screenTextureSampler, uv+offsets[i]).rgb;
        sumColor += sampleColor * kernel[i];
    }
    sumColor /= 16.0;
    return sumColor;
}

void main()
{
    vec3 color = blur();
    fragColor = vec4(color, 1.0);
}