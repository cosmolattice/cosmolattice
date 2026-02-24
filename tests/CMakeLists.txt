# ##############################################################################
# Number of MPI processes
# ##############################################################################

set(NPROCESSES
    4
    CACHE STRING "Number of MPI processes used for testing")

# ##############################################################################
# Helper function
# ##############################################################################

function(add_cosmointerface_test execName mainFile)
  add_executable(${execName} ${mainFile})
  # and tell the compiler which model to choose
  target_compile_definitions(${execName} PRIVATE MODELTYPE=${MODEL})
  target_compile_definitions(${execName} PRIVATE MODELINCLUDE=models/${MODEL}.h)
  target_compile_definitions(${execName} PRIVATE MODELLABEL=${MODEL_LABEL})
  # link against TempLat
  target_link_libraries(${execName} PRIVATE TempLat::TempLat)
  # and include CosmoInterface
  target_include_directories(
    ${execName} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include/
                        ${CMAKE_CURRENT_SOURCE_DIR}/../)
endfunction()

# ##############################################################################
# Discover and register tests
# ##############################################################################

file(GLOB_RECURSE test_sources
     "${CMAKE_CURRENT_SOURCE_DIR}/CosmoInterface/*/*.cpp"
     "${CMAKE_CURRENT_SOURCE_DIR}/CosmoInterface/*.cpp")

foreach(fn ${test_sources})
  get_filename_component(f_name ${fn} NAME)
  string(REPLACE ".cpp" "" test_name "${f_name}")
  set(target_name "test-${test_name}")

  add_cosmointerface_test(${target_name}
                          ${CMAKE_CURRENT_SOURCE_DIR}/tddmain.cpp)
  target_sources(${target_name} PRIVATE ${fn})
  target_compile_definitions(${target_name} PUBLIC CHECKBOUNDS)

  # Labels for ctest filtering
  string(REPLACE "${CMAKE_CURRENT_SOURCE_DIR}/" "" relative_path "${fn}")
  get_filename_component(folder_path "${relative_path}" DIRECTORY)
  string(REPLACE "/" ";" folder_labels "${folder_path}")

  if(MPI)
    add_test(
      NAME ${target_name}
      COMMAND
        bash -c
        "make ${target_name} &&
                ${MPIEXEC_EXECUTABLE} ${MPIEXEC_NUMPROC_FLAG} ${NPROCESSES} ${MPIEXEC_PREFLAGS} $<TARGET_FILE:${target_name}> ${MPIEXEC_POSTFLAGS}"
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  else()
    add_test(
      NAME ${target_name}
      COMMAND bash -c "make ${target_name} && $<TARGET_FILE:${target_name}>"
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
  endif()

  set_tests_properties(${target_name} PROPERTIES LABELS "${folder_labels}")
endforeach()

file(GLOB_RECURSE TL_headers
     "${CMAKE_CURRENT_SOURCE_DIR}/../include/CosmoInterface/*/*.h"
     "${CMAKE_CURRENT_SOURCE_DIR}/../include/CosmoInterface/*.h")

# A list of files that really do not need tests, e.g. because they just bundle
# includes.
set(Allow_missing_tests CosmoInterface/cosmointerface.h)

foreach(header ${TL_headers})
  # strip the common prefix
  cmake_path(RELATIVE_PATH header BASE_DIRECTORY
             ${CMAKE_CURRENT_SOURCE_DIR}/../include/ OUTPUT_VARIABLE rel_header)
  # replace .h with .cpp
  string(REPLACE ".h" ".cpp" test_file "${rel_header}")

  # skip if the file is in the allow list
  if(rel_header IN_LIST Allow_missing_tests)
    continue()
  endif()

  # Otherwise, complain if the corresponding .cpp file does not exist in the
  # tests directory
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${test_file}")
    continue() # skip if the corresponding .cpp file already exists
  else()
    message(
      "${BoldMagenta} Header ${rel_header} does not have a corresponding test source file."
    )
  endif()
endforeach()
