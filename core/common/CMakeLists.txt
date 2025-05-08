file(
  GLOB_RECURSE
  SOURCES
  CONFIGURE_DEPENDS
  "src/*.cpp"
  "src/*.hpp"
  "src/*.h"
  "include/*.hpp"
  "include/*.h"
)

add_library(common STATIC ${SOURCES})
add_library(nes::common ALIAS common)

target_include_directories(common PUBLIC "include")

set_target_properties(
  common
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(common PRIVATE nes::warnings)
