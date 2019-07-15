#include "emulator.h"

#include "cartridge.h"
#include "cpu.h"

namespace nes {
emulator::emulator()
{
  SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
  // Bilinear filter
  // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

  window.reset(SDL_CreateWindow(
      "nes-emulator",
      SDL_WINDOWPOS_CENTERED,
      SDL_WINDOWPOS_CENTERED,
      width * 2,
      height * 2,
      0));

  renderer.reset(SDL_CreateRenderer(
      window.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC));

  SDL_RenderSetLogicalSize(renderer.get(), width, height);

  texture.reset(SDL_CreateTexture(
      renderer.get(),
      SDL_PIXELFORMAT_ARGB8888,
      SDL_TEXTUREACCESS_STREAMING,
      width,
      height));

  keys = SDL_GetKeyboardState(nullptr);
}

emulator::~emulator()
{
  this->close();
}

void emulator::close()
{
  // leak?
  // valgrind --log-file='valgrind%p.log' --track-origins=yes --leak-check=full
  // ./nes-emulator

  texture  = nullptr;
  renderer = nullptr;
  window   = nullptr;
  SDL_Quit();
}

void emulator::set_bus(nes::bus& ref)
{
  this->bus = &ref;
}

uint8_t emulator::get_controller(size_t n) const
{
  uint8_t state = 0;

  state |= (keys[KEY_A[n]]) << 0u;
  state |= (keys[KEY_B[n]]) << 1u;
  state |= (keys[KEY_SELECT[n]]) << 2u;
  state |= (keys[KEY_START[n]]) << 3u;
  state |= (keys[KEY_UP[n]]) << 4u;
  state |= (keys[KEY_DOWN[n]]) << 5u;
  state |= (keys[KEY_LEFT[n]]) << 6u;
  state |= (keys[KEY_RIGHT[n]]) << 7u;

  return state;
}

void emulator::update_frame(const uint32_t* frame)
{
  SDL_UpdateTexture(texture.get(), nullptr, frame, width * sizeof(uint32_t));
}

void emulator::draw()
{
  SDL_RenderClear(renderer.get());
  SDL_RenderCopy(renderer.get(), texture.get(), nullptr, nullptr);
  SDL_RenderPresent(renderer.get());
}

void emulator::run()
{
  uint32_t frame_rate = 60;
  uint32_t frame_time = 1000 / frame_rate;

  while (true) {
    uint32_t start = SDL_GetTicks();

    SDL_Event e;

    while (SDL_PollEvent(&e)) {
      switch (e.type) {
        case SDL_QUIT: this->bus->cartridge.dump_prg_ram(); return;
      }
    }

    // this->bus->nestest();
    this->bus->cpu.run_frame();
    this->draw();

    uint32_t current = SDL_GetTicks() - start;
    if (current < frame_time) {
      SDL_Delay(frame_time - current);
    }
  }
}
}  // namespace nes
