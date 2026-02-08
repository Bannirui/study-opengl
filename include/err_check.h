//
// Created by dingrui on 25-6-28.
//

#pragma once

#include <assert.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "x_log.h"

// 调用opengl的api后检查异常
// glGetError拿到的opengl最近的一个错误
#define CHECK_GL_ERRORS                                          \
    do                                                           \
    {                                                            \
        GLenum      errCode = glGetError();                      \
        std::string errMsg;                                      \
        if (errCode != GL_NO_ERROR)                              \
        {                                                        \
            switch (errCode)                                     \
            {                                                    \
                case GL_INVALID_ENUM:                            \
                    errMsg = "OPENGL::ERROR::INVALID ENUM";      \
                    break;                                       \
                case GL_INVALID_VALUE:                           \
                    errMsg = "OPENGL::ERROR::INVALID VALUE";     \
                    break;                                       \
                case GL_INVALID_OPERATION:                       \
                    errMsg = "OPENGL::ERROR::INVALID OPERATION"; \
                    break;                                       \
                case GL_OUT_OF_MEMORY:                           \
                    errMsg = "OPENGL::ERROR::OUT OF MEMORY";     \
                    break;                                       \
                default:                                         \
                    errMsg = "OPENGL::ERROR::UNKNOWN ERROR";     \
                    break;                                       \
            }                                                    \
            XLOG_ERROR("{}", errMsg);                            \
            assert(false);                                       \
        }                                                        \
    } while (0)

inline void clearGLErrors()
{
    while (glGetError() != GL_NO_ERROR)
    {
    }
}

#ifdef DEBUG
#define GL_CALL_AND_CHECK_ERR(fn) \
    do                            \
    {                             \
        clearGLErrors();          \
        fn;                       \
        CHECK_GL_ERRORS;          \
    } while (0)
#else
#define GL_CALL_AND_CHECK_ERR(fn) \
    do                            \
    {                             \
        fn;                       \
    } while (0)
#endif
