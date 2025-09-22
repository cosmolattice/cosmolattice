# ##############################################################################
# Build Kokkos-FFT! This will happen very manually - this way we have the
# greatest control over what is happening.
# ##############################################################################

set(KOKKOS_FFT_VERSION v0.4.0)

message(STATUS "Downloading Kokkos-FFT ${KOKKOS_FFT_VERSION}")
execute_process(
  COMMAND
    bash -c
    "mkdir -p _dep && git clone https://github.com/kokkos/kokkos-fft.git --recursive --branch ${KOKKOS_FFT_VERSION} _dep/kokkos-fft-repo  2>&1 > ${CMAKE_CURRENT_BINARY_DIR}/kokkos-fft.log"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  OUTPUT_QUIET)

message(STATUS "Configure Kokkos-FFT...")
execute_process(
  COMMAND bash -c "mkdir -p _dep/kokkos-fft-bin"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
  OUTPUT_QUIET)
execute_process(
  COMMAND
    bash -c
    "cmake \
        -DCMAKE_CXX_FLAGS=-fPIC \
        -DCMAKE_PREFIX_PATH=${CMAKE_CURRENT_BINARY_DIR}/Kokkos \
        -DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER} \
        -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/Kokkos-FFT \
        -DCMAKE_BUILD_TYPE=Release \
        -DCMAKE_CXX_STANDARD=${CMAKE_CXX_STANDARD} \
        ../kokkos-fft-repo &>> ${CMAKE_CURRENT_BINARY_DIR}/kokkos-fft_config.log"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/_dep/kokkos-fft-bin)

message(STATUS "Building Kokkos-FFT...")
execute_process(
  COMMAND bash -c "make -j &>> ${CMAKE_CURRENT_BINARY_DIR}/kokkos-fft_build.log"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/_dep/kokkos-fft-bin)

message(STATUS "Installing Kokkos-FFT...")
execute_process(
  COMMAND bash -c
          "make install &>> ${CMAKE_CURRENT_BINARY_DIR}/kokkos-fft_install.log"
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/_dep/kokkos-fft-bin)

# Kokkos by default does not support extensions, so we force them off
set(CMAKE_CXX_EXTENSIONS OFF)
# Make the package available
