//standard library
#include <iostream>
//DIP
#include "Renderer.h"
//main
int main(int argc, char* argv[])
{
  Renderer renderer;

  renderer.init();
  renderer.run();
  //cleanup - destroy VAOs, FBOs, ...
  return 0;
}
