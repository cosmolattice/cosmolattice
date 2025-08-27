# Function to compile any main which use CosmoLattice.
function(add_cosmolattice execName path ofile)
  add_executable(${execName} ${path} ${ofile})
  if(MPI)
    target_link_libraries(${execName} PUBLIC ${MPI_CXX_LIBRARIES})
    if(PFFT)
      target_link_libraries(${execName} PUBLIC ${PFFT_LIBRARIES})
    endif()
    # else() target_link_libraries(${execName} PUBLIC Threads::Threads)
  endif()
  target_link_libraries(${execName} PUBLIC Threads::Threads)
  target_link_libraries(${execName} PUBLIC ${FFTW_LIBRARIES})

  if(HDF5)
    target_link_libraries(${execName} PUBLIC ${HDF5_LIBRARIES})
  endif()

  if(KOKKOS)
    target_link_libraries(${execName} PUBLIC Kokkos::kokkos)
  endif()

  if(HEFFTE)
    target_link_libraries(${execName} PUBLIC Heffte::Heffte)
  endif()
endfunction()

# Function to compile any main which use CosmoLattice.
function(add_cosmolattice_executable execName path outputname)
  add_cosmolattice(${execName} ${path}${execName}.cpp "")
  set_target_properties(${execName} PROPERTIES OUTPUT_NAME "${outputname}")
  target_compile_options(${execName} PUBLIC -ftemplate-backtrace-limit=0)
endfunction()
