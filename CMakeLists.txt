cmake_minimum_required(VERSION 3.16)

project(Triangulation_ESGI)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Libraries)
add_subdirectory(MathLib)
add_subdirectory(Application)

add_subdirectory(Tests)
