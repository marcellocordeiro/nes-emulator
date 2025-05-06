file(
  GLOB_RECURSE
  SOURCES
  CONFIGURE_DEPENDS
  "include/*.hpp"
  "include/*.h"
  "src/*.cpp"
  "src/*.hpp"
  "src/*.h"
)

add_library(nes-core STATIC ${SOURCES})
add_library(nes::core ALIAS nes-core)

target_include_directories(nes-core PUBLIC "include")

target_link_libraries(nes-core PRIVATE common)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(nes-core PRIVATE fmt::fmt)

find_package(spdlog CONFIG REQUIRED)
target_link_libraries(nes-core PRIVATE spdlog::spdlog)

set_target_properties(
  nes-core
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(nes-core PRIVATE project-warnings)
