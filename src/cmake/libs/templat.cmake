message(STATUS "Fetching TempLat...")

include(FetchContent)
set(FETCHCONTENT_QUIET ON)

FetchContent_Declare(TempLat
    GIT_REPOSITORY https://github.com/cosmolattice/templat.git
    GIT_TAG        main
)
FetchContent_MakeAvailable(TempLat)

message(STATUS "TempLat available")
