# nes-emulator

Experimental NES emulator written in C++23 using SDL3.

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
- [ ] Snapshots (removed for now)
- [x] Colour emphasis
- [x] Custom palettes (.pal)
  - [x] 64 colours
  - [ ] 64 \* 8 colours
  - [x] Auto generation of color emphasis palettes

## Screenshots

![Alt text](/docs/screenshots/Super_Mario_Bros_3.png?raw=true "Super Mario Bros 3")

![Alt text](/docs/screenshots/The_Legend_of_Zelda.png?raw=true "The Legend of Zelda")

![Alt text](/docs/screenshots/Final_Fantasy_III.png?raw=true "Final Fantasy III")

## Supported tools

This project requires a recent compiler with C++23 support to be built.

- GCC 15.0.1
  - Tested on Fedora 42
- Clang 20.1.2
  - Tested on Fedora 42
- MSVC 19.43.34810 (Visual Studio 17.13.6)
  - Tested on Windows 11
- Apple Clang 17.0.0 (Xcode 16.3)
  - Tested on macOS 15.5 (Intel)

## Dependencies

### vcpkg

1. Install [vcpkg](https://vcpkg.io/)
2. Add `VCPKG_ROOT` to the environment variables, containing the path to vcpkg (e.g. `C:\vcpkg`)
3. vcpkg will install the dependencies during the configuration step (manifest mode)

SDL3 is excluded from the dependency list on Linux, so it needs to be manually installed instead:

### Arch Linux

```bash
sudo pacman -S sdl3
```

### Fedora

```bash
sudo dnf install SDL3-devel
```

## Building

Generate the solution files using CMake and build it.

## Running

Run the `nes-emulator-sdl3{,.exe}` executable generated in the `bin` folder passing the ROM path as an argument (e.g. `./nes-emulator-sdl3{,.exe} rom.nes`).

## todo

- APU
- Improve the code and make it easier to select games. Maybe a nice UI with a settings editor?
- Tweak parameters and controls in a configuration file
- Automated tests
- Debugger/analyser

## References

- [NESdev](http://wiki.nesdev.com/w/index.php/Nesdev_Wiki)
- [Mesen](https://www.mesen.ca/) for the excellent debugger and well documented hardware behavior
- [@ulfalizer](https://github.com/ulfalizer)'s great PPU implementation and NTSC PPU timing diagram
