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
set(FETCHCONTENT_QUIET ON)
FetchContent_Declare(
  TempLat
  GIT_REPOSITORY https://github.com/cosmolattice/templat.git
  GIT_TAG main)
FetchContent_MakeAvailable(TempLat)

# ##############################################################################
# Now, configure CosmoLattice
# ##############################################################################

include(${CMAKE_CURRENT_SOURCE_DIR}/src/cmake/title.cmake)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/include/)

# Set the path to the cosmolattice models. If for some reasons you want to
# change that, do it there.
set(MODELS_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src/models/)

#########################################################################################################################
#__  __           _      _    _____      _           _   _
#|  \/  |         | |    | |  / ____|    | |         | | (_)              _
#| \  / | ___   __| | ___| | | (___   ___| | ___  ___| |_ _  ___  _ __   (_)
#| |\/| |/ _ \ / _` |/ _ \ |  \___ \ / _ \ |/ _ \/ __| __| |/ _ \| '_ \
#| |  | | (_) | (_| |  __/ |  ____) |  __/ |  __/ (__| |_| | (_) | | | |  _
#|_|  |_|\___/ \__,_|\___|_| |_____/ \___|_|\___|\___|\__|_|\___/|_| |_| (_)
#
# You can change the model that is compiled by default by changing the default value of DEFAULT_MODEL below
# You can also just pass the model name to cmake as -DMODEL=MyModel when you call cmake.
#########################################################################################################################

set(DEFAULT_MODEL lphi4)
set(MODEL
    ${DEFAULT_MODEL}
    CACHE STRING "Model that is compiled.")

message("")
message(
  STATUS
    "${Green}---------- The model which is going to be compiled is: ${MODEL} ----------${ColorReset}"
)

# This allows to customise the name of the output executable
set(MODEL_LABEL ${MODEL})

# Additional cmake configuration, model-specific.
find_file(MODELS_CMAKE ${MODEL}.cmake ${MODELS_PATH})
if(NOT MODELS_CMAKE)
  message(STATUS "(With no special options)")
else()
  include(${MODELS_CMAKE})
endif()

message("")

# Add the executable
add_executable(${MODEL_LABEL} src/cosmolattice.cpp)
add_custom_target(cosmolattice DEPENDS ${MODEL_LABEL})
# and tell the compiler which model to choose
target_compile_definitions(${MODEL_LABEL} PRIVATE MODELTYPE=${MODEL})
target_compile_definitions(${MODEL_LABEL}
                           PRIVATE MODELINCLUDE=models/${MODEL}.h)
target_compile_definitions(${MODEL_LABEL} PRIVATE MODELLABEL=${MODEL_LABEL})
# link against TempLat
target_link_libraries(${MODEL_LABEL} PUBLIC TempLat::TempLat)
