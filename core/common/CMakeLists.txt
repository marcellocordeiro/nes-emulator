set(HEADERS
  include/lib/bitfield.hpp
  include/lib/bitfield_helper.hpp
  include/lib/common.hpp
  include/lib/concepts/binary_ops.hpp
  include/lib/files.hpp
  include/lib/integer.hpp
)

add_library(common INTERFACE)
add_library(lib::common ALIAS common)

target_sources(common
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS include
    FILES ${HEADERS}
)

configure_file(
  include/lib/version.hpp.in
  include/lib/version.hpp
)

target_sources(common
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS ${CMAKE_CURRENT_BINARY_DIR}/include
    FILES ${CMAKE_CURRENT_BINARY_DIR}/include/lib/version.hpp
)

set_project_options(common)
set_compiler_warnings(common)

install(
  TARGETS common
  FILE_SET HEADERS
)

add_subdirectory(tests)
