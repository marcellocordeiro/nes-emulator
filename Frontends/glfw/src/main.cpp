#include "MainWindow.h"

int main(int argc, char* argv[])
{
  MainWindow w; 
  
  w.setArgs(argc, argv);
  w.show();

  w.run();

  return 0;
}
