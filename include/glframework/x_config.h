//
// Created by dingrui on 2/7/26.
//

#pragma once

// ---------- stringify ----------
#define STR_IMPL(x) #x
#define STR(x) STR_IMPL(x)

// ---------- token concatenation ----------
#define CAT_IMPL(a, b) a##b
#define CAT(a, b) CAT_IMPL(a, b)

// ---------- GLSL version ----------
// 45
#define X_GL_VERSION_2DIGIT CAT(OPENGL_VERSION_MAJOR, OPENGL_VERSION_MINOR)
// 450
#define X_GL_VERSION_3DIGIT CAT(X_GL_VERSION_2DIGIT, 0)

// 450
#define X_GLSL_VERSION_STR STR(X_GL_VERSION_3DIGIT)

// ---------- optional helpers ----------
// 4.5
#define X_GLSL_VERSION_DOTTED \
STR(OPENGL_VERSION_MAJOR) "." STR(OPENGL_VERSION_MINOR)
