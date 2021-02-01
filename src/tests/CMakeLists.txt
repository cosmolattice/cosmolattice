#with oldish (<3.12) version of CMake, a bit painful to create a .o and link it
#Need to make a custom_command to generate the .o and then add it in the executable
#To do that, we need to know the compile flags and the include folder
#Perhaps change in future releases?

set(flags ${CMAKE_CXX_FLAGS})

set(NPROCESSES 4 CACHE STRING "Set to ON to build without PFFT (default = ON)")


separate_arguments(flags) #makes it into a list, needed for custom command
#get_property(dirs GLOBAL PROPERTY INCLUDE_DIRECTORIES)
#get_property(dirs DIRECTORY ../cmake PROPERTY INCLUDE_DIRECTORIES)
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)

#Also, nice to have a rule to run all the tests
add_custom_target(test-all)
add_custom_target(test-all-untested)

# Basically a "res=`ls tests/*.cpp`"
file(GLOB_RECURSE res "${CMAKE_SOURCE_DIR}/src/tests/*.cpp")

set(myincludes "")
if(HIGH_FIVE)
    find_package(HDF5 REQUIRED)
    set(myincludes ${myincludes} -I${CMAKE_SOURCE_DIR}/${MYHIGHFIVE_PATH}/include/ -I${HDF5_INCLUDE_DIRS})
    #get_target_property(letsee HighFive INTERFACE_INCLUDE_DIRECTORIES)
endif()

foreach(tmp ${dirs})
    # 	  message(${letsee})
    set(myincludes ${myincludes} -I${tmp} )
endforeach()

#and a loop over "res"
foreach(fn ${res})


    get_filename_component(f_name ${fn} NAME) #extract the filename from the full path
    if(NOT ${f_name} STREQUAL "tddmain.cpp")

      string(REPLACE ".cpp" "" target_name "test-${f_name}") #remove cpp and create a string appending test_ at the begining. Allows to create a rule test_filename
      string(REPLACE ".cpp" "" makeo_name "makeo_${f_name}") # Intermediary string to name the *.o generation rule
      string(REPLACE ".cpp" "" cmakeo_name "cmakeo_${f_name}") # and to call it

      add_custom_command(OUTPUT ${makeo_name} #Ugly custom command to generate the .o
        COMMAND ${CMAKE_CXX_COMPILER} ${flags} -std=gnu++${CMAKE_CXX_STANDARD} -c -o ${f_name}.o ${fn} ${myincludes}  #in the new versions of CMake (>3.12) it looks like it could be replaced by a add_library(...OBJECT...) as it can now be linked
        COMMAND_EXPAND_LISTS
        BYPRODUCTS ${f_name}.o
      )
      add_custom_target(${cmakeo_name} ALL DEPENDS ${makeo_name}) #call the command.

      add_cosmolattice(${target_name} ${CMAKE_SOURCE_DIR}/src/tests/tddmain.cpp ${f_name}.o )
      add_dependencies(${target_name} ${cmakeo_name}) #to trigger the generation of .o

      #-l$<TARGET_PROPERTY:HighFive,INTERFACE_LINK_LIBRARIES>

      if(NOT MPI)
        add_custom_command(TARGET ${target_name} POST_BUILD #to run directly the test after building
          COMMAND ./${target_name}
        )
      else()
        add_custom_command(TARGET ${target_name} POST_BUILD #to run directly the test after building
          COMMAND mpirun -n ${NPROCESSES} ${target_name}
        )
      endif()


     # add_custom_command(TARGET ${target_name} POST_BUILD #to clean after the test
     # COMMAND rm *.o *.d
     # )
      add_dependencies(test-all ${target_name}) #"make testall" rule
    endif()

endforeach()

add_custom_command(TARGET test-all-untested POST_BUILD #to run directly the test after building
	COMMAND ${CMAKE_SOURCE_DIR}/src/tests/make_all_untested.sh ${CMAKE_SOURCE_DIR}/src/tests ${CMAKE_BINARY_DIR}/src/tests/
)
