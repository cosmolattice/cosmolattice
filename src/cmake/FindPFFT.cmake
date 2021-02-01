find_library(PFFT_LIB
NAMES "pfft"
)

if(FlOATFFT)
find_library(PFFTF_LIB
NAMES "pfftf"
)
set(PFFT_LIBRARIES ${PFFT_LIB} ${PFFTF_LIB})
else()
set(PFFT_LIBRARIES ${PFFT_LIB})
endif()

find_path(
PFFT_INCLUDES
NAMES "pfft.h"
)
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(PFFT DEFAULT_MSG
                                  PFFT_INCLUDES PFFT_LIBRARIES)

mark_as_advanced(PFFT_INCLUDES PFFT_LIBRARIES)

