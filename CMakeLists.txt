cmake_minimum_required(VERSION 3.19)
project(Proyecto2)

set(CMAKE_CXX_STANDARD 20)

add_executable(Proyecto2 main.cpp glad.c stb_image.h stb_image.cpp Objeto.cpp Objeto.h BoundingVolume.cpp BoundingVolume.h)

target_link_libraries(Proyecto2 -lOpenGL32 -lGLU32 -lglfw3 -lGLEW32)