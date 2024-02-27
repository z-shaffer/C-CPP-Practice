cmake_minimum_required(VERSION 3.12)
project(opengl)

set(CMAKE_CXX_STANDARD 17)

# Include directory for GLFW
include_directories("/usr/local/include/")

# Add the executable
add_executable(opengl main.cpp "./glad.c")

# Link against GLFW library
target_link_libraries(opengl "/usr/local/Cellar/glfw/3.3.9/lib/libglfw.dylib")
