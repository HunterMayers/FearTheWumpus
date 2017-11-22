#include <iostream>

#include "graphics.h"
#include "tinyRandomMap.h"

int main(int argc, char **argv) {
  unsigned int n = 16;

  Graphics graphics;
  tinyRandomMap map(n, n, 0x0420FEFD, 96);

  graphics.Render(&map);

  SDL_Delay(10000);

  return 0;
}
