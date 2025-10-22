set(NPROCESSES
    4
    CACHE STRING "Number of MPI processes used for testing (default = 4)")

# Also, nice to have a rule to run all the tests
add_custom_target(test-all)
add_custom_target(test-all-untested)

set(testFolderList "")

# Glob all test sources and then create targets for each of them
file(GLOB_RECURSE test_sources "${CMAKE_SOURCE_DIR}/src/tests/*.cpp")
foreach(fn ${test_sources})
  get_filename_component(f_name ${fn} NAME) # extract the filename from the full
                                            # path
  if(NOT ${f_name} STREQUAL "tddmain.cpp")

    # remove cpp and create a string appending test_ at the begining. Allows to
    # create a rule test_filename
    string(REPLACE ".cpp" "" target_name "test-${f_name}")
    string(REPLACE ".cpp" "-buildonly" build_name "test-${f_name}")

    add_cosmolattice(${build_name} ${CMAKE_SOURCE_DIR}/src/tests/tddmain.cpp "")
    target_sources(${build_name} PRIVATE ${fn})
    target_compile_definitions(${build_name} PUBLIC CHECKBOUNDS)

    add_custom_target(${target_name})
    add_dependencies(${target_name} ${build_name})
    if(NOT MPI)
      add_custom_command(
        TARGET ${target_name}
        POST_BUILD # to run directly the test after building
        COMMAND ./${build_name})
    else()
      add_custom_command(
        TARGET ${target_name}
        POST_BUILD # to run directly the test after building
        COMMAND mpirun -n ${NPROCESSES} ${build_name})
    endif()

    # Add to the target to run all tests (test-all)
    add_dependencies(test-all ${target_name})

    # Split fn into its folder components
    string(REPLACE "${CMAKE_SOURCE_DIR}/src/tests/" "" folder_components
                   "${fn}")
    string(REPLACE "/" ";" folder_components "${folder_components}")
    # Remove the last component (the filename)
    list(POP_BACK folder_components)
    # Rebuild the full folder path
    string(REPLACE ";" "/" full_folder "${folder_components}")

    # Iterate over the components to create custom targets for each folder
    set(folder "")
    foreach(folderpart ${folder_components})
      # The folder is the concatenation of all parts up to this one
      if(folder STREQUAL "")
        set(folder "${folderpart}")
      else()
        set(folder "${folder}-${folderpart}")
      endif()

      string(REPLACE ";" "-" folder_target_name "test-${folder}")
      if(NOT folder IN_LIST testFolderList)
        list(APPEND testFolderList ${folder})
        add_custom_target(${folder_target_name})

        add_custom_target(${folder_target_name}-untested)
        add_custom_command(
          TARGET ${folder_target_name}-untested
          POST_BUILD # to run directly the test after building
          COMMAND
            ${CMAKE_SOURCE_DIR}/src/tests/make_all_untested.sh
            ${CMAKE_SOURCE_DIR}/src/tests/${full_folder}
            ${CMAKE_BINARY_DIR}/src/tests/${full_folder}
            ${CMAKE_BINARY_DIR}/src/tests/)
      endif()

      add_dependencies(${folder_target_name} ${target_name})
    endforeach()
  endif()
endforeach()

add_custom_command(
  TARGET test-all-untested
  POST_BUILD # to run directly the test after building
  COMMAND
    ${CMAKE_SOURCE_DIR}/src/tests/make_all_untested.sh
    ${CMAKE_SOURCE_DIR}/src/tests ${CMAKE_BINARY_DIR}/src/tests/
    ${CMAKE_BINARY_DIR}/src/tests/)
