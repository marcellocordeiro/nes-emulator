set(SOURCES
  src/app.cpp
  src/app.hpp
  src/main.cpp
  src/sdl/sdl.hpp
  src/sdl/sdl_context.hpp
  src/sdl/sdl_error.hpp
  src/sdl/sdl_include.hpp
  src/sdl/sdl_renderer.hpp
  src/sdl/sdl_texture.hpp
  src/sdl/sdl_window.hpp
  src/utils/scaling.hpp
  src/utils/vec2.hpp
)

add_executable(nes-emulator-sdl3 ${SOURCES})

set_target_properties(nes-emulator-sdl3
  PROPERTIES
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
)

target_link_libraries(nes-emulator-sdl3
  PRIVATE
    global::warnings
    lib::common
    nes::core
    fmt::fmt
    spdlog::spdlog
    SDL3::SDL3
)

function(copy_palette target)
  add_custom_command(
    TARGET ${target}
    POST_BUILD
    COMMAND
      ${CMAKE_COMMAND} -E copy_if_different
      "${CMAKE_SOURCE_DIR}/data/nes_mesen.pal"
      "$<TARGET_FILE_DIR:${target}>/palette.pal"
  )
endfunction()

copy_palette(nes-emulator-sdl3)

install(
  TARGETS nes-emulator-sdl3
)
