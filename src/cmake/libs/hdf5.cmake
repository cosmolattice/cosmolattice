include(${CMAKE_CURRENT_SOURCE_DIR}/src/cmake/libs/hdf5/find.cmake)

if(NOT HDF5_FOUND OR NOT CORRECT_HDF_FOUND)
  unset(HDF5_INCLUDE_DIRS CACHE)
  unset(HDF5_C_INCLUDE_DIR CACHE)
  unset(HDF5_LIBRARIES CACHE)

  include(${CMAKE_CURRENT_SOURCE_DIR}/src/cmake/libs/hdf5/get.cmake)
  include(${CMAKE_CURRENT_SOURCE_DIR}/src/cmake/libs/hdf5/find.cmake)
  if(NOT HDF5_FOUND OR NOT CORRECT_HDF_FOUND)
    message(
      FATAL_ERROR "HDF5 libraries not found, even after building from source.")
  endif()
endif()

include_directories(${HDF5_INCLUDE_DIRS})
