# ##############################################################################
# Get Kokkos
# ##############################################################################

message(STATUS "---------- Getting Kokkos ----------")

if(CUDA OR HIP)
  if(NOT DEFINED Kokkos_ARCH)
    set(Kokkos_ARCH_NATIVE
        ON
        CACHE BOOL "Enable Kokkos native architecture")
  else()
    set(Kokkos_ARCH_NATIVE
        OFF
        CACHE BOOL "Enable Kokkos native architecture")
  endif()

  if(NOT DEFINED Kokkos_ARCH_LIST)
    message(
      STATUS
        "Kokkos_ARCH_LIST not set. GPU architecture must be detectable when building Kokkos."
    )
    set(Kokkos_ARCH_LIST "")
  else()
    # prepend every element with a -D and postpend with a :BOOL=ON
    string(REPLACE ";" ";-D" Kokkos_ARCH_LIST ";${Kokkos_ARCH_LIST}")
    # remove the first element
    string(SUBSTRING "${Kokkos_ARCH_LIST}" 1 -1 Kokkos_ARCH_LIST)
    string(REPLACE ";" ":BOOL=ON " Kokkos_ARCH_LIST "${Kokkos_ARCH_LIST};")
    # remove the last element
    string(SUBSTRING "${Kokkos_ARCH_LIST}" 0 -1 Kokkos_ARCH_LIST)
  endif()
endif()

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