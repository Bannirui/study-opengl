cmake_minimum_required(VERSION 3.15)

# 项目信息
project(study-opengl
        VERSION 1.0
        LANGUAGES CXX C
)

# cpp标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")

# 输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# sub directory
add_subdirectory(vendor)

# 源文件
set(source_dir "${PROJECT_SOURCE_DIR}/src")
file(GLOB_RECURSE srcs
        "${source_dir}/*.cpp"
        "${source_dir}/*.c"
)

# exe
add_executable(${PROJECT_NAME} ${srcs})

target_include_directories(${PROJECT_NAME}
        PRIVATE "${PROJECT_SOURCE_DIR}/vendor/GLFW/include"
        PUBLIC "${PROJECT_SOURCE_DIR}/include"
        PUBLIC "${PROJECT_SOURCE_DIR}/include/GLAD"
)
target_link_libraries(${PROJECT_NAME}
        PRIVATE glfw
)
