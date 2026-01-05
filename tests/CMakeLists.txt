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

add_executable(earth earth.cpp)
target_link_libraries(earth PRIVATE study-opengl)
add_dependencies(earth copy_assets)

add_executable(box box.cpp)
target_link_libraries(box PRIVATE study-opengl)
add_dependencies(box copy_assets)

add_executable(point_light point_light.cpp)
target_link_libraries(point_light PRIVATE study-opengl)
add_dependencies(point_light copy_assets)

add_executable(spot_light spot_light.cpp)
target_link_libraries(spot_light PRIVATE study-opengl)
add_dependencies(spot_light copy_assets)

add_executable(scene scene.cpp)
target_link_libraries(scene PRIVATE study-opengl)
add_dependencies(scene copy_assets)

add_executable(model model.cpp)
target_link_libraries(model PRIVATE study-opengl)
add_dependencies(model copy_assets)
