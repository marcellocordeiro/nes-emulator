set(HEADERS
  include/lib/bitfield.hpp
  include/lib/common.hpp
)

add_library(common INTERFACE)
add_library(lib::common ALIAS common)

target_sources(common
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES ${HEADERS}
)

set_target_properties(common
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(common INTERFACE global::warnings)

install(
  TARGETS common
  FILE_SET HEADERS
)
