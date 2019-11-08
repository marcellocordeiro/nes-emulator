cmake_minimum_required(VERSION 3.15)

project(nes-emulator)

# Setting up the target

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

add_executable(nes-emulator
  "src/Main.cpp"
  "src/MainWindow.cpp"
  "src/MainWindow.h"
  "src/MainWindow.ui"
  "src/QtOpenGL.cpp"
  "src/QtOpenGL.h"
  "src/Sound_Queue.cpp"
  "src/Sound_Queue.h"
)

target_include_directories(nes-emulator PRIVATE	"src")

# Dependencies

add_subdirectory("nes")
target_link_libraries(nes-emulator PRIVATE nes)

find_package(Qt5 COMPONENTS Widgets OpenGL REQUIRED)
target_link_libraries(nes-emulator PRIVATE Qt5::Widgets Qt5::OpenGL)

find_package(OpenGL REQUIRED)
target_link_libraries(nes-emulator PRIVATE OpenGL::OpenGL)

find_package(SDL2 2.0 REQUIRED)
target_link_libraries(nes-emulator PRIVATE SDL2::SDL2)

find_package(fmt REQUIRED)
target_link_libraries(nes-emulator PRIVATE fmt::fmt)

find_package(spdlog REQUIRED)
target_link_libraries(nes-emulator PRIVATE spdlog::spdlog)

# Compiler settings

set_target_properties(nes-emulator
  PROPERTIES
    CXX_STANDARD 20
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    AUTOMOC ON
    AUTOUIC ON
    AUTORCC ON
)

if(MSVC)
  target_compile_options(nes-emulator PRIVATE /W4 /permissive- /diagnostics:caret)
elseif(UNIX)
  target_compile_options(nes-emulator PRIVATE -Wall -Wextra -pedantic -Wshadow -Wformat=2 -Wdisabled-optimization)
endif()

# Copy resource files

configure_file("data/nes_mesen.pal" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/palette.pal" COPYONLY)
