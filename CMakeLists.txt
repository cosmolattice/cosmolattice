cmake_minimum_required (VERSION 3.0)
include(src/cmake/title.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/src/cmake/)  #Tells CMake where the magic-find-my-path-functions are.
set(MODELS_PATH ./src/models/) #path to the cosmolattice models. If for some reasons you want to change that, do it there.

message(${CMAKE_MODULE_PATH})

project (CosmoLattice CXX C)

# These are the options which are available to the user.

set(MPI OFF CACHE BOOL "Set to ON to build with MPI (default = OFF)")
set(PFFT OFF CACHE BOOL "Set to ON to build with PFFT (default = OFF)")
set(HDF5 OFF CACHE BOOL "Set to ON to build with HDF5 support for saving fields (default = OFF)")
set(TESTING OFF CACHE BOOL "Set to ON to be abble to build the tests (default = OFF)")

# Not an option that was tested, so can't modify it easily
set(FLOATFFT OFF)

set(CMAKE_CXX_STANDARD 14) #This is likely to evolve in future releases.


#Most likely, if you use external library, they won't be in the default path. You need to help CMake where to find them.
#The easiest way is to put below the absolute path to the folder where the library is.
#You can also provide them when calling cmake or just dire
#tly add them to CMAKE_PREFIX_PATH.

#Cool note: if you installed these libraries with the automated scripts in thedependencies subfolder, you don't need to
# do anything, the following line does it for you.
include(src/cmake/auto_install_paths.cmake)


#########################################################################################################################
#  ____       _   _       _          ____  _____ _____ _____
# |  _ \ __ _| |_| |__   | |_ ___   |  _ \|  ___|  ___|_   _|  _
# | |_) / _` | __| '_ \  | __/ _ \  | |_) | |_  | |_    | |   (_)
# |  __/ (_| | |_| | | | | || (_) | |  __/|  _| |  _|   | |    _
# |_|   \__,_|\__|_| |_|  \__\___/  |_|   |_|   |_|     |_|   (_)
#
# ie. the folder which contains as subfolder the bin and include of PFFT.
# Example:  set(MYPPFT_PATH "/home/user/pfft/" CACHE STRING "PFFT guess path"), assuming that the library files are in
#					 /home/user/pfft/bin/  and t he include file in /home/user/pfft/include/

 set(MYPFFT_PATH "insert/here" CACHE STRING "PFFT guess path")

#########################################################################################################################


#if you have an exotic FFTW3, you can also help cmake by adding the path below.

set(MYFFTW3_PATH "insert/here" CACHE STRING "FFTW3 guess path")

#if you have an exotic HDF5, you can also help cmake by adding the path below.

set(MYHDF5_PATH "insert/here" CACHE STRING "HDF5 guess path")


#if for some reason you have other exotic installations the cmake can't find, just repeat the above procedure below.

#...

#Now we add these indications to the relevant CMake variable. Btw, any path you add to CMAKE_PREFIX_PATH upon calling
#cmake will be remembered, so don't hesitate to continue using it.

set(CMAKE_PREFIX_PATH "${CMAKE_PREFIX_PATH};${MYPFFT_PATH};${MYHDF5_PATH};${MYFFTW3_PATH}" )



###########The CMake shenanigans are hidden in this file. You can check it out if interested ############################
include(src/cmake/configure.cmake)
#########################################################################################################################

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

set(MODEL ${DEFAULT_MODEL} CACHE STRING "Model that is compiled.")
message("The model which is going to be compiled is: ${MODEL}.")
set(MODEL_LABEL ${MODEL}) #Allows to customise the name below if wanted.



#Next lines looks to see if a file name Model.cmake exists at the same place than the model. If so,
#loads it, allows you to custom the cmake behaviour depending on the model

find_file(MODELS_CMAKE ${MODEL}.cmake ${MODELS_PATH})
if( NOT MODELS_CMAKE)
  message("With no special options.")
else()
    include(${MODELS_CMAKE})
endif()

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -DMODELTYPE=${MODEL} -DMODELINCLUDE=models/${MODEL}.h -DMODELLABEL=${MODEL_LABEL}")

##########################################################################################################################


# Compiler options. We let them here so they are easy to access. You can move it in cmake/configure.cmake if you prefer.
set(G++OPT Ofast CACHE STRING "Set optimisation level of g++. Can choose between Ofast, O3, O2, O1, G (debug flag) (default = Ofast).")
set(G++SSE OFF CACHE BOOL "Set the sse instruction flags of g++. Default is OFF, but most likely you want to set is to ON.")
set(G++AVX OFF CACHE STRING "Set the avx instruction flags during compilation. Can choose between OFF, mavx, mavx2, mavx512f (default = OFF). If you want something else, feel free to change the CMakeLists.")

if(${G++OPT} STREQUAL "G")
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g ")
elseif(${G++OPT} STREQUAL "O1")
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O1 ")
elseif(${G++OPT} STREQUAL "O2")
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 ")
elseif(${G++OPT} STREQUAL "O3")
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 ")
elseif(${G++OPT} STREQUAL "Ofast")
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast ")
else()
    message(FATAL_ERROR "G++OPT=${G++OPT} is not a valid option.")
endif()
if(${G++SSE})
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse -msse2 -msse3 -msse4")
endif()
if(${G++AVX} STREQUAL mavx)
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx")
elseif(${G++AVX} STREQUAL mavx2)
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx -mavx2")
elseif(${G++AVX} STREQUAL mavx512f)
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx -mavx2 -mavx512f")
elseif(NOT ${G++AVX} STREQUAL OFF)
    message(FATAL_ERROR "G++AVX=${G++AVX} is not a valid option.")
endif()

#you can target your computer architecture by changing the arch here.
if(NOT ${G++OPT} STREQUAL "G")
    # example: set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native ")
    set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
endif()

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wshadow ")


message("The compiler will be called with the following flags: ${CMAKE_CXX_FLAGS}")




#########################################################################################################################
#    _____                     _        _     _
#   | ____|_  _____  ___ _   _| |_ __ _| |__ | | ___  ___   _
#   |  _| \ \/ / _ \/ __| | | | __/ _` | '_ \| |/ _ \/ __| (_)
#   | |___ >  <  __/ (__| |_| | || (_| | |_) | |  __/\__ \  _
#   |_____/_/\_\___|\___|\__,_|\__\__,_|_.__/|_|\___||___/ (_)
#
# Add whatever new program here. First arg is the name, second is where the main is, relative to cosmolattice/src,
#last is the name of the binary to be generated.
# Warning: don't forget that CosmoLattice is designed to be header only, so this function won't work if you have more
#          than one file to compile.


# We add the default cosmolattice. The model is given as a precompiler option and the corresponding .h is expected to
# be in the folder "models".

add_cosmolattice_executable(cosmolattice ./src/ ${MODEL_LABEL})


########################################################################################################################

#This defines a rule to refresh the cmake by calling make clean-cmake; cmake src-dir; .

add_custom_target(clean-cmake
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/src/cmake/clean-all.cmake
        )

#########################################################################################################################

#Credits: ASCII art from http://patorjk.com/software/taag/
# Font for comments: Big

if(TESTING)
enable_testing()
add_subdirectory(src/tests/)
endif()
