# 依赖FetchContent管理三方库
include(FetchContent)
# spdlog
FetchContent_Declare(
        spdlog
        GIT_REPOSITORY	https://github.com/Bannirui/spdlog.git
        GIT_TAG 	    4397dac510274c7ccf79b5f40b1b747033c12dda
)
# 下载依赖的源码
FetchContent_MakeAvailable(spdlog)
