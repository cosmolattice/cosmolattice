# Glob all test sources and then create targets for each of them
file(GLOB_RECURSE bench_sources "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")
foreach(fn ${bench_sources})
  get_filename_component(f_name ${fn} NAME) # extract the filename from the full
                                            # path

  # remove cpp and create a string appending test_ at the begining. Allows to
  # create a rule test_filename
  string(REPLACE ".cpp" "" target_name "bench-${f_name}")

  add_cosmolattice(${target_name} ${fn} "")
endforeach()
