# ##############################################################################
# Get Kokkos
# ##############################################################################

message(STATUS "---------- Getting Kokkos ----------")

# ##############################################################################
# Build Kokkos! This will happen very manually - this way we have the greatest
# control over what is happening.
# ##############################################################################

set(Kokkos_ENABLE_CUDA ${CUDA} CACHE BOOL "")
set(Kokkos_ENABLE_CUDA_CONSTEXPR ${CUDA} CACHE BOOL "")
set(Kokkos_ENABLE_HIP ${HIP} CACHE BOOL "")
set(Kokkos_ENABLE_OPENMP ${OpenMP} CACHE BOOL "")
set(Kokkos_ENABLE_THREADS ${Threads} CACHE BOOL "")
set(Kokkos_ENABLE_SERIAL ${Serial} CACHE BOOL "")
set(Kokkos_ENABLE_TESTS OFF CACHE BOOL "")

include(FetchContent)
FetchContent_Declare(
    Kokkos
    DOWNLOAD_EXTRACT_TIMESTAMP FALSE
    URL      https://github.com/kokkos/kokkos/releases/download/5.0.2/kokkos-5.0.2.tar.gz
    URL_HASH SHA256=188817bb452ca805ee8701f1c5adbbb4fb83dc8d1c50624566a18a719ba0fa5e
    SYSTEM
)
FetchContent_MakeAvailable(Kokkos)

message(STATUS "---------- Getting Kokkos DONE ----------\n")