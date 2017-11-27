#include <iostream>

#include "graphics.h"
#include "tinyRandomMap.h"
#include "agent.h"

int main(int argc, char **argv) {
  unsigned int n = 16;

  tinyRandomMap map(n, n, 0x0420FEFD, 96);
  Agent agent(n, &map);
  agent.traverse_matrix();

  return 0;
}
