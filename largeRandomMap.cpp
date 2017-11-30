#include <time.h>
#include <stdlib.h>
#include <limits.h>
#include <vector>
#include <utility>
#include <iostream>
#include "largeRandomMap.h"

using namespace std;

largeRandomMap::largeRandomMap(unsigned int N, unsigned int numPits) {
  dimension = N;
  map.resize(N); 
  for(unsigned int i = 0; i < map.size(); ++i) {
    map[i].resize(N, 0);
  }
  unsigned int roll_x;
  unsigned int roll_y;
  unsigned long seed_val = time(NULL);
  bool valid;
  int count;
  vector< pair< unsigned int, unsigned int > > taken;
  //Initial player values we don't want to overlap
  taken.push_back(make_pair(N - 1, 0));
  taken.push_back(make_pair(N - 2, 0));
  taken.push_back(make_pair(N - 1, 1));

  //seed rand()
  srand(seed_val);

  //place the gold
  count = 1;
  while(count) {
    valid = true; 
    roll_x = rand() % N;
    roll_y = rand() % N;
    //check the value isn't already taken
    for(unsigned int i = 0; i < taken.size(); ++i) {
      if(roll_x == taken[i].first && roll_y == taken[i].second) {
        valid = false;
      }
    }
    if(valid) {
      taken.push_back(make_pair(roll_x, roll_y));
      --count;
      place_gold(roll_x, roll_y);
    }
  }

  //place the wumpus
  count = 1;
  while(count) {
    valid = true; 
    roll_x = rand() % N;
    roll_y = rand() % N;
    //check the value isn't already taken
    for(unsigned int i = 0; i < taken.size(); ++i) {
      if(roll_x == taken[i].first && roll_y == taken[i].second) {
        valid = false;
      }
    }
    if(valid) {
      taken.push_back(make_pair(roll_x, roll_y));
      --count;
      place_wumpus(roll_x, roll_y);
    }
  }

  //place pits
  count = numPits;
  while(count) {
    valid = true; 
    roll_x = rand() % N;
    roll_y = rand() % N;
    //check the value isn't already taken
    for(unsigned int i = 0; i < taken.size(); ++i) {
      if(roll_x == taken[i].first && roll_y == taken[i].second) {
        valid = false;
      }
    }
    if(valid) {
      taken.push_back(make_pair(roll_x, roll_y));
      --count;
      place_pit(roll_x, roll_y);
    }
  }

  printf("Seed: %lu.\n", seed_val);
}

unsigned int largeRandomMap::get_dimension(void) {
  return dimension;
}

void largeRandomMap::place_gold(unsigned int x, unsigned int y) {
  map[x][y] |= 0x10;
}

void largeRandomMap::place_wumpus(unsigned int x, unsigned int y) {
  map[x][y] |= 0x8;
  if(x > 0) {
    map[x - 1][y] |= 0x4;
  }
  if(x < dimension - 1) {
    map[x + 1][y] |= 0x4;
  }
  if(y > 0) {
    map[x][y - 1] |= 0x4;
  }
  if(y < dimension - 1) {
    map[x][y + 1] |= 0x4;
  }
}

void largeRandomMap::place_pit(unsigned int x, unsigned int y) {
  map[x][y] |= 0x2;
  if(x > 0) {
    map[x - 1][y] |= 0x1;
  }
  if(x < dimension - 1) {
    map[x + 1][y] |= 0x1;
  }
  if(y > 0) {
    map[x][y - 1] |= 0x1;
  }
  if(y < dimension - 1) {
    map[x][y + 1] |= 0x1;
  }
}

unsigned char largeRandomMap::get(unsigned int x, unsigned int y) {
  return map[x][y];
}

bool largeRandomMap::hasGold(unsigned int x, unsigned int y) {
  if(map[x][y] & 0x10) {
    return true;
  }
  else {
    return false;
  }
}


void largeRandomMap::print() {
  for(unsigned int i = 0; i < map.size(); ++i) {
    for(unsigned int j = 0; j < map[i].size(); ++j) {
      cout << (int)(map[i][j]) << ' ';
    }
    cout << '\n';
  }
}
