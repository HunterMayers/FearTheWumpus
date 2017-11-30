#include <string>
#include <iostream>

#include "graphics.h"
//#include "tinyRandomMap.h"
#include "agent.h"
#include "largeRandomMap.h"

int main(int argc, char **argv) {
  unsigned int n = 16, pit_count = 19;

  if (argc > 2) {
    pit_count = std::stoi(argv[2]);
    n = std::stoi(argv[1]);
  } else if (argc > 1) {
    n = std::stoi(argv[1]);
  }

  if (n > 30) {
    n = 30;
  } else if (n < 8) {
    n = 8;
  }

  //tinyRandomMap map(n, n, 0x0420FEFD, 96);
  while (1) {
    largeRandomMap *map = new largeRandomMap(n, pit_count);
    Agent *agent = new Agent(n, map);
    agent->traverse_matrix();

    free(agent);
    free(map);
  }

  return 0;
}
