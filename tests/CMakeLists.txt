# 拷贝资源文件到构建目录和运行目录
add_custom_target(test_copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/asset" "${CMAKE_CURRENT_BINARY_DIR}/asset"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/asset" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/asset"
        COMMENT "Copying assets to build and runtime directories"
)

# Collect all cpp files in this directory
file(GLOB TEST_SOURCES CONFIGURE_DEPENDS "*.cpp")

foreach (TEST_SRC ${TEST_SOURCES})
    # Extract file name without extension
    get_filename_component(TEST_NAME ${TEST_SRC} NAME_WE)
    message(STATUS "Start building ${TEST_NAME}")
    # Create executable
    add_executable(${TEST_NAME} ${TEST_SRC})
    # Link main library
    target_link_libraries(${TEST_NAME}
            PRIVATE ${LIB_NAME}
    )
    # 构建时触发资源文件复制
    add_dependencies(${TEST_NAME} test_copy_assets)
    # Register with CTest
    add_test(NAME ${TEST_NAME}
            COMMAND ${TEST_NAME}
    )
endforeach ()