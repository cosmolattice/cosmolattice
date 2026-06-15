# ##############################################################################
# CMake configuration for CosmoLattice
#
# Common options:
# - MODEL: the model to compile. Default: lphi4
# - COSMOINTERFACE_TEST: whether to compile the tests. Default: OFF
#
# Options passed to TempLat:
# - PARAFAFT: compile TempLat with ParaFaft support for parallel FFTs. Default: OFF
# - MPI: compile TempLat with MPI support. Default: OFF
# - HDF5: compile TempLat with HDF5 support. Default: OFF
# - TEMPLAT_TEST: enable TempLat's tests. Default: OFF
# - DEVICE_PROVIDER: switches the backend for parallelization. Default: Kokkos
#     - CUDA: compile with CUDA support for GPU parallelization. Default: OFF
#     - HIP: compile with HIP support for GPU parallelization. Default: OFF
#     - OpenMP: compile with OpenMP support for CPU parallelization. Default: OFF
#     - Threads: compile with C++ threads support for CPU parallelization. Default: OFF
#     - Serial: compile without parallelization. Default: OFF
#   By default, TempLat will try to auto-detect the system's capabilities and choose the backend.
# - NATIVE: pass --march=native to the compiler for better performance. Default: ON on non-Darwin systems, OFF on Darwin (macOS)
# - KOKKOSFFT: compile TempLat with KokkosFFT support for single-node GPU FFTs. Default: ON when CUDA or HIP support is enabled, OFF otherwise
# - TEMPLAT_BRANCH: the branch of TempLat to use. Default: main
#
# ##############################################################################

cmake_minimum_required(VERSION 3.16)

# Set the default build type to Release.
if((NOT DEFINED CMAKE_BUILD_TYPE) OR (CMAKE_BUILD_TYPE STREQUAL ""))
  set(CMAKE_BUILD_TYPE
      Release
      CACHE STRING "Build type" FORCE)
  message(STATUS "No build type specified, defaulting to Release.")
endif()

project(
  CosmoLattice
  VERSION 2.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ##############################################################################
# Get TempLat, which is the main dependency of CosmoLattice.
# ##############################################################################

include(FetchContent)
# @label:fetchcontent_templat
set(FETCHCONTENT_QUIET ON)
set(TEMPLAT_BRANCH main CACHE STRING "Branch of TempLat to use")
FetchContent_Declare(
  TempLat
  GIT_REPOSITORY https://github.com/cosmolattice/templat.git
  GIT_TAG ${TEMPLAT_BRANCH})
# @endlabel
FetchContent_MakeAvailable(TempLat)


# ##############################################################################
# Now, configure CosmoLattice
# ##############################################################################

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/title.cmake)

# Set the path to the cosmolattice models. If for some reasons you want to
# change that, do it there.
set(MODELS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/models/)

# ##############################################################################
#  __  __           _      _    _____      _           _   _
# |  \/  |         | |    | |  / ____|    | |         | | (_)              _
# | \  / | ___   __| | ___| | | (___   ___| | ___  ___| |_ _  ___  _ __   (_)
# | |\/| |/ _ \ / _` |/ _ \ |  \___ \ / _ \ |/ _ \/ __| __| |/ _ \| '_ \
# | |  | | (_) | (_| |  __/ |  ____) |  __/ |  __/ (__| |_| | (_) | | | |  _
# |_|  |_|\___/ \__,_|\___|_| |_____/ \___|_|\___|\___|\__|_|\___/|_| |_| (_)
#
# You can change the model that is compiled by default by changing the default
# value of DEFAULT_MODEL below You can also just pass the model name to cmake as
# -DMODEL=MyModel when you call cmake.
# ##############################################################################

set(DEFAULT_MODEL lphi4)
set(MODEL
    ${DEFAULT_MODEL}
    CACHE STRING "Model that is compiled.")

message(
    "${BoldGreen}━━━━━━━━━━━━━━━━━━━━━━ The model which is going to be compiled is: ${MODEL} ━━━━━━━━━━━━━━━━━━━━━${ColorReset}"
)

# This allows to customise the name of the output executable
set(MODEL_LABEL ${MODEL})

# Additional cmake configuration, model-specific.
find_file(MODELS_CMAKE ${MODEL}.cmake ${MODELS_PATH})
if(MODELS_CMAKE)

  include(${MODELS_CMAKE})
endif()

message("")

# ##############################################################################
#  _____                     _        _     _
# | ____|_  _____  ___ _   _| |_ __ _| |__ | | ___  ___   _
# |  _| \ \/ / _ \/ __| | | | __/ _` | '_ \| |/ _ \/ __| (_)
# | |___ >  <  __/ (__| |_| | || (_| | |_) | |  __/\__ \  _
# |_____/_/\_\___|\___|\__,_|\__\__,_|_.__/|_|\___||___/ (_)
#
# Here, we add the executable for the configured model. We also need to tell the
# compiler which model to compile, which is done by defining the precompiler
# variable MODELTYPE. We also define MODELINCLUDE, which is the path to the .h
# file of the model, and MODELLABEL, which is the name of the executable.
# Finally, we link against TempLat and include CosmoInterface, which is the
# heart of CosmoLattice.
# ##############################################################################

# Add the executable
add_executable(${MODEL_LABEL} source/cosmolattice.cpp)
add_custom_target(cosmolattice DEPENDS ${MODEL_LABEL})
# and tell the compiler which model to choose
target_compile_definitions(${MODEL_LABEL} PRIVATE MODELTYPE=${MODEL})
target_compile_definitions(${MODEL_LABEL}
                           PRIVATE MODELINCLUDE=models/${MODEL}.h)
target_compile_definitions(${MODEL_LABEL} PRIVATE MODELLABEL=${MODEL_LABEL})
# link against TempLat
target_link_libraries(${MODEL_LABEL} PRIVATE TempLat::TempLat)
# and include CosmoInterface
target_include_directories(
  ${MODEL_LABEL} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/
                         ${CMAKE_CURRENT_SOURCE_DIR}/)

# ##############################################################################
#  _______        _   _               
# |__   __|      | | (_)            _ 
#    | | ___  ___| |_ _ _ __   __ _(_)
#    | |/ _ \/ __| __| | '_ \ / _` |  
#    | |  __/\__ \ |_| | | | | (_| |_ 
#    |_|\___||___/\__|_|_| |_|\__, (_)
#                              __/ |  
#                             |___/   
# Tests (only for CosmoInterface) are off by default. To enable tests set
# -DCOSMOINTERFACE_TEST=ON when configuring with cmake.
# ##############################################################################

if(COSMOINTERFACE_TEST)
  enable_testing()
  add_subdirectory(tests)
endif()
