set(CMAKE_SYSTEM_NAME Linux)

set(CMAKE_C_COMPILER /usr/bin/clang)
set(CMAKE_CXX_COMPILER /usr/bin/clang++)

# OpenGL的API在mac上被弃用了 防止编译器warn
add_definitions(-DGL_SILENCE_DEPRECATION)
