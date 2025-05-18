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

set_project_options(nes-emulator-sdl3)
set_compiler_warnings(nes-emulator-sdl3)

target_link_libraries(nes-emulator-sdl3
  PRIVATE
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
