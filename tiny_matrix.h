#ifndef __TINY_MATRIX__
#define __TINY_MATRIX__

#include <cstring>
#include <iomanip>
#include <iostream>

class TinyMatrix {
  public:
    TinyMatrix(unsigned int p_row, unsigned int p_col);
    ~TinyMatrix(void);

    void Set(unsigned int p_row, unsigned int p_col, unsigned int p_val);
    unsigned int Get(unsigned int p_row, unsigned int p_col);
    void Dimensions(unsigned int *row, unsigned int *col);

    void Output(void);

  private:
    unsigned int col, row, llen;
    unsigned char *matrix;
};

#endif /* __TINY_MATRIX__ */
