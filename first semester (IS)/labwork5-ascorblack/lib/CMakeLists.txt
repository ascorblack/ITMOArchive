cmake_minimum_required(VERSION 3.25)
project(Array3D)

set(SOURCES uint17_t.h uint17_t.cpp ArrayData.h ArrayData.cpp
        Array1D.h Array1D.cpp Array2D.h Array2D.cpp Array3D.h Array3D.cpp)

add_library(${PROJECT_NAME} ${SOURCES})
