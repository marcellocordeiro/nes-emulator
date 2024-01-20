project(nes-core)

file(GLOB SOURCE_FILES
  "src/mappers/*.cpp"
  "src/types/*.cpp"
  "src/utility/*.cpp"
  "src/*.cpp"
)

add_library(nes-core STATIC ${SOURCE_FILES})

target_include_directories(nes-core PUBLIC "include")

target_link_libraries(nes-core PRIVATE common)

find_package(fmt REQUIRED)
target_link_libraries(nes-core PRIVATE fmt::fmt-header-only)

find_package(spdlog REQUIRED)
target_link_libraries(nes-core PRIVATE spdlog::spdlog_header_only)

set_target_properties(nes-core
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(nes-core PRIVATE project-warnings)
