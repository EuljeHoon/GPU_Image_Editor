cmake_minimum_required(VERSION 3.3)
project(ImageEditor)


set(CMAKE_CXX_STANDARD 11) # no explicit compiler flags if possible
set(OpenGL_GL_PREFERENCE GLVND)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})
include_directories(. lib)

add_executable(ImageEditor 
    main.cpp
    lib/glad/glad.c)

target_link_libraries(ImageEditor ${OPENGL_gl_LIBRARY} glfw)

