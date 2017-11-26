#ifndef TINY_RANDOM_MAP
#define TINY_RANDOM_MAP
#include "tiny_matrix.h"

class tinyRandomMap {
  private:
    TinyMatrix *tinyMat;
    void setupOrdinals(unsigned int, unsigned int, unsigned int);
    void clearAndSetupOrdinals(unsigned int, unsigned int, unsigned int);
    void setMap(unsigned int, unsigned int, unsigned int, unsigned int);

  public:
    tinyRandomMap();
    tinyRandomMap(unsigned int, unsigned int, int, unsigned int);
    unsigned char get(unsigned int, unsigned int);
    void set(unsigned int, unsigned int, unsigned int);
    void dimensions(unsigned int*, unsigned int*);
    void print();
    ~tinyRandomMap();
};

#endif /* end of include guard: TINY_RANDOM_MAP */
