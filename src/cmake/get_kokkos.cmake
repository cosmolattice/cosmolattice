# ##############################################################################
# Find out which backends are available
# ##############################################################################
include(CheckLanguage)

option(CUDA "Enable CUDA support" ON)
option(HIP "Enable HIP support" ON)
option(OpenMP "Enable OpenMP support" ON)

if(CUDA)
  # Let's see if we have a CUDA compiler
  check_language(CUDA)
  if(CMAKE_CUDA_COMPILER)
    set(CUDA ON)
    set(HIP OFF)
  else()
    set(CUDA OFF)
  endif()
endif()

if(HIP AND NOT CUDA)
  # Let's see if we have a HIP compiler
  check_language(HIP)
  if(CMAKE_HIP_COMPILER)
    set(HIP ON)
  else()
    set(HIP OFF)
  endif()
endif()

if(OpenMP
   AND NOT CUDA
   AND NOT HIP)
  check_language(OpenMP)
  find_package(OpenMP QUIET)
  if(OpenMP_CXX_FOUND)
    set(OPENMP ON)
    set(THREADS OFF)
  else()
    set(OPENMP OFF)
  endif()
else()
  set(OPENMP OFF)
endif()

if(NOT CUDA
   AND NOT HIP
   AND NOT OPENMP)
  set(THREADS ON)
else()
  set(THREADS OFF)
endif()

message(
  STATUS
    "Kokkos configuration: \n    CUDA: ${CUDA} \n    HIP: ${HIP} \n    OpenMP: ${OPENMP} \n    Threads: ${THREADS}"
)

# ##############################################################################
# Get Kokkos
# ##############################################################################

if(GPU)
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

set(KOKKOS_VERSION 4.7.00)

message(STATUS "Downloading Kokkos ${KOKKOS_VERSION}")
execute_process(
  COMMAND
    bash -c
    "mkdir -p _dep && git clone https://github.com/kokkos/kokkos.git --depth 1 --branch ${KOKKOS_VERSION} _dep/kokkos-repo  2>&1 > ${CMAKE_CURRENT_BINARY_DIR}/kokkos.log"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  OUTPUT_QUIET)

message(STATUS "Configure Kokkos...")
execute_process(
  COMMAND bash -c "mkdir -p _dep/kokkos-bin"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  OUTPUT_QUIET)
execute_process(
  COMMAND
    bash -c "cmake \
        -DCMAKE_CXX_FLAGS=-fPIC \
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} \
        -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/Kokkos \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} \
        -DKokkos_ARCH_NATIVE=${Kokkos_ARCH_NATIVE} \
        ${Kokkos_ARCH_LIST} \
        -DKokkos_ENABLE_CUDA=${CUDA} \
        -DKokkos_ENABLE_CUDA_CONSTEXPR=${CUDA} \
        -DKokkos_ENABLE_HIP=${HIP} \
        -DKokkos_ENABLE_OPENMP=${OPENMP} \
        -DKokkos_ENABLE_THREADS=${THREADS} \
        -DKokkos_ENABLE_SERIAL=ON \
        -DKokkos_ENABLE_TESTS=OFF \
        ../kokkos-repo &>> ${CMAKE_CURRENT_BINARY_DIR}/kokkos_config.log"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/_dep/kokkos-bin)

message(STATUS "Building Kokkos...")
execute_process(
  COMMAND bash -c "make -j &>> ${CMAKE_CURRENT_BINARY_DIR}/kokkos_build.log"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/_dep/kokkos-bin)

message(STATUS "Installing Kokkos...")
execute_process(
  COMMAND bash -c
          "make install &>> ${CMAKE_CURRENT_BINARY_DIR}/kokkos_install.log"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/_dep/kokkos-bin)

# Kokkos by default does not support extensions, so we force them off
set(CMAKE_CXX_EXTENSIONS OFF)
# Make the package available

# ##############################################################################
# If profiling is requested, we add the kokkos-tools repository
# ##############################################################################
set(PROFILING
    OFF
    CACHE BOOL
          "Set to ON to build with Kokkos tools for profiling (default = OFF)")
if(DEFINED PROFILING)
  if(NOT PROFILING)
    message(STATUS "Profiling is disabled, not downloading Kokkos tools.")
    return()
  endif()
else()
  message(STATUS "Profiling is enabled, downloading Kokkos tools.")

  set(KOKKOS_TOOLS_VERSION develop)

  message(STATUS "Downloading Kokkos tools ${KOKKOS_TOOLS_VERSION}")
  execute_process(
    COMMAND
      bash -c
      "mkdir -p _dep && git clone https://github.com/kokkos/kokkos-tools.git --depth 1 --branch ${KOKKOS_TOOLS_VERSION} _dep/kokkos-tools-repo  2>&1 > ${CMAKE_CURRENT_BINARY_DIR}/kokkos_tools.log"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    OUTPUT_QUIET)

  message(STATUS "Configure Kokkos tools...")
  execute_process(
    COMMAND bash -c "mkdir -p _dep/kokkos-tools-bin"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    OUTPUT_QUIET)
  execute_process(
    COMMAND
      bash -c "cmake -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} \
           -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/Kokkos-Tools \
           ../kokkos-tools-repo \
           2>&1 >> ${CMAKE_CURRENT_BINARY_DIR}/kokkos_tools.log"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/_dep/kokkos-tools-bin)

  message(STATUS "Building Kokkos tools...")
  execute_process(
    COMMAND bash -c
            "make -j8 2>&1 >> ${CMAKE_CURRENT_BINARY_DIR}/kokkos_tools.log"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/_dep/kokkos-tools-bin)

  message(STATUS "Installing Kokkos tools...")
  execute_process(
    COMMAND bash -c
            "make install 2>&1 >> ${CMAKE_CURRENT_BINARY_DIR}/kokkos_tools.log"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/_dep/kokkos-tools-bin)
endif()
