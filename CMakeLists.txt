cmake_minimum_required(VERSION 3.12)
project(lab_4 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")


add_executable(lab_4 main.c)
add_executable(pub_sym_1 pub_sym_1.c)
add_executable(pub_sym_2 pub_sym_2.c)