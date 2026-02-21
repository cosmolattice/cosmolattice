cmake_minimum_required(VERSION 3.10)
include(src/cmake/title.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/src/cmake/
)# Tells CMake where the magic-find-my-path-functions are.
set(MODELS_PATH ./src/models/) # path to the cosmolattice models. If for some
                               # reasons you want to change that, do it there.

message(${CMAKE_MODULE_PATH})

if(NOT DEFINED CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      Release
      CACHE STRING "Build type")
  message(STATUS "No build type specified, defaulting to Release.")
endif()

project(CosmoLattice CXX)

set(CMAKE_CXX_STANDARD 20) # This is likely to evolve in future releases.
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# These are the options which are available to the user.

set(DOCUMENTATION
    OFF "Build documentation"
    CACHE BOOL "Build documentation with Doxygen (default = OFF)")
set(MPI
    OFF
    CACHE BOOL "Set to ON to build with MPI (default = OFF)")
set(DEVICE_PROVIDER
    Kokkos
    CACHE
      STRING
      "Set to the device provider to use for shared memory and/or GPU parallelization (default = Kokkos)"
)
set(PARAFAFT
    OFF
    CACHE BOOL "Set to ON to build with PARAFAFT (default = OFF)")
set(KOKKOSFFT
    ON
    CACHE
      BOOL
      "Enable KokkosFFT support (default = ON), acts as a fallback for non-MPI GPU builds"
)

set(HDF5
    OFF
    CACHE
      BOOL
      "Set to ON to build with HDF5 support for saving fields (default = OFF)")
set(TESTING
    ON
    CACHE BOOL "Set to ON to be able to build the tests (default = ON)")
set(PROFILING
    OFF
    CACHE BOOL "Set to ON to build with tools for profiling (default = OFF)")

# Not an option that was tested, so can't modify it easily
set(FLOATFFT OFF)

# Most likely, if you use external library, they won't be in the default path.
# You need to help CMake where to find them. The easiest way is to put below the
# absolute path to the folder where the library is. You can also provide them
# when calling cmake or just dire tly add them to CMAKE_PREFIX_PATH.

# Cool note: if you installed these libraries with the automated scripts in
# thedependencies subfolder, you don't need to do anything, the following line
# does it for you.
include(src/cmake/auto_install_paths.cmake)

# ##############################################################################

# if you have an exotic FFTW3, you can also help cmake by adding the path below.

set(MYFFTW3_PATH
    "insert/here"
    CACHE STRING "FFTW3 guess path")

# if you have an exotic HDF5, you can also help cmake by adding the path below.

set(MYHDF5_PATH
    "insert/here"
    CACHE STRING "HDF5 guess path")

# if you have parafaft elsewhere, help cmake by adding the path below.

set(MYPARAFAFT_PATH
    ""
    CACHE STRING "path to PARAFAFT installation directory")

# if for some reason you have other exotic installations the cmake can't find,
# just repeat the above procedure below.

# ...

# Now we add these indications to the relevant CMake variable. Btw, any path you
# add to CMAKE_PREFIX_PATH upon calling cmake will be remembered, so don't
# hesitate to continue using it.

set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${MYHDF5_PATH};${MYFFTW3_PATH}")
# Also set FFTW_ROOT so KokkosFFT's FindFFTW.cmake uses the local path
if(NOT MYFFTW3_PATH STREQUAL "insert/here")
  set(FFTW_ROOT
      "${MYFFTW3_PATH}"
      CACHE PATH "FFTW root for KokkosFFT" FORCE)
endif()
# Note: MYPARAFAFT_PATH is handled separately in configure.cmake (header-only
# library)

# ##########The CMake shenanigans are hidden in this file. You can check it out
# if interested ############################
include(src/cmake/configure.cmake)
# ##############################################################################

# ##############################################################################
# __  __           _      _    _____      _           _   _ |  \/  |         | |
# | |  / ____|    | |         | | (_)              _ | \  / | ___   __| | ___| |
# | (___   ___| | ___  ___| |_ _  ___  _ __   (_) | |\/| |/ _ \ / _` |/ _ \ |
# \___ \ / _ \ |/ _ \/ __| __| |/ _ \| '_ \ | |  | | (_) | (_| |  __/ |  ____) |
# __/ |  __/ (__| |_| | (_) | | | |  _ |_|  |_|\___/ \__,_|\___|_| |_____/
# \___|_|\___|\___|\__|_|\___/|_| |_| (_)
#
# You can change the model that is compiled by default by changing the default
# value of DEFAULT_MODEL below You can also just pass the model name to cmake as
# -DMODEL=MyModel when you call cmake.
# ##############################################################################

set(DEFAULT_MODEL lphi4)

set(MODEL
    ${DEFAULT_MODEL}
    CACHE STRING "Model that is compiled.")
message("")
message(
  STATUS
    "---------- The model which is going to be compiled is: ${MODEL} ----------"
)
set(MODEL_LABEL ${MODEL}) # Allows to customise the name below if wanted.

# Next few lines are to see if a file name Model.cmake exists at the same place
# as the model. If so, we load it, which allows you to customize the cmake
# behaviour depending on the model

find_file(MODELS_CMAKE ${MODEL}.cmake ${MODELS_PATH})
if(NOT MODELS_CMAKE)
  message(STATUS "(With no special options)")
