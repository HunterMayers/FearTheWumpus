#include "tiny_matrix.h"

TinyMatrix::TinyMatrix(unsigned int p_row, unsigned int p_col) {
  unsigned int len;

  row = p_row;
  col = p_col;
  llen = p_row*p_col;

  len = (llen >> 1)+(llen & 0x1);

  matrix = new unsigned char[len];

  memset(matrix, 0, len);
}

TinyMatrix::~TinyMatrix(void) {
  if (matrix) { delete matrix; }
}

void TinyMatrix::Set(unsigned int p_row, unsigned int p_col, unsigned int p_val) {
  if ((p_row < row) && (p_col < col) && (p_val <= 0xF)) {
    unsigned int i = p_col+p_row*col,
                 j = i >> 1;

    *(matrix+j) = (i & 0x1) ? ((*(matrix+j) & 0xF) | ((p_val << 4) & 0xF0)) : ((*(matrix+j) & 0xF0) | (p_val & 0xF));
  }
}

unsigned int TinyMatrix::Get(unsigned int p_row, unsigned int p_col) {
  if ((p_row < row) && (p_col < col)) {
    unsigned int i = p_col+p_row*col,
                 j = i >> 1;

    return (i & 0x1) ? ((*(matrix+j) & 0xF0) >> 4) : (*(matrix+j) & 0xF);
  }

  return 0xFF;
}

void TinyMatrix::Dimensions(unsigned int *p_row, unsigned int *p_col) {
  *p_row = row;
  *p_col = col;
}

void TinyMatrix::Output(void) {
  for (unsigned int i = 0; i < llen; i += 1) {
    std::cout << std::setw(2) << Get(i/col, i%col) << (((i+1)%col) ? " " : "\n");
  }
}
