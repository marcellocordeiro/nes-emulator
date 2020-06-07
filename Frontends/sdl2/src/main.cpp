#include "main_window.h"

int main(int argc, char* argv[])
{
  main_window w(argc, argv);

  w.init();
  w.run();
}