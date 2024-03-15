cmake_minimum_required(VERSION 3.12)
project(bitghetti)

# libs
add_subdirectory(libs/glfw)

add_library(glad libs/glad/src/glad.c)
target_include_directories(glad PRIVATE libs/glad/include)

# source files
add_executable(bitghetti main.cpp)

# include directories
target_include_directories(bitghetti PRIVATE libs/glad/include)

# link libs
target_link_libraries(bitghetti glad glfw)