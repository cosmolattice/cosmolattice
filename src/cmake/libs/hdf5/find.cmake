if(MPI)
  set(HDF5_PREFER_PARALLEL ${MPI})
endif()

find_package(HDF5 COMPONENTS C)
set(CORRECT_HDF_FOUND OFF)

if(HDF5_FOUND)
  # Try to determine if HDF5 is parallel by checking for MPI support in the HDF5
  # libraries or checking the HDF5 config
  set(HDF5_PARALLEL_DETECTED FALSE)

  # Method 1: Check the HDF5_IS_PARALLEL variable (if set by FindHDF5)
  if(HDF5_IS_PARALLEL)
    set(HDF5_PARALLEL_DETECTED TRUE)
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
          break()
        endif()
      endif()
    endforeach()
  endif()

  if(MPI AND HDF5_PARALLEL_DETECTED)
    set(CORRECT_HDF_FOUND ON)
  elseif(NOT MPI AND NOT HDF5_PARALLEL_DETECTED)
    set(CORRECT_HDF_FOUND ON)
  else()
    if(MPI AND NOT HDF5_PARALLEL_DETECTED)
      message(STATUS "MPI is enabled but HDF5 is single-threaded")
    else()
      message(STATUS "MPI is disabled but HDF5 is parallel")
    endif()
    set(CORRECT_HDF_FOUND OFF)
  endif()

  message(
    STATUS
      "HDF5 version = ${HDF5_VERSION}, parallel support = ${HDF5_PARALLEL_DETECTED}"
  )
endif()
