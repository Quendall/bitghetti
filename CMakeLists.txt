cmake_minimum_required(VERSION 3.12)
project(bitghetti)

# libs
add_subdirectory(libs/glfw)

add_library(glad libs/glad/src/glad.c)
target_include_directories(glad PRIVATE libs/glad/include)

# get all source files
file(GLOB_RECURSE HEADERS "engine/*h")
file(GLOB_RECURSE SOURCES "engine/*.cpp")

# source files
add_executable(${PROJECT_NAME} "main.cpp"
    ${HEADERS}
    ${SOURCES}
)

# include directories
target_include_directories(${PROJECT_NAME} PRIVATE libs/glad/include)

# link libs
target_link_libraries(${PROJECT_NAME} glad glfw)