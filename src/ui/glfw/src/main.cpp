#include "MainWindow.h"

auto main(int argc, char* argv[]) -> int {
  MainWindow w(argc, argv);

  w.show();

  w.run();

  return 0;
}
