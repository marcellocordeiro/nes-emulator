file(GLOB SOURCE_FILES
  "src/Mappers/*.cpp"
  "src/Types/*.cpp"
  "src/Utility/*.cpp"
  "src/*.cpp"
)

add_library(nes-emulator STATIC ${SOURCE_FILES})

target_include_directories(nes-emulator PUBLIC "include")

find_package(fmt REQUIRED)
target_link_libraries(nes-emulator PRIVATE fmt::fmt)

find_package(spdlog REQUIRED)
target_link_libraries(nes-emulator PRIVATE spdlog::spdlog_header_only)

target_link_libraries(nes-emulator PRIVATE nes-libs)

set_target_properties(nes-emulator
  PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)
