#include <string>
#include <iostream>

//#include "graphics.h"
//#include "tinyRandomMap.h"
#include "agent.h"
#include "largeRandomMap.h"
#include <time.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  unsigned int n = 16, pit_count = 19;
  unsigned int it = 1000;
  unsigned long seed = time(NULL);
  srand(seed);

  if (argc > 2) {
    pit_count = std::stoi(argv[2]);
    n = std::stoi(argv[1]);
    it = std::stoi(argv[3]);
  } else if (argc > 1) {
    n = std::stoi(argv[1]);
  }

  if (n > 30) {
    n = 30;
  } else if (n < 8) {
    n = 8;
  }

  //tinyRandomMap map(n, n, 0x0420FEFD, 96);
  for(unsigned int i = 0; i < pit_count; ++i) {
    int wins = 0;
    unsigned int j = 0;
    for(j = 0; j < it; ++j) {
      largeRandomMap *map = new largeRandomMap(n, i, seed);
      Agent *agent = new Agent(n, map);
      agent->traverse_matrix();
      if(agent->get_has_gold()) {
        ++wins;
      }
      delete agent;
      delete map;
    }
    double percent = ((double)wins / (double)j) * 100;
    std::cout << "Percentage of successful runs [" << it << " iterations] [" << i << " pits] " << "[" << n << "x" << n << "world]: " << percent << "%\n";
  }
  /*
  while (1) {
    largeRandomMap *map = new largeRandomMap(n, pit_count);
    Agent *agent = new Agent(n, map);
    agent->traverse_matrix();

    free(agent);
    free(map);
  }
  */
  return 0;
}
