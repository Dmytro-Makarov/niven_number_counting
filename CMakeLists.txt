cmake_minimum_required(VERSION 3.25)
project(niven_number_counting)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -fopenmp -static-libstdc++")

find_package(MPI REQUIRED)

add_executable(niven_number_mpi
        mpi/mpi.cpp)

target_link_libraries(niven_number_mpi PUBLIC MPI::MPI_CXX)

add_executable(niven_number_openmp
        openmp/openMP.cpp)

add_executable(niven_number_sequential
        sequential/main.cpp)
