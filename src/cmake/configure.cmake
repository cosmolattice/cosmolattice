include(./src/cmake/add_cosmolattice_exec.cmake)

include_directories(src/include/)
include_directories(SYSTEM external/include/)

# Set up the device
include(${CMAKE_CURRENT_SOURCE_DIR}/src/cmake/device/device.cmake)

include(${CMAKE_CURRENT_SOURCE_DIR}/src/cmake/libs/fftw.cmake)

# Need pthread to compile the non-mpi version
set(THREADS_PREFER_PTHREAD ON)
find_package(Threads REQUIRED)

if(MPI)
  find_package(
    MPI
    COMPONENTS CXX
    REQUIRED)
  include_directories(${MPI_CXX_INCLUDE_DIRS})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_MPI")
endif()

# Parafaft configuration
if(PARAFAFT AND MPI)
  # Parafaft is header-only, just need include path
  include(${CMAKE_CURRENT_SOURCE_DIR}/src/cmake/libs/parafaft.cmake)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_PARAFAFT")
endif()

if(HDF5)
  include(./src/cmake/libs/hdf5.cmake)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_HDF5")
endif()

if(TESTING)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -DTEMPLATTEST")
endif()

if(FLOATFFT)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_FFTFLOAT")
endif()

if(NOT DEFINED FFTW_LIB
   AND NOT DEFINED KOKKOSFFT
   AND NOT DEFINED PARAFAFT)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOFFT")
endif()
