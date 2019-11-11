# nes-emulator

NES emulator written in C++ using Qt5 and OpenGL

## Features

- [x] CPU
- [x] PPU (NTSC only)
- [ ] APU
- [x] Input
- [x] Cartridge
- [x] Saving (on exit)
- [x] Mapper 0 (NROM)
- [x] Mapper 1 (MMC1)
- [x] Mapper 2 (UxROM)
- [x] Mapper 4 (MMC3)
- [x] Mapper 7 (AxROM)
- [x] Snapshots
- [x] Colour emphasis
- [x] Custom palettes (.pal)
  - [x] 64 colours
  - [ ] 64 * 8 colours
  - [x] Auto generation of colour emphasis palettes

## Building

Generate the solution files using CMake and build it. It has been tested on Windows and Arch Linux with the following compilers, tools and libraries:

- Compilers:
  - GCC 9.2
  - Clang 9.0.0
  - MSVC 19.23.28105.4 (VS 2019)

- Tools:
  - CMake 3.15

- Libraries:
  - {fmt} 6.0.0
  - spdlog 1.4.2
  - Qt5 5.13

## Running

Run the `nes-emulator` executable generated in the `bin` folder passing the ROM path as an argument (e.g. `./nes-emulator rom.nes`).

## todo

- APU
- Improve the code and make it easier to select games. Maybe a nice UI with a settings editor?
- Tweak parameters and controls in a configuration file.
- Automated tests.
- Debugger/analyser.

## References

- [NESdev](http://wiki.nesdev.com/w/index.php/Nesdev_Wiki)
- [Mesen](https://www.mesen.ca/) for the excellent debugger and well documented implementation details.
- [@ulfalizer](https://github.com/ulfalizer)'s great PPU implementation and NTSC PPU timing diagram.