else()
  include(${MODELS_CMAKE})
endif()

message("")

set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -DMODELTYPE=${MODEL} -DMODELINCLUDE=models/${MODEL}.h -DMODELLABEL=${MODEL_LABEL}"
)

# ##############################################################################

# Compiler options. We let them here so they are easy to access. You can move it
# in cmake/configure.cmake if you prefer.

if(DEFINED G++OPT)
  message(
    FATAL_ERROR
      "G++OPT is deprecated. Please use CMAKE_BUILD_TYPE=Release or Debug instead, and set NATIVE, SSE, and AVX as needed."
  )
endif()
if(DEFINED G++AVX)
  message(
    FATAL_ERROR
      "G++AVX is deprecated. Please use CMAKE_BUILD_TYPE=Release or Debug instead, and set NATIVE, SSE, and AVX as needed."
  )
endif()
if(DEFINED G++SSE)
  message(
    FATAL_ERROR
      "G++SSE is deprecated. Please use CMAKE_BUILD_TYPE=Release or Debug instead, and set NATIVE, SSE, and AVX as needed."
  )
endif()

set(NATIVE
    (NOT APPLE)
    CACHE STRING "Set whether to use -march=native.")
set(SSE
    OFF
    CACHE
      BOOL
      "Set the sse instruction flag for the compiler. Default is OFF, but most likely you want to set is to ON."
)
set(AVX
    OFF
    CACHE
      STRING
      "Set the avx instruction flags during compilation. Can choose between OFF, mavx, mavx2, mavx512f (default = OFF). If you want something else, feel free to change the CMakeLists."
)

# First, fix the release flags. O3 is default in CMake Release mode.
if(NATIVE)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -march=native")
endif()
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -ffast-math")

# Disable the warnings about NaN checking, if using fast-math,
if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  set(CMAKE_CXX_FLAGS_RELEASE
      "${CMAKE_CXX_FLAGS_RELEASE} -Wno-nan-infinity-disabled")
endif()

# Add debug flags: -g for debugging symbols, -fsanitize=address for address
# sanitizer, and -fconcepts-diagnostics-depth=8 for better error messages with
# concepts.
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
if(NOT (CMAKE_CXX_COMPILER_ID MATCHES "Clang"))
  set(CMAKE_CXX_FLAGS_DEBUG
      "${CMAKE_CXX_FLAGS_DEBUG} -fconcepts-diagnostics-depth=8")
endif()

if(${SSE})
  set(CMAKE_CXX_FLAGS_RELEASE
      "${CMAKE_CXX_FLAGS_RELEASE} -msse -msse2 -msse3 -msse4")
endif()

if(${AVX} STREQUAL mavx)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mavx")
elseif(${AVX} STREQUAL mavx2)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -mavx -mavx2")
elseif(${AVX} STREQUAL mavx512f)
  set(CMAKE_CXX_FLAGS_RELEASE
      "${CMAKE_CXX_FLAGS_RELEASE} -mavx -mavx2 -mavx512f")
elseif(NOT ${AVX} STREQUAL OFF)
  message(FATAL_ERROR "AVX=${AVX} is not a valid option.")
endif()

# Make sure GCC can handle huge amounts of constexpr computations.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
  set(CMAKE_CXX_FLAGS
      "${CMAKE_CXX_FLAGS} -fconstexpr-loop-limit=2621440 -fconstexpr-ops-limit=335544320"
  )
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow")

if(MPI)
  message(STATUS "MPI CXX compiler: ${MPI_CXX_COMPILER}")
endif()
message(STATUS "CXX compiler: ${CMAKE_CXX_COMPILER}")
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  message(
    STATUS
      "Build type is Release. The compiler will be called with the following flags:\n  ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}"
  )
else()
  message(
    STATUS
      "Build type is Debug. The compiler will be called with the following flags:\n  ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}"
  )
endif()

# ##############################################################################
# _____                     _        _     _ | ____|_  _____  ___ _   _| |_ __
# _| |__ | | ___  ___   _ |  _| \ \/ / _ \/ __| | | | __/ _` | '_ \| |/ _ \/ __|
# (_) | |___ >  <  __/ (__| |_| | || (_| | |_) | |  __/\__ \  _
# |_____/_/\_\___|\___|\__,_|\__\__,_|_.__/|_|\___||___/ (_)
#
# Add whatever new program here. First arg is the name, second is where the main
# is, relative to cosmolattice/src, last is the name of the binary to be
# generated. Warning: don't forget that CosmoLattice is designed to be header
# only, so this function won't work if you have more than one file to compile.

# We add the default cosmolattice. The model is given as a precompiler option
# and the corresponding .h is expected to be in the folder "models".

add_cosmolattice_executable(cosmolattice ./src/ ${MODEL_LABEL})

# ##############################################################################

# This defines a rule to refresh the cmake by calling make clean-cmake; cmake
# src-dir; .

add_custom_target(
  clean-cmake COMMAND ${CMAKE_COMMAND} -P
                      ${CMAKE_SOURCE_DIR}/src/cmake/clean-all.cmake)

# ##############################################################################

# Credits: ASCII art from http://patorjk.com/software/taag/ Font for comments:
# Big

if(TESTING)
  enable_testing()
  add_subdirectory(src/tests/)
  add_subdirectory(src/benchmarks/)
endif()
