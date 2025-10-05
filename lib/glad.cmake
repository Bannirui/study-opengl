# glad的代码生成依赖py和glad
find_package(Python3 REQUIRED COMPONENTS Interpreter)
set(MY_VENV "${CMAKE_BINARY_DIR}/.venv")
if (WIN32)
    set(MY_PYTHON "${MY_VENV}/bin/python.exe")
else ()
    set(MY_PYTHON "${MY_VENV}/bin/python")
endif ()
# py虚拟环境
if(NOT EXISTS "${MY_PYTHON}")
    message(STATUS "Creating virtualenv at ${MY_VENV}")
    # 创建.venv
    execute_process(
            COMMAND ${Python3_EXECUTABLE} -m venv "${MY_VENV}"
            RESULT_VARIABLE VENV_CREATE_RESULT
    )
    if(NOT VENV_CREATE_RESULT EQUAL 0)
        message(FATAL_ERROR "Failed to create virtual environment at ${MY_VENV}")
    endif()
else()
    message(STATUS "Found existing virtualenv: ${MY_PYTHON}")
endif()
# 检查glad命令
execute_process(
        COMMAND ${MY_PYTHON} -m glad --help
        RESULT_VARIABLE GLAD_RET
        OUTPUT_QUIET
        ERROR_QUIET
)
if(NOT GLAD_RET EQUAL 0)
    message(STATUS "Installing glad in venv...")
    # 安装glad到虚拟环境中
    execute_process(COMMAND ${MY_PYTHON} -m pip install --upgrade pip)
    execute_process(
            COMMAND ${MY_PYTHON} -m pip install glad --timeout 60 -i https://pypi.tuna.tsinghua.edu.cn/simple
            RESULT_VARIABLE GLAD_INSTALL_RET
            ERROR_VARIABLE GLAD_INSTALL_ERR
    )
    if(NOT GLAD_INSTALL_RET EQUAL 0)
        message(FATAL_ERROR "Failed to install glad in virtualenv:\n${GLAD_INSTALL_ERR}")
    endif()
    message(STATUS "Glad installed successfully in ${MY_VENV}")
else ()
    message(STATUS "Glad already exist in ${MY_VENV}")
endif()
# 依赖FetchContent管理三方库
include(FetchContent)
# glad
FetchContent_Declare(
        glad
        GIT_REPOSITORY	https://github.com/Bannirui/glad.git
        GIT_TAG 	    431786d8126e4f383a81e36f47b61a5d52a1c20d
)
# 下载依赖的源码
FetchContent_MakeAvailable(glad)
# glad2没有现成的代码 用py生成glad代码
set(GLAD_GENERATED_DIR "${CMAKE_BINARY_DIR}/generated/glad")
set(GLAD_GENERATOR_SCRIPT ${CMAKE_SOURCE_DIR}/cmake/glad.cmake)
set(GLAD_C_FILE "${GLAD_GENERATED_DIR}/src/glad.c")
set(GLAD_H_FILE "${GLAD_GENERATED_DIR}/include/glad/glad.h")
file(MAKE_DIRECTORY ${GLAD_GENERATED_DIR})
# glad源码文件不存在 再执行生成
if(NOT EXISTS ${GLAD_C_FILE} OR NOT EXISTS ${GLAD_H_FILE})
    message(STATUS "Glad output not found, will generate with glad2")
    add_custom_command(
            OUTPUT
            ${GLAD_GENERATED_DIR}/src/glad.c
            ${GLAD_GENERATED_DIR}/include/glad/glad.h
            COMMAND ${CMAKE_COMMAND} -E env
            http_proxy=http://127.0.0.1:7890
            https_proxy=http://127.0.0.1:7890
            ${MY_PYTHON} -m glad
            --generator c
            --spec gl
            --api gl=3.3
            --profile core
            --out-path ${GLAD_GENERATED_DIR}
            --extensions="" # glad2没做隔离 会把所有函数都生成 禁用所有扩展 避免看到4.x的高版本gl函数
            COMMENT "Generating glad loader with glad2"
            VERBATIM
    )
else()
    message(STATUS "Glad already generated, skipping generation")
endif()
# glad
add_custom_target(glad-gen
        DEPENDS ${GLAD_C_FILE} ${GLAD_H_FILE}
)
add_library(glad STATIC ${GLAD_GENERATED_DIR}/src/glad.c)
target_include_directories(glad PUBLIC ${GLAD_GENERATED_DIR}/include)
add_dependencies(glad glad-gen)