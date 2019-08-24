#pragma once

namespace nes {
class emulator;
class io;
class debugger;

class cpu;
class apu;
class ppu;
class cartridge;
class mapper;
class controller;
}  // namespace nes

namespace nes::util {
class snapshotable;
}  // namespace nes::util

// SDL2
struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
struct SDL_KeyboardEvent;

// blargg's Nes_Snd_Emu
class Nes_Apu;
class Blip_Buffer;
class Sound_Queue;
