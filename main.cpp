#include <iostream>

#include "graphics.h"
//#include "tinyRandomMap.h"
#include "agent.h"
#include "largeRandomMap.h"

int main(int argc, char **argv) {
  unsigned int n = 16;

  //tinyRandomMap map(n, n, 0x0420FEFD, 96);
  while (1) {
    largeRandomMap *map = new largeRandomMap(n, 19);
    Agent *agent = new Agent(n, map);
    agent->traverse_matrix();

    free(agent);
    free(map);
  }

  return 0;
}
