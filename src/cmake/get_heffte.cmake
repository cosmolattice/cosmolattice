# ##############################################################################
# Build heffte! This will happen very manually - this way we have the greatest
# control over what is happening.
# ##############################################################################

if((CUDA OR HIP) AND (MPI))
  set(HEFFTE_VERSION v2.4.1)

  message(STATUS "Downloading HEFFTE ${HEFFTE_VERSION}")
  execute_process(
    COMMAND
      bash -c
      "mkdir -p _dep && git clone https://github.com/icl-utk-edu/heffte.git --depth 1 --branch ${HEFFTE_VERSION} _dep/heffte-repo  2>&1 > ${CMAKE_CURRENT_BINARY_DIR}/heffte.log"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    OUTPUT_QUIET)

  message(STATUS "Configure HEFFTE...")
  execute_process(
    COMMAND bash -c "mkdir -p _dep/heffte-bin"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    OUTPUT_QUIET)
  execute_process(
    COMMAND
      bash -c "cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DBUILD_SHARED_LIBS=OFF     \
    -DCMAKE_INSTALL_PREFIX=${CMAKE_CURRENT_BINARY_DIR}/heffte \
    -DHeffte_ENABLE_AVX=ON \
    -DHeffte_ENABLE_AVX512=512 \
    -DHeffte_ENABLE_FFTW=ON \
    -DHeffte_ENABLE_CUDA=${CUDA} \
    -DHeffte_ENABLE_ROCM=${HIP} \
    -DHeffte_ENABLE_TESTING=OFF \
    -DCMAKE_CUDA_FLAGS=\"--use_fast_math -arch=native\" \
    -DCMAKE_CXX_STANDARD:STRING=${CMAKE_CXX_STANDARD} \
      ../heffte-repo &>> ${CMAKE_CURRENT_BINARY_DIR}/heffte.log"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/_dep/heffte-bin)

  message(STATUS "Building heffte...")
  execute_process(
    COMMAND bash -c "make -j &>> ${CMAKE_CURRENT_BINARY_DIR}/heffte.log"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/_dep/heffte-bin)

  message(STATUS "Installing heffte...")
  execute_process(
    COMMAND bash -c "make install &>> ${CMAKE_CURRENT_BINARY_DIR}/heffte.log"
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/_dep/heffte-bin)
else()
  set(HEFFTE OFF)
endif()
