cmake_minimum_required(VERSION 3.15)

# 项目信息
project(study-opengl
        VERSION 1.0
        LANGUAGES C CXX
)

# 输出详细日志
set(CMAKE_VERBOSE_MAKEFILE ON)

# c/cpp标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
add_compile_options(-Wall -Werror)
# 输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# dep
set(lib_directory ${CMAKE_SOURCE_DIR}/cmake)
include(${lib_directory}/spdlog.cmake)
include(${lib_directory}/OpenGL.cmake)
include(${lib_directory}/glad.cmake)
include(${lib_directory}/glfw.cmake)
include(${lib_directory}/glm.cmake)
include(${lib_directory}/stb.cmake)
include(${lib_directory}/imgui.cmake)
include(${lib_directory}/assimp.cmake)

if(NOT OPENGL_VERSION_MAJOR MATCHES "^[0-9]+$")
    message(FATAL_ERROR "OPENGL_VERSION_MAJOR must be numeric")
endif()

if(NOT OPENGL_VERSION_MINOR MATCHES "^[0-9]+$")
    message(FATAL_ERROR "OPENGL_VERSION_MINOR must be numeric")
endif()

set(LIB_NAME ${PROJECT_NAME}-core)
add_subdirectory(src)

# Enable CTest
include(CTest)
enable_testing()
add_subdirectory(tests)