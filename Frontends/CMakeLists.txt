option(EMULATOR_QT5 "Build with Qt5 frontend" ON)
option(EMULATOR_SDL2 "Build with SDL2 frontend" ON)

# Qt5
if(EMULATOR_QT5)
  message("Using Qt5")
  add_subdirectory("qt5")
endif()

# SDL2
if(EMULATOR_SDL2)
  message("Using SDL2")
  add_subdirectory("sdl2")
endif()
