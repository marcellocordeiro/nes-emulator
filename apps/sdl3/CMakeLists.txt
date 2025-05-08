file(
  GLOB_RECURSE
  SOURCES
  CONFIGURE_DEPENDS
  "src/*.cpp"
  "src/*.hpp"
  "src/*.h"
)

add_executable(nes-emulator-sdl3)
target_sources(nes-emulator-sdl3 PRIVATE ${SOURCES})

set_target_properties(
  nes-emulator-sdl3
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(
  nes-emulator-sdl3
    PRIVATE
      nes::warnings
      nes::common
      nes::core
      fmt::fmt
      spdlog::spdlog
      SDL3::SDL3
)

function(copy_palette target)
  add_custom_command(
    TARGET ${target} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_SOURCE_DIR}/data/nes_mesen.pal"
      "$<TARGET_FILE_DIR:${target}>/palette.pal"
  )
endfunction()

copy_palette(nes-emulator-sdl3)
