#ifndef AGENT_H
#define AGENT_H

#include <vector>
#include "graphics.h"
//#include "tinyRandomMap.h"
#include "largeRandomMap.h"

enum Direction {
  east,
  north,
  west,
  south,
  none
};

class Agent {
public:
  Agent(unsigned int, largeRandomMap *);
  ~Agent();
  bool is_safe(unsigned int, unsigned int);
  void print_nodes();
  void update_current(unsigned int, unsigned int);
  void move(char);
  void found_wumpus();
  unsigned char get_bits(unsigned int, unsigned int);
  unsigned char get_internal_bits(unsigned int, unsigned int, unsigned int*);
  void DFS_move(unsigned int, unsigned int);
  void traverse_matrix();
private:
  class Node {
  public:
    Node(unsigned int, unsigned int);
    enum Color {white, gray, black};
    enum State {present, clear, unknown};
    State wumpus;
    State pit;
    Direction dir;
    bool breeze;
    bool stench;
    bool visited;
    Node * parent;
    Color color;
    unsigned int node_x_position;
    unsigned int node_y_position;
  };
  void matrix_DFS_visit(Node *);
  std::vector< std::vector< Node * > > internal_map;
  unsigned int agent_x_position, agent_x_prev;
  unsigned int agent_y_position, agent_y_prev;
  unsigned int m_dimension;
  largeRandomMap * m_map;
  Graphics graphics;
  bool agent_has_gold;
protected:
};
#endif
