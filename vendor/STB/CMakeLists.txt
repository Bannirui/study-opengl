project(stb LANGUAGES CXX)
add_library(${PROJECT_NAME} STATIC src/stb_image.cpp)
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
