# 源文件
file(GLOB_RECURSE SRCS "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
set(HDR_DIR ${PROJECT_SOURCE_DIR}/include)
file(GLOB_RECURSE HDRS "${HDR_DIR}/*.h" "${HDR_DIR}/*.hpp")
add_library(${LIB_NAME}
        STATIC
        ${HDRS} ${SRCS}
)

target_include_directories(${LIB_NAME} PUBLIC ${HDR_DIR})

target_link_libraries(${LIB_NAME}
        PUBLIC glad
        PUBLIC glfw
        PUBLIC stb
        PUBLIC glm
        PUBLIC imgui
        PUBLIC assimp
)

# 拷贝资源文件到构建目录和运行目录
add_custom_target(copy_shader
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/asset" "${CMAKE_CURRENT_BINARY_DIR}/asset"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/asset" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/asset"
        COMMENT "Copying shaders to build and runtime directories"
)

# 构建时触发资源文件复制
add_dependencies(${LIB_NAME} copy_shader)
