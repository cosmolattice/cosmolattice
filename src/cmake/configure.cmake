include(./src/cmake/add_cosmolattice_exec.cmake)

include_directories(src/include/)
include_directories(SYSTEM external/include/)

# Set up documentation
include(./src/cmake/doxygen.cmake)

# Set up the device
include(./src/cmake/device/device.cmake)

find_package(FFTW REQUIRED)

# Ensure the FFTW library directory is in the linker search path.
# This is needed because KokkosFFT may propagate -lfftw3 as a bare flag
# (without a full path), so the linker needs to know where to find it.
if(FFTW_LIB)
	get_filename_component(FFTW_LIB_DIR "${FFTW_LIB}" DIRECTORY)
	link_directories(${FFTW_LIB_DIR})
endif()

# Need pthread to compile the non-mpi version
set(THREADS_PREFER_PTHREAD ON)
find_package(Threads REQUIRED)

if(MPI)
	find_package(MPI COMPONENTS C CXX REQUIRED)
	include_directories(${MPI_CXX_INCLUDE_DIRS})
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_MPI")
endif()

# Fourier transformation things
if(PFFT AND  MPI)
	find_package(PFFT REQUIRED)
	include_directories(${PFFT_INCLUDES})
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_PFFT")
endif()

# Parafaft configuration
if(PARAFAFT AND MPI)
	# Parafaft is header-only, just need include path
	include(${CMAKE_CURRENT_SOURCE_DIR}/src/cmake/parafaft.cmake)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_PARAFAFT")
endif()

if(HDF5)
	if(MPI)
		set(HDF5_PREFER_PARALLEL ON)
	endif()
	find_package(HDF5 REQUIRED)
	include(./src/cmake/libs/hdf5.cmake)
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_HDF5")
	include_directories(${HDF5_INCLUDE_DIRS})
endif()

if(TESTING)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -DTEMPLATTEST")
endif()

if(FLOATFFT)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHAVE_FFTFLOAT")
endif()

if(NOT DEFINED FFTW_LIB AND NOT DEFINED KOKKOSFFT AND NOT DEFINED PFFT AND NOT DEFINED PARAFAFT)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DNOFFT")
endif()

# set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${FFTW_INCLUDES}" )
include_directories(${FFTW_INCLUDES})
