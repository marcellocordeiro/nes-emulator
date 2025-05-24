set(SOURCES
  src/system_utils.cpp
)

set(HEADERS
  include/lib/system_utils.hpp
)

add_library(common-sys STATIC ${SOURCES})
add_library(lib::common-sys ALIAS common-sys)

target_sources(common-sys
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES ${HEADERS}
)

set_target_options(common-sys)
set_compiler_warnings(common-sys)

target_link_libraries(common-sys
  PRIVATE
    lib::common
)

install(
  TARGETS common-sys
  FILE_SET HEADERS
)
