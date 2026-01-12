# 向项目添加全局的预编译宏
add_definitions(-DDEBUG)

if (APPLE)
    # OpenGL的API在mac上被弃用了 防止编译器warn
    add_definitions(-DGL_SILENCE_DEPRECATION)
endif ()

# 拷贝资源文件到构建目录和运行目录
add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/asset" "${CMAKE_CURRENT_BINARY_DIR}/asset"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/asset" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/asset"
        COMMENT "Copying assets to build and runtime directories"
)

# Collect all cpp files in this directory
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "*.cpp")

foreach (test_src ${TEST_SOURCES})
    # Extract file name without extension
    get_filename_component(test_name ${test_src} NAME_WE)
    # Create executable
    add_executable(${test_name} ${test_src})
    # Link main library
    target_link_libraries(${test_name}
            PRIVATE ${PROJECT_NAME}
    )
    # 构建时触发资源文件复制
    add_dependencies(${test_name} copy_assets)
    # Register with CTest
    add_test(NAME ${test_name}
            COMMAND ${test_name}
    )
endforeach ()