# ##############################################################################
# Find out which backends are available
# ##############################################################################
include(CheckLanguage)

option(CUDA "Enable CUDA support" ON)
option(HIP "Enable HIP support" ON)
option(OpenMP "Enable OpenMP support" ON)

if(CUDA)
  # Let's see if we have a CUDA compiler
  check_language(CUDA)
  if(CMAKE_CUDA_COMPILER)
    set(CUDA ON)
    set(HIP OFF)
  else()
    set(CUDA OFF)
  endif()
endif()

if(HIP AND NOT CUDA)
  # Let's see if we have a HIP compiler
  check_language(HIP)
  if(CMAKE_HIP_COMPILER)
    set(HIP ON)
  else()
    set(HIP OFF)
  endif()
endif()

if(OpenMP
   AND NOT CUDA
   AND NOT HIP)
  check_language(OpenMP)
  find_package(OpenMP QUIET)
  if(OpenMP_CXX_FOUND)
    set(OPENMP ON)
    set(THREADS OFF)
  else()
    set(OPENMP OFF)
  endif()
else()
  set(OPENMP OFF)
endif()

if(NOT CUDA
   AND NOT HIP
   AND NOT OPENMP)
  set(THREADS ON)
else()
  set(THREADS OFF)
endif()

message(
  STATUS
    "Device configuration: \n    CUDA: ${CUDA} \n    HIP: ${HIP} \n    OpenMP: ${OPENMP} \n    Threads: ${THREADS}"
)

# ##############################################################################
# Choose a device provider
# ##############################################################################

if(DEVICE STREQUAL "KOKKOS")
  set(KOKKOS ON)

  message(STATUS "Using Kokkos as device provider.")

  include(src/cmake/device/kokkos.cmake)
  if(CUDA OR HIP)
    include(src/cmake/device/kokkos-fft.cmake)
  endif()

  set(CMAKE_REQUIRED_QUIET ON)

  set(CMAKE_MESSAGE_LOG_LEVEL WARNING)
  find_package(Kokkos REQUIRED HINTS ${CMAKE_CURRENT_BINARY_DIR}/Kokkos QUIET)
  if(CUDA OR HIP)
    find_package(KokkosFFT REQUIRED HINTS
                 ${CMAKE_CURRENT_BINARY_DIR}/Kokkos-FFT QUIET)
  endif()
  set(CMAKE_MESSAGE_LOG_LEVEL STATUS)

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEVICE_KOKKOS")
  if(CUDA OR HIP)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DKOKKOS_FFT")
    set(KOKKOS_FFT ON)
  endif()
else()
  message(FATAL_ERROR "Unknown DEVICE option: ${DEVICE}.
      Supported options: KOKKOS")
endif()

function(target_link_device target)
  if(NOT TARGET ${target})
    message(FATAL_ERROR "Target ${target} does not exist.")
  endif()
  if(KOKKOS)
    target_link_libraries(${target} PUBLIC Kokkos::kokkos)
    if(KOKKOS_FFT)
      target_link_libraries(${target} PUBLIC KokkosFFT::fft)
    endif()
  endif()
endfunction()
