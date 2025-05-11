set(SOURCES
  src/system_utils.cpp
)

set(HEADERS
  include/lib/bitfield.hpp
  include/lib/common.hpp
  include/lib/system_utils.hpp
)

add_library(common STATIC ${SOURCES})
add_library(lib::common ALIAS common)

target_sources(common
  INTERFACE
    FILE_SET HEADERS
    BASE_DIRS include
    FILES ${HEADERS}
)

target_include_directories(common
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

set_target_properties(common
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(common PRIVATE global::warnings)

install(
  TARGETS common
  LIBRARY
  FILE_SET HEADERS
)
