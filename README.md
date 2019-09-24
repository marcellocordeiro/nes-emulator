# nes-emulator

NES emulator written in C++ using SDL2

## Features

- [x] CPU
- [x] PPU (NTSC only)
- [x] APU (blargg's Nes_Snd_Emu)
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
  - GCC 9.1
  - Clang 9.0.0
  - MSVC 19.23.28105.4 (VS 2019)

- Tools:
  - CMake 3.15

- Libraries:
  - SDL2 2.0.9
  - {fmt} 6.0.0

## Running

Run the `nes-emulator` executable generated in the `bin` folder passing the ROM path as an argument (e.g. `./nes-emulator rom.nes`).

## todo

- Improve the code and make it easier to select games. Maybe a nice UI with a settings editor?
- Tweak parameters and controls in a configuration file.
- Automated tests.
- Debugger/analyser.

## References

- [NESdev](http://wiki.nesdev.com/w/index.php/Nesdev_Wiki)
- [Mesen](https://www.mesen.ca/) for the excellent debugger and well documented implementation details.
- [@ulfalizer](https://github.com/ulfalizer)'s great PPU implementation and NTSC PPU timing diagram.
- [blargg's Nes_Snd_Emu](http://blargg.8bitalley.com/libs/audio.html#Nes_Snd_Emu) for the APU emulation module.
  - Code adjustments have been made to remove Boost's dependency, use fixed width integer types and fix most compiler warnings.
