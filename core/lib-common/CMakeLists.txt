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

add_library(lib-common STATIC ${SOURCES})

target_include_directories(lib-common PUBLIC "include")

set_target_properties(
  lib-common
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(lib-common PRIVATE project-warnings)
