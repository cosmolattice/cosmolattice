# ##############################################################################
# Find out which backends are available
# ##############################################################################
include(CheckLanguage)

check_language(CUDA)
if (CMAKE_CUDA_COMPILER)
    set(HAVE_CUDA ON)
    set(HAVE_HIP OFF)
else ()
    set(HAVE_CUDA OFF)
    check_language(HIP)
    if (CMAKE_HIP_COMPILER)
        set(HAVE_HIP ON)
    else ()
        set(HAVE_HIP OFF)
    endif ()
endif ()

check_language(OpenMP)
find_package(OpenMP QUIET)
if (OpenMP_CXX_FOUND)
    set(HAVE_OPENMP ON)
    set(HAVE_THREADS OFF)
else ()
    set(HAVE_OPENMP OFF)
    set(HAVE_THREADS ON)
endif ()

message(STATUS "Kokkos configuration: \n    OpenMP: ${HAVE_OPENMP} \n    CUDA: ${HAVE_CUDA} \n    HIP: ${HAVE_HIP} \n    Threads: ${HAVE_THREADS}")


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
            STATUS
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

# ##############################################################################
# Build Kokkos!
# This will happen very manually - this way we have the greatest control over what is happening.
# ##############################################################################

set(KOKKOS_VERSION 4.6.01)

message(STATUS "Downloading Kokkos ${KOKKOS_VERSION}")
execute_process(
        COMMAND
        bash -c
        "mkdir -p _dep && git clone https://github.com/kokkos/kokkos.git --depth 1 --branch ${KOKKOS_VERSION} _dep/kokkos-repo  2>&1 > ${CMAKE_CURRENT_BINARY_DIR}/kokkos.log"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        OUTPUT_QUIET)

message(STATUS "Configure Kokkos...")
execute_process(
        COMMAND
        bash -c
        "mkdir -p _dep/kokkos-bin"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        OUTPUT_QUIET)
execute_process(
        COMMAND
        bash -c
        "cmake \
        -DCMAKE_CXX_FLAGS=-fPIC \
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} \
        -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/Kokkos \
        -DCMAKE_BUILD_TYPE:STRING=Release \
        -DCMAKE_CXX_STANDARD:STRING=17 \
        -DKokkos_ARCH_NATIVE:BOOL=${Kokkos_ARCH_NATIVE} \
        ${Kokkos_ARCH_LIST} \
        -DKokkos_ENABLE_CUDA:BOOL=${HAVE_CUDA} \
        -DKokkos_ENABLE_CUDA_CONSTEXPR:BOOL=${HAVE_CUDA} \
        -DKokkos_ENABLE_HIP:BOOL=${HAVE_HIP} \
        -DKokkos_ENABLE_OPENMP:BOOL=${HAVE_OPENMP} \
        -DKokkos_ENABLE_THREADS:BOOL=${HAVE_THREADS} \
        -DKokkos_ENABLE_SERIAL:BOOL=ON \
        -DKokkos_ENABLE_TESTS:BOOL=OFF \
        ../kokkos-repo 2>&1 >> ${CMAKE_CURRENT_BINARY_DIR}/kokkos.log"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/_dep/kokkos-bin
)

message(STATUS "Building Kokkos...")
execute_process(
        COMMAND
        bash -c
        "make -j8 2<&1 >> ${CMAKE_CURRENT_BINARY_DIR}/kokkos.log"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/_dep/kokkos-bin
)

message(STATUS "Installing Kokkos...")
execute_process(
        COMMAND
        bash -c
        "make install 2<&1 >> ${CMAKE_CURRENT_BINARY_DIR}/kokkos.log"
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/_dep/kokkos-bin
)

# Kokkos by default does not support extensions, so we force them off
set(CMAKE_CXX_EXTENSIONS OFF)
# Make the package available
find_package(Kokkos REQUIRED HINTS ${CMAKE_CURRENT_BINARY_DIR}/Kokkos QUIET)
