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

add_executable(earth_test earth_test.cpp)
target_link_libraries(earth_test PRIVATE study-opengl)
add_dependencies(earth_test copy_assets)

add_executable(box_test box_test.cpp)
target_link_libraries(box_test PRIVATE study-opengl)
add_dependencies(box_test copy_assets)

add_executable(point_light_test point_light_test.cpp)
target_link_libraries(point_light_test PRIVATE study-opengl)
add_dependencies(point_light_test copy_assets)

add_executable(spot_light_test spot_light_test.cpp)
target_link_libraries(spot_light_test PRIVATE study-opengl)
add_dependencies(spot_light_test copy_assets)

add_executable(scene_test scene_test.cpp)
target_link_libraries(scene_test PRIVATE study-opengl)
add_dependencies(scene_test copy_assets)

add_executable(model_test model_test.cpp)
target_link_libraries(model_test PRIVATE study-opengl)
add_dependencies(model_test copy_assets)

add_executable(backpack_model_test backpack_model_test.cpp)
target_link_libraries(backpack_model_test PRIVATE study-opengl)
add_dependencies(backpack_model_test copy_assets)
