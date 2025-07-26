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
else()
    message(STATUS "Using user-defined compiler: ${CMAKE_C_COMPILER}, ${CMAKE_CXX_COMPILER}")
endif()

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
include(${CMAKE_SOURCE_DIR}/cmake/glad.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/glfw.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/glm.cmake)
include(${CMAKE_SOURCE_DIR}/cmake/stb.cmake)
# 源文件
file(GLOB_RECURSE srcs "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE hdrs "${CMAKE_CURRENT_SOURCE_DIR}/include/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp")

set(my_exec_targets earth box point_light spot_light)

foreach(target IN LISTS my_exec_targets)
    # 添加头文件的目的仅仅是为了让IDEA识别到 新建文件的时候添加索引
    add_executable(${target} ${target}.cpp ${srcs} ${hdrs})
    target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(${target} PRIVATE glad glfw stb glm)

    if(UNIX)
        target_link_libraries(${target} PRIVATE dl)
    elseif(WIN32)
        target_link_libraries(${target} PRIVATE opengl32)
    endif()

    # 构建时触发资源文件复制
    add_dependencies(${target} copy_resources)
endforeach()

# 向项目添加全局的预编译宏
add_definitions(-DDEBUG)

if (APPLE)
    # OpenGL的API在mac上被弃用了 防止编译器warn
    add_definitions(-DGL_SILENCE_DEPRECATION)
endif()

# 拷贝资源文件到构建目录和运行目录
add_custom_target(copy_resources
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/resources"
            "${CMAKE_CURRENT_BINARY_DIR}/resources"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/resources"
            "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources"
        COMMENT "Copying assets to build and runtime directories"
)