cmake_minimum_required(VERSION 3.22)
project(Lab_3)

set(CMAKE_CXX_STANDARD 20)

add_executable(Lab_3 src/tsv_reader.h src/sandy_matrix.h src/bmp_writer.h src/tsv_reader.cpp src/sandy_matrix.cpp src/bmp_writer.cpp src/args_parser.h src/args_parser.cpp main.cpp)
