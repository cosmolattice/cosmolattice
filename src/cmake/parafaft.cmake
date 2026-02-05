message(STATUS "Fetching ParaFaFT...")

# enable FetchContent
include(FetchContent)
set(FETCHCONTENT_QUIET ON)
set(PARAFAFT_CUDA ${CUDA} CACHE BOOL "")
set(PARAFAFT_HIP ${HIP} CACHE BOOL "")
# declare ParaFaFT
FetchContent_Declare(ParaFaFT                              # name of the content
  GIT_REPOSITORY https://github.com/aflorio2/parafaft.git  # the repository
  GIT_TAG        main                                      # the tag
  )
# make available
FetchContent_MakeAvailable(ParaFaFT)

message(STATUS "ParaFaFT available with CUDA=${PARAFAFT_CUDA} HIP=${PARAFAFT_HIP}")