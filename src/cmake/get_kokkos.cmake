# ##############################################################################
# Find out which backends are available
# ##############################################################################
include(CheckLanguage)

check_language(CUDA)
if(CMAKE_CUDA_COMPILER)
    set(HAVE_CUDA ON)
    set(HAVE_HIP OFF)
else()
    set(HAVE_CUDA OFF)
    check_language(HIP)
    if(CMAKE_HIP_COMPILER)
        set(HAVE_HIP ON)
    else()
        set(HAVE_HIP OFF)
    endif()
endif()

check_language(OpenMP)
find_package(OpenMP)
if(OpenMP_CXX_FOUND)
    set(HAVE_OPENMP ON)
    set(HAVE_THREADS OFF)
else()
    set(HAVE_OPENMP OFF)
    set(HAVE_THREADS ON)
endif()
# if ENABLE_MPI is not set, set it to OFF
if(NOT DEFINED ENABLE_MPI)
    find_package(MPI)
    if(MPI_FOUND)
        set(ENABLE_MPI
                ON
                CACHE BOOL "Enable MPI support")
    else()
        set(ENABLE_MPI
                OFF
                CACHE BOOL "Enable MPI support")
    endif()
endif()

message(STATUS "MPI: ${ENABLE_MPI}")
message(STATUS "OpenMP: ${HAVE_OPENMP}")
message(STATUS "CUDA: ${HAVE_CUDA}")
message(STATUS "HIP: ${HAVE_HIP}")
message(STATUS "Threads: ${HAVE_THREADS}")

# ##############################################################################
# Get Kokkos
# ##############################################################################

if (NOT DEFINED Kokkos_ARCH)
    set(Kokkos_ARCH_NATIVE
            ON
            CACHE BOOL "Enable Kokkos native architecture")
else ()
    set(Kokkos_ARCH_NATIVE
            OFF
            CACHE BOOL "Enable Kokkos native architecture")
endif ()

if (NOT DEFINED Kokkos_ARCH_LIST)
    message(
            WARNING
            "Kokkos_ARCH_LIST not set. GPU architecture must be detectable when building Kokkos."
    )
    set(Kokkos_ARCH_LIST "")
else ()
    # prepend every element with a -D and postpend with a :BOOL=ON
    string(REPLACE ";" ";-D" Kokkos_ARCH_LIST ";${Kokkos_ARCH_LIST}")
    # remove the first element
    string(SUBSTRING "${Kokkos_ARCH_LIST}" 1 -1 Kokkos_ARCH_LIST)
    string(REPLACE ";" ":BOOL=ON " Kokkos_ARCH_LIST "${Kokkos_ARCH_LIST};")
    # remove the last element
    string(SUBSTRING "${Kokkos_ARCH_LIST}" 0 -1 Kokkos_ARCH_LIST)
endif ()

# add the Kokkos dependency to the build
include(ExternalProject)
ExternalProject_Add(
        kokkos_dep
        GIT_REPOSITORY https://github.com/kokkos/kokkos.git
        GIT_TAG 4.6.01
        PREFIX ${CMAKE_CURRENT_BINARY_DIR}/kokkos-build
        INSTALL_DIR ${CMAKE_CURRENT_BINARY_DIR}/kokkos
        CMAKE_ARGS -DCMAKE_CXX_FLAGS=-fPIC
        CMAKE_CACHE_ARGS
        -DCMAKE_BUILD_TYPE:STRING=Release
        -DCMAKE_CXX_STANDARD:STRING=17
        -DKokkos_ARCH_NATIVE:BOOL=${Kokkos_ARCH_NATIVE}
        ${Kokkos_ARCH_LIST}
        -DKokkos_ENABLE_CUDA:BOOL=${HAVE_CUDA}
        -DKokkos_ENABLE_CUDA_CONSTEXPR:BOOL=${HAVE_CUDA}
        -DKokkos_ENABLE_HIP:BOOL=${HAVE_HIP}
        -DKokkos_ENABLE_OPENMP:BOOL=${HAVE_OPENMP}
        -DKokkos_ENABLE_THREADS:BOOL=${HAVE_THREADS}
        -DKokkos_ENABLE_SERIAL:BOOL=ON
        -DKokkos_ENABLE_TESTS:BOOL=OFF
)