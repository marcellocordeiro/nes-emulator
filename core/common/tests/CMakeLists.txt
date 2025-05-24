set(SOURCES
  src/integer_tests.cpp
)

add_executable(common-tests ${SOURCES})

set_target_options(common-tests)
set_compiler_warnings(common-tests)

target_link_libraries(common-tests
  PRIVATE
    lib::common
    Catch2::Catch2WithMain
)

catch_discover_tests(common-tests)
