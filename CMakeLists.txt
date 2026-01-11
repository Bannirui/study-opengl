cmake_minimum_required(VERSION 3.15)

option(USE_PROXY "Enable proxy for FetchContent for file download" ON)
if (USE_PROXY)
    message(STATUS "Proxy is enabled")
    set(ENV{http_proxy} "http://127.0.0.1:7890")
    set(ENV{https_proxy} "http://127.0.0.1:7890")
    set(CMAKE_TLS_VERIFY OFF)
    set(CMAKE_DOWNLOAD_AGENT "curl")
endif ()

# 项目信息
project(study-opengl
        VERSION 1.0
        LANGUAGES C CXX
)
# 平台判断并设置默认编译器 仅在未手动指定时生效
if(NOT DEFINED CMAKE_C_COMPILER AND NOT DEFINED CMAKE_CXX_COMPILER)
    message(STATUS "No platform default compiler, start user-defined compiler")
    if(APPLE)
        message(STATUS "Detected Apple platform")
        set(CMAKE_C_COMPILER /usr/bin/clang CACHE STRING "C compiler" FORCE)
        set(CMAKE_CXX_COMPILER /usr/bin/clang++ CACHE STRING "C++ compiler" FORCE)
    elseif(UNIX)
        message(STATUS "Detected Linux platform")
        set(CMAKE_C_COMPILER /usr/bin/gcc CACHE STRING "C compiler" FORCE)
        set(CMAKE_CXX_COMPILER /usr/bin/g++ CACHE STRING "C++ compiler" FORCE)
    else()
        message(FATAL_ERROR "Unsupported platform")
    endif()
endif()
message(STATUS "Using compiler: ${CMAKE_C_COMPILER}, ${CMAKE_CXX_COMPILER}")

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
set(lib_directory ${CMAKE_SOURCE_DIR}/lib)
include(${lib_directory}/OpenGL.cmake)
include(${lib_directory}/glad.cmake)
include(${lib_directory}/glfw.cmake)
include(${lib_directory}/glm.cmake)
include(${lib_directory}/stb.cmake)
include(${lib_directory}/imgui.cmake)
include(${lib_directory}/assimp.cmake)

# 源文件
file(GLOB_RECURSE SRCS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE HDRS "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")
add_library(${PROJECT_NAME} ${SRCS})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} PUBLIC glad glfw stb glm imgui assimp)

# 拷贝资源文件到构建目录和运行目录
add_custom_target(copy_shader
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/asset" "${CMAKE_CURRENT_BINARY_DIR}/asset"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/asset" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/asset"
        COMMENT "Copying shaders to build and runtime directories"
)

# 构建时触发资源文件复制
add_dependencies(${PROJECT_NAME} copy_shader)

# Enable CTest
include(CTest)
enable_testing()
add_subdirectory(tests)