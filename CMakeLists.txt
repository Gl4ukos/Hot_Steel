cmake_minimum_required(VERSION 3.10)
project(OpenGLTest)

set(CMAKE_CXX_STANDARD 17)

find_package(glfw3 REQUIRED)

#GLAD source
add_library(glad external/glad/src/glad.c)

target_include_directories(glad PUBLIC
    external/glad/include
)

add_executable(simulator main.cpp utilities/Shader.cpp utilities/Meshes.cpp )

target_include_directories(simulator PRIVATE
    external/glad/include
)

target_link_libraries(simulator PRIVATE glfw glad dl)