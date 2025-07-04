cmake_minimum_required(VERSION 3.15)

# 项目信息
project(study-opengl
        VERSION 1.0
        LANGUAGES CXX
)
# c/cpp标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Werror")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
# 输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# 向项目添加全局的预编译宏
add_definitions(-DDEBUG)

# sub directory
add_subdirectory(vendor/GLAD)
add_subdirectory(vendor/GLFW)
add_subdirectory(vendor/STB)

# 源文件
set(source_dir "${PROJECT_SOURCE_DIR}/src")
file(GLOB_RECURSE srcs
        "${source_dir}/*.cpp"
)

# exe
add_executable(${PROJECT_NAME} ${srcs})

target_include_directories(${PROJECT_NAME}
        PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include"
        PRIVATE vendor/GLM
)
target_link_libraries(${PROJECT_NAME}
        PRIVATE glad
        PRIVATE glfw
        PRIVATE stb
)

# 资源文件
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_CURRENT_SOURCE_DIR}/resources $<TARGET_FILE_DIR:${PROJECT_NAME}>/resources
)
