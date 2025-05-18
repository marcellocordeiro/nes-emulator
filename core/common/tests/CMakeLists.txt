set(SOURCES
  src/sane_integer_tests.cpp
)

add_executable(common-tests ${SOURCES})

set_project_options(common-tests)
set_compiler_warnings(common-tests)

target_link_libraries(common-tests
  PRIVATE
    lib::common
    Catch2::Catch2WithMain
)

enable_testing()

catch_discover_tests(common-tests)
