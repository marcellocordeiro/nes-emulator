#include "sdl2_frontend.h"

int main(int argc, char* argv[])
{
  nes::sdl2_frontend frontend(argc, argv);

  frontend.init();
  frontend.run();
}
