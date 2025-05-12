set(SOURCES
  src/sane_integer.cpp
)

add_executable(common-tests ${SOURCES})

set_target_properties(common-tests
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(common-tests
  PRIVATE
    global::warnings
    lib::common
    Catch2::Catch2WithMain
)

enable_testing()

catch_discover_tests(common-tests)
