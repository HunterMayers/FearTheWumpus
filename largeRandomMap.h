#ifndef _largeRandomMap

#define _largeRandomMap

#include <vector>

class largeRandomMap {
public:
  largeRandomMap(unsigned int, unsigned int);
  unsigned char get(unsigned int, unsigned int);
  void place_gold(unsigned int, unsigned int);
  void place_wumpus(unsigned int, unsigned int);
  void place_pit(unsigned int, unsigned int);
  bool hasGold(unsigned int, unsigned int);
  void print();
private:
  std::vector< std::vector<unsigned char> > map;
  unsigned int dimension;
protected:
};
#endif
