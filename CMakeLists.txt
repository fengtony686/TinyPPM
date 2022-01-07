cmake_minimum_required(VERSION 3.13)
project(PPM)

set(CMAKE_CXX_STANDARD 14)
find_package(OpenCV 3 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

FIND_PACKAGE(OpenMP REQUIRED)
if (OPENMP_FOUND)
    message("OPENMP FOUND")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
endif ()

add_executable(PPM
        utils/utils.h
        utils/utils.cpp
        utils/texture.h
        utils/texture.cpp
        utils/normal_texture.h
        utils/normal_texture.cpp
        objects/bezier2D.h
        objects/bezierObject.h
        objects/sphere.h
        objects/sphere.cpp
        objects/kdTree.cpp
        objects/kdTree.h
        objects/plane.h
        objects/plane.cpp
        objects/triangle.h
        objects/mesh.cpp
        objects/mesh.h
        objects/Object.h
        monitors/manager.h
        monitors/manager.cpp
        main.cpp)

target_link_libraries(PPM ${OpenCV_LIBS})