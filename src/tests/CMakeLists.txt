set(NPROCESSES 4 CACHE STRING "Number of MPI processes used for testing (default = 4)")

#Also, nice to have a rule to run all the tests
add_custom_target(test-all)
add_custom_target(test-all-untested)

# Glob all test sources and then create targets for each of them
file(GLOB_RECURSE test_sources "${CMAKE_SOURCE_DIR}/src/tests/*.cpp")
foreach (fn ${test_sources})
    get_filename_component(f_name ${fn} NAME) #extract the filename from the full path
    if (NOT ${f_name} STREQUAL "tddmain.cpp")

        #remove cpp and create a string appending test_ at the begining. Allows to create a rule test_filename
        string(REPLACE ".cpp" "" target_name "test-${f_name}")

        add_cosmolattice(${target_name} ${CMAKE_SOURCE_DIR}/src/tests/tddmain.cpp "")
        target_sources(${target_name} PRIVATE ${fn})

        if (NOT MPI)
            add_custom_command(TARGET ${target_name} POST_BUILD #to run directly the test after building
                    COMMAND ./${target_name}
            )
        else ()
            add_custom_command(TARGET ${target_name} POST_BUILD #to run directly the test after building
                    COMMAND mpirun -n ${NPROCESSES} ${target_name}
            )
        endif ()

        # Add to the target to run all tests (test-all)
        add_dependencies(test-all ${target_name})
    endif ()

endforeach ()

add_custom_command(TARGET test-all-untested POST_BUILD #to run directly the test after building
        COMMAND ${CMAKE_SOURCE_DIR}/src/tests/make_all_untested.sh ${CMAKE_SOURCE_DIR}/src/tests ${CMAKE_BINARY_DIR}/src/tests/
)