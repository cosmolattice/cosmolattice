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
    # Debug: print the HDF5_IS_PARALLEL variable and related info
    message(STATUS "HDF5_IS_PARALLEL = ${HDF5_IS_PARALLEL}")
    message(STATUS "HDF5 version = ${HDF5_VERSION}")
    message(STATUS "HDF5_LIBRARIES = ${HDF5_LIBRARIES}")

    # Try to determine if HDF5 is parallel by checking for MPI support in the
    # HDF5 libraries or checking the HDF5 config
    set(HDF5_PARALLEL_DETECTED FALSE)

    # Method 1: Check the HDF5_IS_PARALLEL variable (if set by FindHDF5)
    if(HDF5_IS_PARALLEL)
      set(HDF5_PARALLEL_DETECTED TRUE)
      message(STATUS "Parallel HDF5 detected via HDF5_IS_PARALLEL")
    endif()

    # Method 2: Check if h5pcc (parallel HDF5 compiler wrapper) was used by
    # looking for MPI references in include directories
    if(NOT HDF5_PARALLEL_DETECTED AND HDF5_INCLUDE_DIRS)
      foreach(include_dir ${HDF5_INCLUDE_DIRS})
        if(EXISTS "${include_dir}/H5pubconf.h")
          file(STRINGS "${include_dir}/H5pubconf.h" h5_config_lines
               REGEX "#define H5_HAVE_PARALLEL")
          if(h5_config_lines)
            set(HDF5_PARALLEL_DETECTED TRUE)
            message(STATUS "Parallel HDF5 detected via H5pubconf.h")
            break()
          endif()
        endif()
      endforeach()
    endif()

    if(MPI AND HDF5_PARALLEL_DETECTED)
      message(STATUS "Found parallel HDF5")
      set(CORRECT_HDF_FOUND ON)
    elseif(NOT MPI AND NOT HDF5_PARALLEL_DETECTED)
      message(STATUS "Found single-threaded HDF5")
      set(CORRECT_HDF_FOUND ON)
    else()
      if(MPI AND NOT HDF5_PARALLEL_DETECTED)
        message(STATUS "MPI is enabled but HDF5 is single-threaded")
      else()
        message(STATUS "MPI is disabled but HDF5 is parallel")
      endif()
      set(CORRECT_HDF_FOUND OFF)
    endif()
  endif()

  if(NOT CORRECT_HDF_FOUND)
    message(
      FATAL_ERROR
        "HDF5 has been found on your system, but MPI is set to ${MPI}, while HDF5's parallel support is ${HDF5_PARALLEL_DETECTED}.
        If you want to use the system's HDF5, please make sure that it matches your MPI setting."
    )
  endif()

  include_directories(${HDF5_INCLUDE_DIRS})
endif()
