# ##############################################################################
# Documentation
# ##############################################################################

# check if Doxygen is installed
if(DOCUMENTATION STREQUAL "ON")
  find_package(Doxygen QUIET)
  if(DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/documentation/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message(STATUS "Documentation target configured")

    add_custom_target(
      documentation ALL
      COMMAND
        bash -c
        "${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT} &> ${CMAKE_CURRENT_BINARY_DIR}/doxygen.log"
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM)
  endif()
endif()
