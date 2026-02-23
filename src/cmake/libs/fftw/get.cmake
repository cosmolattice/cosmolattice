# Download FFTW

message(STATUS "---------- Getting FFTW ----------")

set(FFTW_URL "https://www.fftw.org/fftw-3.3.10.tar.gz")
set(FFTW_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/fftw")

if(NOT EXISTS "${FFTW_DIR}/fftw-3.3.10.tar.gz")
  message(STATUS "Downloading FFTW from ${FFTW_URL}...")
  file(DOWNLOAD "${FFTW_URL}" "${FFTW_DIR}/fftw-3.3.10.tar.gz")
else()
  message(
    STATUS
      "FFTW archive already exists at ${FFTW_DIR}/fftw-3.3.10.tar.gz, skipping download."
  )
endif()

# Extract FFTW
if(NOT EXISTS "${FFTW_DIR}/fftw-3.3.10")
  message(STATUS "Extracting FFTW...")
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xzf "${FFTW_DIR}/fftw-3.3.10.tar.gz"
    WORKING_DIRECTORY "${FFTW_DIR}")
else()
  message(
    STATUS
      "FFTW already extracted at ${FFTW_DIR}/fftw-3.10, skipping extraction.")
endif()

set(FFTW_CONFIGFLAGS
    " --enable-threads --disable-shared --enable-static --enable-sse2"
    CACHE
      STRING
      "Configuration flags for FFTW. Default is --enable-threads --disable-shared --enable-static --enable-sse2"
)

if(MPI)
  set(FFTW_CONFIGFLAGS_MPI
      "--enable-mpi"
      CACHE STRING "")
endif()

if(AVX)
  set(FFTW_CONFIGFLAGS_AVX
      "--enable-avx"
      CACHE STRING "")
endif()

# Build FFTW
message(
  STATUS
    "Configuring FFTW... (flags: ${FFTW_CONFIGFLAGS} ${FFTW_CONFIGFLAGS_MPI} ${FFTW_CONFIGFLAGS_AVX})"
)
execute_process(
  COMMAND
    bash -c
    "./configure --prefix=${FFTW_DIR} ${FFTW_CONFIGFLAGS} ${FFTW_CONFIGFLAGS_MPI} ${FFTW_CONFIGFLAGS_AVX}"
  WORKING_DIRECTORY "${FFTW_DIR}/fftw-3.3.10/"
  OUTPUT_FILE "${FFTW_DIR}/configure.log"
  ERROR_FILE "${FFTW_DIR}/configure.log"
  RESULT_VARIABLE fftw_configure_ret)
if(NOT fftw_configure_ret EQUAL 0)
  message(
    FATAL_ERROR
      "FFTW configure failed. See ${FFTW_DIR}/configure.log for details.")
endif()

message(STATUS "Building FFTW...")
execute_process(
  COMMAND make -j ${CMAKE_PROCESSOR_COUNT}
  WORKING_DIRECTORY "${FFTW_DIR}/fftw-3.3.10/"
  OUTPUT_FILE "${FFTW_DIR}/build.log"
  ERROR_FILE "${FFTW_DIR}/build.log"
  RESULT_VARIABLE fftw_build_ret)
if(NOT fftw_build_ret EQUAL 0)
  message(
    FATAL_ERROR "FFTW build failed. See ${FFTW_DIR}/build.log for details.")
endif()

message(STATUS "Installing FFTW...")
execute_process(
  COMMAND make install
  WORKING_DIRECTORY "${FFTW_DIR}/fftw-3.3.10/"
  OUTPUT_FILE "${FFTW_DIR}/install.log"
  ERROR_FILE "${FFTW_DIR}/install.log"
  RESULT_VARIABLE fftw_install_ret)
if(NOT fftw_install_ret EQUAL 0)
  message(
    FATAL_ERROR "FFTW install failed. See ${FFTW_DIR}/install.log for details.")
endif()

set(CMAKE_PREFIX_PATH "${FFTW_DIR};${CMAKE_PREFIX_PATH}")

message(STATUS "---------- Getting FFTW DONE ----------\n")
