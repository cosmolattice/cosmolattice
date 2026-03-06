cmake_minimum_required(VERSION 3.10)
project(thread_exec)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_DIRS})

find_package(OpenMP REQUIRED)

set(CMAKE_CXX_STANDARD 11)

add_executable(thread_exec thread_exec.cpp)
target_link_libraries(thread_exec ${MPI_LIBRARIES} OpenMP::OpenMP_CXX)
