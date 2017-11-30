#include <iostream>

//#include "graphics.h"
//#include "tinyRandomMap.h"
#include "agent.h"
#include "largeRandomMap.h"
#include <time.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  unsigned int n = 16;
  unsigned long seed = time(NULL);
  srand(seed);

  //tinyRandomMap map(n, n, 0x0420FEFD, 96);
  for(unsigned int i = 0; i < 40; ++i) {
    int wins = 0;
    unsigned int j = 0;
    for(j = 0; j < 1000; ++j) {
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
    std::cout << "Percentage of successful runs for " << i << " pits: " << percent << "%\n";
  }
  /*
  while (1) {
    largeRandomMap *map = new largeRandomMap(n, 19);
    Agent *agent = new Agent(n, map);
    agent->traverse_matrix();

    free(agent);
    free(map);
  }
  */
  return 0;
}
