cmake_minimum_required(VERSION 3.12)
project(bitghetti)

# libs
add_subdirectory(libs/glad/cmake)
add_subdirectory(libs/glfw)

# source files
add_executable(bitghetti main.cpp)

# link libs
target_link_libraries(bitghetti 'glad C' glfw)