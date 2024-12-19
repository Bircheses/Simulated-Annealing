cmake_minimum_required(VERSION 3.28)
project(PEA3)

set(CMAKE_CXX_STANDARD 17)

add_executable(PEA3 main.cpp
        SimulatedAnnealing.cpp
        SimulatedAnnealing.h)
