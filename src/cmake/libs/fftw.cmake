include(${CMAKE_CURRENT_SOURCE_DIR}/src/cmake/libs/fftw/find.cmake)

# Tell about the found FFTW configuration
if(FFTW_LIBRARIES)
  message(STATUS "Found FFTW libraries: ${FFTW_LIBRARIES}")
else()
  include(${CMAKE_CURRENT_SOURCE_DIR}/src/cmake/libs/fftw/get.cmake)
  include(${CMAKE_CURRENT_SOURCE_DIR}/src/cmake/libs/fftw/find.cmake)
  if(FFTW_LIBRARIES)
    message(STATUS "Found FFTW libraries: ${FFTW_LIBRARIES}")
  else()
    message(
      FATAL_ERROR "FFTW libraries not found, even after building from source.")
  endif()
endif()

# Ensure the FFTW library directory is in the linker search path. This is needed
# because KokkosFFT may propagate -lfftw3 as a bare flag (without a full path),
# so the linker needs to know where to find it.
if(FFTW_LIB)
  get_filename_component(FFTW_LIB_DIR "${FFTW_LIB}" DIRECTORY)
  link_directories(${FFTW_LIB_DIR})
endif()

include_directories(${FFTW_INCLUDES})
