option(EMULATOR_QT5 "Build with Qt5 frontend" OFF)
option(EMULATOR_SDL2 "Build with SDL2 frontend" OFF)
option(EMULATOR_GLFW "Build with GLFW frontend" ON)

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

# GLFW
if(EMULATOR_GLFW)
  message("Using GLFW")
  add_subdirectory("glfw")
endif()