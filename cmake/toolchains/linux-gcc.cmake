set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER /usr/bin/gcc)
set(CMAKE_CXX_COMPILER /usr/bin/g++)

# 为平台设置宏 传递给shader编译用 区分对opengl版本的依赖
#target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_LINUX)
