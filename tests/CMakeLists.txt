# Enable CTest
enable_testing()

# Number of MPI processes for testing
set(NPROCESSES
    4
    CACHE STRING "Number of MPI processes used for testing (default = 4)")

# Get the path to the fetched TempLat source
FetchContent_GetProperties(TempLat SOURCE_DIR templat_SOURCE_DIR)
set(TDDMAIN "${templat_SOURCE_DIR}/tests/tddmain.cpp")

# Discover test sources from TWO locations:
#   1. TempLat tests — from the fetched templat source
#   2. CosmoInterface tests — from the in-tree src/tests/
file(GLOB_RECURSE templat_test_sources "${templat_SOURCE_DIR}/tests/TempLat/*.cpp")
file(GLOB_RECURSE cosmointerface_test_sources "${CMAKE_SOURCE_DIR}/src/tests/CosmoInterface/*.cpp")
set(test_sources ${templat_test_sources} ${cosmointerface_test_sources})

foreach(fn ${test_sources})
  get_filename_component(f_name ${fn} NAME)

  # Skip the test driver main file
  if(f_name STREQUAL "tddmain.cpp")
    continue()
  endif()

  # Create target name from filename (e.g., test-mytest from mytest.cpp)
  string(REPLACE ".cpp" "" test_name "${f_name}")
  set(target_name "test-${test_name}")

  # Build the test executable using cosmolattice infrastructure
  add_cosmolattice(${target_name} ${TDDMAIN} "")
  target_sources(${target_name} PRIVATE ${fn})
  target_compile_definitions(${target_name} PUBLIC CHECKBOUNDS)

  # Labels: derive from folder path relative to the tests/ directory in each source
  # For TempLat tests:  .../templat/tests/TempLat/fft/foo.cpp → TempLat;fft
  # For CosmoInterface: .../src/tests/CosmoInterface/foo.cpp   → CosmoInterface
  if(fn MATCHES ".*/tests/(.+)/[^/]+\\.cpp$")
    set(relative_dir "${CMAKE_MATCH_1}")
    string(REPLACE "/" ";" folder_labels "${relative_dir}")
  else()
    set(folder_labels "")
  endif()

  # Register test with CTest
  if(MPI)
    add_test(
      NAME ${target_name}
      COMMAND bash -c "make ${target_name} &&
              ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} ${NPROCESSES} ${MPIEXEC_PREFLAGS} $<TARGET_FILE:${target_name}> ${MPIEXEC_POSTFLAGS}"
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  else()
    add_test(
      NAME ${target_name}
      COMMAND bash -c "make ${target_name} && $<TARGET_FILE:${target_name}>"
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  endif()

  # Add labels for filtering tests by folder
  # This allows running tests like: ctest -L TempLat or ctest -L CosmoInterface
  set_tests_properties(${target_name} PROPERTIES LABELS "${folder_labels}")
endforeach()
