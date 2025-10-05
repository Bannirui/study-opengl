# 依赖FetchContent管理三方库
include(FetchContent)
# glfw
FetchContent_Declare(
        glfw
        GIT_REPOSITORY	https://github.com/Bannirui/glfw.git
        GIT_TAG 	    506c11ba43b901dbcc4d90449f46de67cf000af4
)
# 下载依赖的源码
FetchContent_MakeAvailable(glfw)
