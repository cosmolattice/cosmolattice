if(DEVICE STREQUAL "KOKKOS")
  set(KOKKOS ON)

  include(src/cmake/device/kokkos.cmake)
  include(src/cmake/device/kokkos-fft.cmake)

  find_package(Kokkos REQUIRED HINTS ${CMAKE_CURRENT_BINARY_DIR}/Kokkos QUIET)
  find_package(KokkosFFT REQUIRED HINTS ${CMAKE_CURRENT_BINARY_DIR}/Kokkos-FFT
               QUIET)

  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DDEVICE_KOKKOS")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DKOKKOS_FFT")

  set(KOKKOS_FFT ON)
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
    target_link_libraries(${target} PUBLIC KokkosFFT::fft)
  endif()
endfunction()
