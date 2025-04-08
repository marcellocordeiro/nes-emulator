# nes-emulator

Experimental NES emulator written in C++20 using SDL2.

**_Disclaimer_**: this emulator is an experimental project for educational purposes. The development and use of emulators is legal, as long as no copyrighted content is illegally obtained. This means you are responsible for dumping your and games. However, there exists free and open-source content in case you'd like to test this project and/or contribute :blush:

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
  - [ ] 64 \* 8 colours
  - [x] Auto generation of colour emphasis palettes

## Screenshots

![Alt text](/doc/screenshots/Super_Mario_Bros_3.png?raw=true "Super Mario Bros 3")

![Alt text](/doc/screenshots/The_Legend_of_Zelda.png?raw=true "The Legend of Zelda")

![Alt text](/doc/screenshots/Final_Fantasy_III.png?raw=true "Final Fantasy III")

## Dependencies

This project requires an updated compiler with C++20 support to be built. It has been tested with GCC 11.1.0 and MSVC 19.28.29915.0 (VS 16.9).

### Arch Linux

```bash
pacman -S fmt spdlog
pacman -S sdl2
```

### Windows

```cmd
vcpkg install fmt:x64-windows spdlog:x64-windows
vcpkg install sdl2:x64-windows
```

## Building

Generate the solution files using CMake and build it.

## Running

Run the `nes-emulator-sdl2{,.exe}` executable generated in the `bin` folder passing the ROM path as an argument (e.g. `./nes-emulator-sdl2{,.exe} rom.nes`).

## todo

- APU
- Improve the code and make it easier to select games. Maybe a nice UI with a settings editor?
- Tweak parameters and controls in a configuration file
- Automated tests
- Debugger/analyser

## References

- [NESdev](http://wiki.nesdev.com/w/index.php/Nesdev_Wiki)
- [Mesen](https://www.mesen.ca/) for the excellent debugger and well documented hardware behaviour
- [@ulfalizer](https://github.com/ulfalizer)'s great PPU implementation and NTSC PPU timing diagram
