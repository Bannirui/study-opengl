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

# 获取资源文件列表 递归获取所有resources子文件
file(GLOB_RECURSE RESOURCE_FILES "${CMAKE_CURRENT_SOURCE_DIR}/resources/*")
# 目标拷贝目录
set(RESOURCE_OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/resources")
# 添加 目标运行目录
set(RESOURCE_RUNTIME_DIR "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources")
# 自定义命令 拷贝并生成哑文件用于触发依赖
add_custom_command(
        OUTPUT "${RESOURCE_OUTPUT_DIR}/.copied"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${CMAKE_CURRENT_SOURCE_DIR}/resources"
            "${RESOURCE_OUTPUT_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory
            "${RESOURCE_OUTPUT_DIR}"
            "${RESOURCE_RUNTIME_DIR}"
        COMMAND ${CMAKE_COMMAND} -E touch "${RESOURCE_OUTPUT_DIR}/.copied"
        DEPENDS ${RESOURCE_FILES}
        COMMENT "Copying updated resources to both build and runtime directories"
)
# 自定义目标 执行上面的命令
add_custom_target(copy_resources ALL
        DEPENDS "${RESOURCE_OUTPUT_DIR}/.copied"
)
# 添加依赖 确保拷贝指令在主程序构建前完成
add_dependencies(${PROJECT_NAME} copy_resources)