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

set_target_properties(common-sys
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(common-sys
  PRIVATE
    global::warnings
    lib::common
)

install(
  TARGETS common-sys
  #LIBRARY
  #FILE_SET HEADERS
)
