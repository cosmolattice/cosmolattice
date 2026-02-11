# ##############################################################################
# Find out which backends are available
# ##############################################################################
include(CheckLanguage)

option(CUDA "Enable CUDA support" OFF)
option(HIP "Enable HIP support" OFF)
option(OpenMP "Enable OpenMP support" OFF)
option(Threads "Enable Threads support" OFF)
option(Serial "Enable Serial support" OFF)

message("")

if(NOT CUDA
   AND NOT HIP
   AND NOT OpenMP
   AND NOT Threads
   AND NOT Serial)
  message(STATUS "---------- No device specified, trying to auto-detect ----------")
  set(CUDA ON)
  set(HIP ON)
  set(OpenMP ON)
  set(Threads ON)
  set(Serial ON)
endif()

if(CUDA)
  # Let's see if we have a CUDA compiler
  check_language(CUDA)
  if(CMAKE_CUDA_COMPILER)
    set(CUDA ON)
    set(HIP OFF)
    set(OpenMP OFF)
    set(Threads OFF)
    set(Serial OFF)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEVICE_CUDA")
  else()
    set(CUDA OFF)
  endif()
endif()

if(HIP)
  # Let's see if we have a HIP compiler
  check_language(HIP)
  if(CMAKE_HIP_COMPILER)
    set(HIP ON)
    set(CUDA OFF)
    set(OpenMP OFF)
    set(Threads OFF)
    set(Serial OFF)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEVICE_HIP")
  else()
    set(HIP OFF)
  endif()
endif()

if(OpenMP)
  check_language(OpenMP)
  find_package(OpenMP QUIET)
  if(OpenMP_CXX_FOUND)
    set(OpenMP ON)
    set(Threads OFF)
    set(Serial OFF)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEVICE_CPU")
  else()
    set(OpenMP OFF)
    set(Threads ON)
    set(Serial OFF)
  endif()
endif()

if(Serial)
  set(Serial ON)
  set(CUDA OFF)
  set(HIP OFF)
  set(OpenMP OFF)
  set(Threads OFF)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEVICE_CPU")
else()
  set(Serial OFF)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEVICE_CPU")
endif()

message(
  STATUS
    "Device configuration: \n    CUDA: ${CUDA} \n    HIP: ${HIP} \n    OpenMP: ${OpenMP} \n    Threads: ${Threads} \n    Serial: ${Serial}"
)

# ##############################################################################
# Choose a device provider
# ##############################################################################

if(DEVICE_PROVIDER STREQUAL "Kokkos")
  set(KOKKOS ON)

  message(STATUS "---------- Using Kokkos as device provider ----------\n")

  include(src/cmake/device/kokkos.cmake)
  include(src/cmake/device/kokkos-fft.cmake)

  set(CMAKE_REQUIRED_QUIET ON)

  set(CMAKE_MESSAGE_LOG_LEVEL WARNING)
  set(CMAKE_MESSAGE_LOG_LEVEL STATUS)

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEVICE_KOKKOS")
else()
  message(FATAL_ERROR "Unknown DEVICE_PROVIDER option: ${DEVICE_PROVIDER}.
      Supported options: Kokkos")
endif()

function(target_link_device target)
  if(NOT TARGET ${target})
    message(FATAL_ERROR "Target ${target} does not exist.")
  endif()
  if(KOKKOS)
    target_link_libraries(${target} PUBLIC Kokkos::kokkos)
    if(KOKKOSFFT)
      target_link_libraries(${target} PUBLIC KokkosFFT::fft)
    endif()
  endif()
endfunction()
