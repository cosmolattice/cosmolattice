# ##############################################################################
# Get Kokkos
# ##############################################################################

message(STATUS "---------- Getting Kokkos ----------")

set(Kokkos_ENABLE_CUDA
    ${CUDA}
    CACHE BOOL "")
set(Kokkos_ENABLE_CUDA_CONSTEXPR
    ${CUDA}
    CACHE BOOL "")
set(Kokkos_ENABLE_HIP
    ${HIP}
    CACHE BOOL "")
set(Kokkos_ENABLE_OPENMP
    ${OpenMP}
    CACHE BOOL "")
set(Kokkos_ENABLE_THREADS
    ${Threads}
    CACHE BOOL "")
set(Kokkos_ENABLE_SERIAL
    ${Serial}
    CACHE BOOL "")
set(Kokkos_ENABLE_TESTS
    OFF
    CACHE BOOL "")
set(Kokkos_ENABLE_AGGRESSIVE_VECTORIZATION
    ON
    CACHE BOOL "")

include(FetchContent)
FetchContent_Declare(
  Kokkos
  DOWNLOAD_EXTRACT_TIMESTAMP FALSE
  URL https://github.com/kokkos/kokkos/releases/download/5.0.2/kokkos-5.0.2.tar.gz
  URL_HASH
    SHA256=188817bb452ca805ee8701f1c5adbbb4fb83dc8d1c50624566a18a719ba0fa5e
  SYSTEM)
FetchContent_MakeAvailable(Kokkos)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEVICE_KOKKOS")

# There is a problem with Kokkos + HIP, where HIP's cmake sets offload-arch and
# Kokkos does the same, which leads to a conflict. We purge the info set by
# HIP's cmake here.
if(HIP)
  set(GPU_BUILD_TARGETS
      ""
      CACHE STRING "" FORCE)
  set(GPU_TARGETS
      ""
      CACHE STRING "" FORCE)
endif()

message(STATUS "---------- Getting Kokkos DONE ----------\n")
