#include "tinyRandomMap.h"
#define ITERATIONS 5
#define DEFAULT_MAP 16
#define DEFAULT_SEED 0x0420FEFD
#define DEFAULT_SEED2 0xCAFEFABF
#define DEFAULT_PITS 6
// XXXX -> (WUMP), (STENCH), (PIT), (BREEZE)

///////////////
// UTILITIES //
///////////////

/**
 * roll the given seed into a psuedo random value
 * @param  a    the number of matrix rows
 * @param  b    the number of matrix cols
 * @param  seed the value to use as a seed
 * @return      a psuedorandom number
 */
int rollSeed(unsigned int a, unsigned int b, int seed) {
  int roll = (a*b) ^ seed;

  for (size_t i = 0; i < ITERATIONS; i++) {
    roll = roll ^ seed;
    roll = roll ^ (a*b);
    roll = roll << i;
  }
  return roll;
}

//////////////////////////////
// PRIVATE MEMBER FUNCTIONS //
//////////////////////////////

/**
 * Sets the N, S, E and W values of the given matrix cell
 * @param a     the matrix row
 * @param b     the matrix col
 * @param input the value that is being placed there
 */
void tinyRandomMap::setupOrdinals(unsigned int a, unsigned int b, unsigned int input) {
  int curr = 0;
  input = input >> 1;
  //TinyMatrix checks for errors, so I don't have to.
  curr = tinyMat->Get(a-1, b);
  tinyMat->Set(a-1, b, input | curr);

  curr = tinyMat->Get(a+1, b);
  tinyMat->Set(a+1, b, input | curr);

  curr = tinyMat->Get(a, b-1);
  tinyMat->Set(a, b-1, input | curr);

  curr = tinyMat->Get(a, b+1);
  tinyMat->Set(a, b+1, input | curr);
}

/**
 * Sets the map.  Places the pits, then places the wumpus.  CAN overwrite the values,
 * but this depends on the seed.  Pick a good long seed!
 * @param a       the number of matrix rows
 * @param b       the number of matrix cols
 * @param seed    the seed to use
 * @param numPits the number of pits to place
 */
void tinyRandomMap::setMap(unsigned int a, unsigned int b, unsigned int seed, unsigned int numPits) {
  int roll = rollSeed(a, b, seed);

  for (size_t i = 0; i < numPits; i++) {
    if (roll == 0) {
      roll = rollSeed(a, b, DEFAULT_SEED2);
    }
    while ((roll&0xFF)%a == 0 && (roll&0xFF)%b == 0) {
      roll = roll >> 1;
      if (roll == 0) {
        roll = rollSeed(a, b, DEFAULT_SEED);
      }
    }
    tinyMat->Set((roll&0xFF+b)%a, ((roll >> 2)&0xFF+b)%b, 0b0010);
    this->setupOrdinals((roll&0xFF+b)%a, ((roll >> 2)&0xFF+b)%b, 0b0010);
    roll = roll >> 4;
  }

  while (roll%a == 0 && roll%b == 0) {
    roll = roll >> 1;
    if (roll == 0) {
      roll = rollSeed(a, b, DEFAULT_SEED);
    }
  }
  tinyMat->Set(((roll>>2)+a)%a, (roll-b)%b, 0b1000);
  this->setupOrdinals(((roll>>2)+a)%a, (roll-b)%b, 0b1000);

  roll >> 4;
  while (roll%a == 0 && roll%b == 0) {
    roll = roll >> 1;
    if (roll == 0) {
      roll = rollSeed(a, b, DEFAULT_SEED);
    }
  }

  goldRow = ((roll>>2)+a)%a;
  goldCol = (roll-b)%b;
  // XXX: I'm sorta assuming that you don't care about "phantom" signals, like a
  // breeze where there is no pit.  This will guarantee that (0,0) is always safe
  tinyMat->Set(0,0,0);
}

/////////////////
// CONSTRUCTOR //
/////////////////

/**
 * Default constructor.  Uses the DEFINE values above.
 */
tinyRandomMap::tinyRandomMap() {
  // When nothing is provided, assume a 16x16 matrix
  tinyMat = new TinyMatrix(DEFAULT_MAP, DEFAULT_MAP);

  //And use a set seed
  this->setMap(DEFAULT_MAP, DEFAULT_MAP, DEFAULT_SEED, DEFAULT_PITS);
}

/**
 * Overloaded constructor
 * @param a       the number of matrix rows
 * @param b       the number of matrix cols
 * @param seed    the seed to use
 * @param numPits the number of pits to place
 */
tinyRandomMap::tinyRandomMap(unsigned int a, unsigned int b, int seed, unsigned int numPits) {
  tinyMat = new TinyMatrix(a, b);

  this->setMap(a, b, seed, numPits);
}

/////////////////////////////
// PUBLIC MEMBER FUNCTIONS //
/////////////////////////////

/**
 * Getter for the TinyMatrix
 * @param  a the row value to get from
 * @param  b the column value to get from
 * @return   the value at tinyMat[a][b]
 */
unsigned char tinyRandomMap::get(unsigned int a, unsigned int b) {
  return tinyMat->Get(a, b);
}

/**
 * Setter for the TinyMatrix
 * @param a   the row value to insert at
 * @param b   the column value to insert at
 * @param val the value to insert
 */
void tinyRandomMap::set(unsigned int a, unsigned int b, unsigned int val) {
  tinyMat->Set(a, b, val);
}

/**
 * checks if the given cell has the gold
 * @param  a the row to check
 * @param  b the col to checks
 * @return   true if the gold is there, false otherwise
 */
bool tinyRandomMap::hasGold(unsigned int a, unsigned int b) {
  if (goldRow == a && goldCol == b) {
    return true;
  }
  return false;
}

void tinyRandomMap::dimensions(unsigned int *row, unsigned int *col) {
  if (tinyMat) {
    tinyMat->Dimensions(row, col);
    return;
  }

  *row = 0;
  *col = 0;
}

/**
 * Wrapper for the TinyMatrix->Output method
 */
void tinyRandomMap::print() {
  tinyMat->Output();
}

////////////////
// DESTRUCTOR //
////////////////

/**
 * Default destructor, just calls the destructor for tiny matrix
 */
tinyRandomMap::~tinyRandomMap() {
  delete tinyMat;
}
