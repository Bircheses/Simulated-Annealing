cmake_minimum_required(VERSION 3.28)
project(PEA3)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXE_LINKER_FLAGS "-static")

include_directories(${PROJECT_SOURCE_DIR}/Headers
        ${PROJECT_SOURCE_DIR}/Utilities
)

add_executable(PEA3 main.cpp
        Sources/SimulatedAnnealing.cpp
        Headers/SimulatedAnnealing.h
        Utilities/StaticFunctions.cpp
        Sources/Counter.cpp
        Headers/Counter.h
        Sources/NearestNeighbourAlgorithm.cpp
        Headers/NearestNeighbourAlgorithm.h
        Utilities/structures.cpp)
