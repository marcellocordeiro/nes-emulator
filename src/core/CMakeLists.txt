project(nes-emulator-core)

file(GLOB SOURCE_FILES
  "src/Mappers/*.cpp"
  "src/Types/*.cpp"
  "src/Utility/*.cpp"
  "src/*.cpp"
)

add_library(nes-emulator-core STATIC ${SOURCE_FILES})

target_include_directories(nes-emulator-core PUBLIC "include")

find_package(fmt REQUIRED)
target_link_libraries(nes-emulator-core PRIVATE fmt::fmt-header-only)

find_package(spdlog REQUIRED)
target_link_libraries(nes-emulator-core PRIVATE spdlog::spdlog_header_only)

set_target_properties(nes-emulator-core
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(nes-emulator-core PRIVATE project-lib)
target_link_libraries(nes-emulator-core PRIVATE project-warnings)
