cmake_minimum_required(VERSION 3.12)

project(MatrixModule)

add_library(MatrixModule STATIC Matrix.cpp Vector.cpp Utils.cpp)