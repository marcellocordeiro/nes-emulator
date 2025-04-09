#include <memory>

#define SDL_MAIN_HANDLED
#include <SDL.h>

class SDLHelper {
public:
  explicit SDLHelper(Uint32 flags) {
    SDL_Init(flags);
    ++counter;
  }

  ~SDLHelper() {
    --counter;
    if (!counter) SDL_Quit();
  }

  struct Deleter {
    void operator()(SDL_Window* ptr) {
      if (ptr) SDL_DestroyWindow(ptr);
    }

    void operator()(SDL_Renderer* ptr) {
      if (ptr) SDL_DestroyRenderer(ptr);
    }

    void operator()(SDL_Texture* ptr) {
      if (ptr) SDL_DestroyTexture(ptr);
    }
  };

  using Window = std::unique_ptr<SDL_Window, Deleter>;
  using Renderer = std::unique_ptr<SDL_Renderer, Deleter>;
  using Texture = std::unique_ptr<SDL_Texture, Deleter>;

private:
  static inline int counter = 0;
};
