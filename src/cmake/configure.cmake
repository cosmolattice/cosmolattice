include(./src/cmake/add_cosmolattice_exec.cmake)

include_directories(src/include/)
include_directories(SYSTEM external/include/)

# Set up documentation
include(./src/cmake/doxygen.cmake)

# Set up the device
include(./src/cmake/device/device.cmake)

find_package(FFTW)

# Need pthread to compile the non-mpi version
set(THREADS_PREFER_PTHREAD ON)
find_package(Threads REQUIRED)

if(NOT MPI)
	#set(CMAKE_CXX_COMPILER ${CXX_COMPILER})
else()
	find_package(MPI COMPONENTS C CXX REQUIRED)
	# set(CMAKE_CXX_COMPILER ${MPI_CXX_COMPILER})
endif()

# Fourier transformation things
if(PFFT AND  MPI)
	find_package(PFFT REQUIRED)
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${PFFT_INCLUDES}")
elseif (NOT MPI)
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -DNOMPI -DNOPFFT")
elseif (NOT PFFT AND  MPI)
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -DNOPFFT")
endif()

# Parafaft configuration
if(PARAFAFT AND MPI)
	# Parafaft is header-only, just need include path
	include(${CMAKE_CURRENT_SOURCE_DIR}/src/cmake/parafaft.cmake)
elseif(NOT PARAFAFT)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOPARAFAFT")
endif()

if(HDF5)
	if(MPI)
		set(HDF5_PREFER_PARALLEL ON)
	endif()
	find_package(HDF5 REQUIRED)
	include(./src/cmake/libs/hdf5.cmake)
	#set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${HDF5_INCLUDE_DIRS} -DHDF5")
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHDF5")
	include_directories(${HDF5_INCLUDE_DIRS})
endif()


# if(HDF5) if(MPI) set(HDF5_PREFER_PARALLEL ON) endif() find_package(HDF5
# REQUIRED) # set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${HDF5_INCLUDE_DIRS}
# -DHDF5") set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHDF5")
# include_directories(${HDF5_INCLUDE_DIRS}) endif()

if(TESTING)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -DTEMPLATTEST")
endif()

if(NOT FLOATFFT)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOFFTFLOAT")
endif()

# set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${FFTW_INCLUDES}" )
include_directories(${FFTW_INCLUDES})

if(MPI)
  message(STATUS "MPI CXX compiler: ${MPI_CXX_COMPILER}")
endif()
message(STATUS "CXX compiler: ${CMAKE_CXX_COMPILER}")
