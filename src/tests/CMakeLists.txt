# Enable CTest
enable_testing()

# Number of MPI processes for testing
set(NPROCESSES
    4
    CACHE STRING "Number of MPI processes used for testing (default = 4)")

# Glob all test sources (excluding tddmain.cpp)
file(GLOB_RECURSE test_sources "${CMAKE_SOURCE_DIR}/src/tests/*/*.cpp")

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
  add_cosmolattice(${target_name} ${CMAKE_SOURCE_DIR}/src/tests/tddmain.cpp "")
  target_sources(${target_name} PRIVATE ${fn})
  target_compile_definitions(${target_name} PUBLIC CHECKBOUNDS)

  # Extract folder path relative to tests directory for test labeling
  string(REPLACE "${CMAKE_SOURCE_DIR}/src/tests/" "" relative_path "${fn}")
  get_filename_component(folder_path "${relative_path}" DIRECTORY)
  # Convert folder path to label format (e.g., TempLat/fft -> TempLat;fft)
  string(REPLACE "/" ";" folder_labels "${folder_path}")

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
  # This allows running tests like: ctest -L TempLat or ctest -L "fft"
  set_tests_properties(${target_name} PROPERTIES LABELS "${folder_labels}")
endforeach()
