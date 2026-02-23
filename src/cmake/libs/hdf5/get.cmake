# Download HDF5

message(STATUS "---------- Getting HDF5 ----------")

set(HDF5_URL
    "https://www.hdfgroup.org/package/hdf5-1-12-0-tar-gz/?wpdmdl=14582&refresh=60058b7b074801610976123"
)
set(HDF5_DIR "${CMAKE_CURRENT_BINARY_DIR}/_deps/hdf5")

if(NOT EXISTS "${HDF5_DIR}/hdf5-1.12.0.tar.gz")
  message(STATUS "Downloading HDF5 from ${HDF5_URL}...")
  file(DOWNLOAD "${HDF5_URL}" "${HDF5_DIR}/hdf5-1.12.0.tar.gz")
else()
  message(
    STATUS
      "HDF5 archive already exists at ${HDF5_DIR}/hdf5-1.12.0.tar.gz, skipping download."
  )
endif()

# Extract HDF5
if(NOT EXISTS "${HDF5_DIR}/hdf5-1.12.0")
  message(STATUS "Extracting HDF5...")
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xzf "${HDF5_DIR}/hdf5-1.12.0.tar.gz"
    WORKING_DIRECTORY "${HDF5_DIR}")
else()
  message(
    STATUS
      "HDF5 already extracted at ${HDF5_DIR}/hdf5-1.12.0, skipping extraction.")
endif()

if(MPI)
  set(HDF5_CONFIGFLAGS_MPI
      "--enable-parallel"
      CACHE STRING "")
endif()

# Build HDF5
message(STATUS "Configuring HDF5... (flags: ${HDF5_CONFIGFLAGS_MPI})")
execute_process(
  COMMAND bash -c "./configure --prefix=${HDF5_DIR} ${HDF5_CONFIGFLAGS_MPI}"
  WORKING_DIRECTORY "${HDF5_DIR}/hdf5-1.12.0/"
  OUTPUT_FILE "${HDF5_DIR}/configure.log"
  ERROR_FILE "${HDF5_DIR}/configure.log"
  RESULT_VARIABLE hdf5_configure_ret)
if(NOT hdf5_configure_ret EQUAL 0)
  message(
    FATAL_ERROR
      "HDF5 configure failed. See ${HDF5_DIR}/configure.log for details.")
endif()

message(STATUS "Building HDF5...")
execute_process(
  COMMAND make -j ${CMAKE_PROCESSOR_COUNT}
  WORKING_DIRECTORY "${HDF5_DIR}/hdf5-1.12.0/"
  OUTPUT_FILE "${HDF5_DIR}/build.log"
  ERROR_FILE "${HDF5_DIR}/build.log"
  RESULT_VARIABLE hdf5_build_ret)
if(NOT hdf5_build_ret EQUAL 0)
  message(
    FATAL_ERROR "HDF5 build failed. See ${HDF5_DIR}/build.log for details.")
endif()

message(STATUS "Installing HDF5...")
execute_process(
  COMMAND make install
  WORKING_DIRECTORY "${HDF5_DIR}/hdf5-1.12.0/"
  OUTPUT_FILE "${HDF5_DIR}/install.log"
  ERROR_FILE "${HDF5_DIR}/install.log"
  RESULT_VARIABLE hdf5_install_ret)
if(NOT hdf5_install_ret EQUAL 0)
  message(
    FATAL_ERROR "HDF5 install failed. See ${HDF5_DIR}/install.log for details.")
endif()

set(CMAKE_PREFIX_PATH "${HDF5_DIR};${CMAKE_PREFIX_PATH}")

set(HDF5_INCLUDE_DIRS
    "${HDF5_DIR}/include"
    CACHE INTERNAL "")
set(HDF5_LIBRARIES
    "${HDF5_DIR}/lib/libhdf5.a"
    CACHE INTERNAL "")
set(HDF5_C_INCLUDE_DIR
    "${HDF5_DIR}/include"
    CACHE INTERNAL "")

message(STATUS "---------- Getting HDF5 DONE ----------\n")
