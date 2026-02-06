if(HDF5)
  set(CORRECT_HDF_FOUND OFF)

  if(NOT HDF5_FOUND)
    message(STATUS "Fetching HDF5")
    include(FetchContent)
    FetchContent_Declare(
      hdf5
      URL https://github.com/HDFGroup/hdf5/releases/download/hdf5_1.14.6/hdf5-1.14.6.tar.gz
    )
    FetchContent_MakeAvailable(hdf5)
  endif()

  if(HDF5_FOUND)
    if(MPI AND HDF5_IS_PARALLEL)
      message(STATUS "Found parallel HDF5")
      set(CORRECT_HDF_FOUND ON)
    elseif(NOT MPI AND NOT HDF5_IS_PARALLEL)
      message(STATUS "Found single-threaded HDF5")
      set(CORRECT_HDF_FOUND ON)
    else()
      message(STATUS "Found single-threaded HDF5 for parallel ")
      set(CORRECT_HDF_FOUND OFF)
    endif()
  endif()

  if(NOT CORRECT_HDF_FOUND)
    message(
      WARNING
        "HDF5 has been found on your system, but MPI is set to ${MPI}, while HDF5's parallel support is ${HDF5_IS_PARALLEL}.
        If you want to use the system's HDF5, please make sure that it matches your MPI setting."
    )
  endif()

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHDF5")
  include_directories(${HDF5_INCLUDE_DIRS})
endif()
