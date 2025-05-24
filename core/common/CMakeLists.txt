set(HEADERS
  include/lib/bitfield.hpp
  include/lib/bitfield_helper.hpp
  include/lib/common.hpp
  include/lib/concepts/binary_ops.hpp
  include/lib/files.hpp
  include/lib/integer.hpp
)

configure_file(
  include/lib/version.hpp.in
  include/lib/version.hpp
)

add_library(common INTERFACE)
add_library(lib::common ALIAS common)

target_sources(common
  PUBLIC
    FILE_SET HEADERS
    BASE_DIRS
      include
      ${CMAKE_CURRENT_BINARY_DIR}/include
    FILES
      ${HEADERS}
      ${CMAKE_CURRENT_BINARY_DIR}/include/lib/version.hpp
)

set_target_options(common)
set_compiler_warnings(common)

install(
  TARGETS common
  FILE_SET HEADERS
)

if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
