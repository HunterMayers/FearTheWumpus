#include <iostream>

//#include "graphics.h"
#include "tinyRandomMap.h"
#include "agent.h"

int main(int argc, char **argv) {
  unsigned int n = 16;

  //Graphics graphics;
  tinyRandomMap map(n, n, 0x0420FEFD, 96);
  Agent * a = new Agent(n, &map);
  a->traverse_matrix();


  //graphics.Render(&map);

  //SDL_Delay(10000);

  return 0;
}
