#include <iostream>
#include <iomanip>
#include "agent.h"
#include "tinyRandomMap.h"

using namespace std;


/*
* Checks if cell is safe to move to
* @param x the vertical index into matrix
* @param y the horizontal index into matrix
* @return bool true if safe, false if unsafe
*/
bool Agent::is_safe(unsigned int x, unsigned int y) {
  if(internal_map[x][y]->pit == Node::clear && internal_map[x][y]->wumpus == Node::clear) {
    return true;
  }
  else {
    return false;
  }
};

/*
* Interface with external map to get data on cells
* @param x the vertical index into matrix
* @param y the horizontal index into matrix
* @return unsigned char Has bits flipped if one of the possible
*                       values if present 0th is right most bit:
*                                          0th bit - breeze
*                                          1st bit - pit
*                                          2nd bit - stench
*                                          3rd bit - wumpus
*                                          4th bit - gold 
*/
unsigned char Agent::get_bits(unsigned int x, unsigned int y) {
  unsigned char a = m_map->get(x,y);
  bool c = m_map->hasGold(x, y);
  unsigned char b = 0;
  if(a & 0x1) {
    b |= 0x1;
  }
  if(a & 0x4) {
    b |= 0x2;
  }
  if(a & 0x2) {
    b |= 0x3;
  }
  if(a & 0x8) {
    b |= 0x4;
  }
  if(c) {
    b |= 0x16;
  }
  return b;
}

/*
* Allows input so you can manually move around agent
* @param move Takes either h,j,k,l to move in their vim-direction
*/
void Agent::move(char move) {
  if(move == 'h') {
    if(agent_y_position > 0 && is_safe(agent_x_position, agent_y_position - 1)) {
      --agent_y_position;
    }
  }
  if(move == 'k') {
    if(agent_x_position > 0 && is_safe(agent_x_position - 1, agent_y_position)) {
      --agent_x_position;
    }
  }
  if(move == 'j') {
    if(agent_x_position < m_dimension - 1 && is_safe(agent_x_position + 1, agent_y_position)) {
      ++agent_x_position;
    }
  }
  if(move == 'l') {
    if(agent_y_position < m_dimension - 1 && is_safe(agent_x_position, agent_y_position + 1)) {
      ++agent_y_position;
    }
  }
  update_current(agent_x_position, agent_y_position);
}

/*
* Initializes agent variables and creates the internal map
* @param dimension The size of the matrix(dimension x dimension)
* @param tinyRandomMap* A pointer to the external map
*/
Agent::Agent(unsigned int dimension, tinyRandomMap *m) {
  m_map = m;
  m_dimension = dimension;
  internal_map.resize(dimension);
  for(unsigned int i = 0; i < dimension; ++i) {
    internal_map[i].resize(dimension);
    for(unsigned int j = 0; j < dimension; ++j) {
      internal_map[i][j] = new Node(i, j);
    }
  }
  agent_has_gold = false;
  agent_x_position = dimension - 1;
  agent_y_position = 0;
  update_current(agent_x_position, agent_y_position);
}

/*
* Free up any nodes once the agent is done
**/
Agent::~Agent() {
  for(unsigned int i = 0; i < internal_map.size(); ++i) {
    for(unsigned int j = 0; j < internal_map[i].size(); ++j) {
      delete internal_map[i][j];
    }
  }
}

/*
 * Prints out the map in ascii characters for testing/debugging.
 * */
void Agent::print_nodes() {
  for(unsigned int i = 0; i < internal_map.size(); ++i) {
    for(unsigned int j = 0; j < internal_map[i].size(); ++j) {
      if(agent_x_position == i && agent_y_position == j) {
        if(agent_has_gold) {
          cout << "AG" << ' ';
        }
        else {
          cout << "AA" << ' ';
        }
      }
      else {
        if(internal_map[i][j]->pit == Node::clear) {
          cout << "S";
        }
        if(internal_map[i][j]->pit == Node::unknown) {
          cout << "?";
        }
        if(internal_map[i][j]->pit == Node::present) {
          cout << "P";
        }
        if(internal_map[i][j]->wumpus == Node::clear) {
          cout << "S" << ' ';
        }
        if(internal_map[i][j]->wumpus == Node::unknown) {
          cout << "?" << ' ';
        }
        if(internal_map[i][j]->wumpus == Node::present) {
          cout << "W" << ' ';
        }
      }
    }
    cout << '\n';
  }
}

/*
* Creates a node intializing starting values.
* @param i The vertical index of this node
* @param j The horizontal index of this node
**/
Agent::Node::Node(unsigned int i, unsigned int j) {
  node_x_position = i;
  node_y_position = j;
  wumpus = unknown;
  pit = unknown;
  parent = NULL;
  color = white;
}

/*
* Function to traverse parent array once the agent finds the gold.
*/
void Agent::return_home() {
  Node * cur_node = internal_map[agent_x_position][agent_y_position];
  while(cur_node->parent) {
    DFS_move(cur_node->parent->node_x_position, cur_node->parent->node_y_position);
  }
}

/*
* Updates the current node and nodes left, right, up, and down of current node.
* @param unsigned int The vertical index of current node
* @param unsigned int The horizontal index of current node
**/
void Agent::update_current(unsigned int cur_x, unsigned int cur_y) {
  //Get the bits(status) of the current node
  unsigned char bits = get_bits(cur_x, cur_y);
  bool breeze, stench, wumpus, pit;
  //Check if current node has the gold
  if(bits & 0x16) {
    agent_has_gold = true;
    return_home();
  }
  //This section updates the current status of the node we are in
  if(bits & 0x1) {
    breeze = true;
  }
  else {
    breeze = false;
  }
  if(bits & 0x2) {
    stench = true;
  }
  else {
    stench = false;
  }
  if(bits & 0x3) {
    pit = true;
  }
  else {
    pit = false;
  }
  if(bits & 0x4) {
    wumpus = true;
  }
  else {
    wumpus = false;
  }
  if(!wumpus) {
    internal_map[cur_x][cur_y]->wumpus = Node::clear;
  }
  else {
    //game should end; we lost;
  }
  if(!pit) {
    internal_map[cur_x][cur_y]->pit = Node::clear;
  }
  else {
    //game shoudl end; we lost;
  }
  if(!breeze) {
    //update up
    if(cur_x > 0) {
      (internal_map[cur_x - 1][cur_y])->pit = Node::clear;
    }
    //update down
    if(cur_x < m_dimension - 1) {
      (internal_map[cur_x + 1][cur_y])->pit = Node::clear;
    }
    //update left
    if(cur_y > 0) {
      (internal_map[cur_x][cur_y - 1])->pit = Node::clear;
    }
    //update right
    if(cur_y < m_dimension - 1) {
      (internal_map[cur_x][cur_y + 1])->pit = Node::clear;
    }
  }
  if(!stench) {
    //update up
    if(cur_x > 0) {
      (internal_map[cur_x - 1][cur_y])->wumpus = Node::clear;
    }
    //update down
    if(cur_x < m_dimension - 1) {
      (internal_map[cur_x + 1][cur_y])->wumpus = Node::clear;
    }
    //update left
    if(cur_y > 0) {
      (internal_map[cur_x][cur_y - 1])->wumpus = Node::clear;
    }
    //update right
    if(cur_y < m_dimension - 1) {
      (internal_map[cur_x][cur_y + 1])->wumpus = Node::clear;
    }
  }
  if(stench) {
    int count = 0;
    int number_of_possible_nodes = 0;
    char dir;
    if(cur_x > 0) {
      ++number_of_possible_nodes;
      if(internal_map[cur_x - 1][cur_y]->wumpus == Node::clear) {
        ++count;
      }
      else {
        dir = 'N';
      }
    }
    if(cur_x < m_dimension - 1) {
      ++number_of_possible_nodes;
      if(internal_map[cur_x + 1][cur_y]->wumpus == Node::clear) {
        ++count;
      }
      else {
        dir = 'S';
      }
    }
    if(cur_y > 0) {
      ++number_of_possible_nodes;
      if(internal_map[cur_x][cur_y - 1]->wumpus == Node::clear) {
        ++count;
      }
      else {
        dir = 'W';
      }
    }
    if(cur_y < m_dimension - 1) {
      ++number_of_possible_nodes;
      if(internal_map[cur_x][cur_y + 1]->wumpus == Node::clear) {
        ++count;
      }
      else {
        dir = 'E';
      }
    }
    if(count == number_of_possible_nodes - 1) {
      if(dir == 'N') {
        internal_map[cur_x - 1][cur_y]->wumpus = Node::present;
      }
      if(dir == 'S') {
        internal_map[cur_x + 1][cur_y]->wumpus = Node::present;
      }
      if(dir == 'W') {
        internal_map[cur_x][cur_y - 1]->wumpus = Node::present;
      }
      if(dir == 'E') {
        internal_map[cur_x][cur_y + 1]->wumpus = Node::present;
      }
      found_wumpus();
    }
  }
  if(breeze) {
    int count = 0;
    int number_of_possible_nodes = 0;
    char dir;
    if(cur_x > 0) {
      ++number_of_possible_nodes;
      if(internal_map[cur_x - 1][cur_y]->pit== Node::clear) {
        ++count;
      }
      else {
        dir = 'N';
      }
    }
    if(cur_x < m_dimension - 1) {
      ++number_of_possible_nodes;
      if(internal_map[cur_x + 1][cur_y]->pit== Node::clear) {
        ++count;
      }
      else {
        dir = 'S';
      }
    }
    if(cur_y > 0) {
      ++number_of_possible_nodes;
      if(internal_map[cur_x][cur_y - 1]->pit== Node::clear) {
        ++count;
      }
      else {
        dir = 'W';
      }
    }
    if(cur_y < m_dimension - 1) {
      ++number_of_possible_nodes;
      if(internal_map[cur_x][cur_y + 1]->pit == Node::clear) {
        ++count;
      }
      else {
        dir = 'E';
      }
    }
    if(count == number_of_possible_nodes - 1) {
      if(dir == 'N') {
        internal_map[cur_x - 1][cur_y]->pit = Node::present;
      }
      if(dir == 'S') {
        internal_map[cur_x + 1][cur_y]->pit = Node::present;
      }
      if(dir == 'W') {
        internal_map[cur_x][cur_y - 1]->pit = Node::present;
      }
      if(dir == 'E') {
        internal_map[cur_x][cur_y + 1]->pit = Node::present;
      }
    }
  }
  print_nodes();
}

/*
* This function is called after we find the wumpus, since there is
* only one wumpus we can set every other node to be safe from having
* the wumpus.
*/
void Agent::found_wumpus() {
  for(unsigned int i = 0; i < internal_map.size(); ++i) {
    for(unsigned int j = 0; j < internal_map[i].size(); ++j) {
      if(internal_map[i][j]->wumpus != Node::present) {
        internal_map[i][j]->wumpus = Node::clear;
      }
    }
  }
}

/*
* Causes our agent to traverse the matrix and look for the gold.
*/
void Agent::traverse_matrix() {
  matrix_DFS_visit(internal_map[agent_x_position][agent_y_position]);
}

/*
* The DFS implementation that our agent uses to traverse the matrix.
* @param u The node we are current calling matrix_DFS_visit on.
*/
void Agent::matrix_DFS_visit(Node * u) {
  //Check to see if we already found the gold, if we have we shouldn't continue looking.
  if(!agent_has_gold) {
    u->color = Node::gray;
    unsigned int cur_x = u->node_x_position;
    unsigned int cur_y = u->node_y_position;
    //explore right edge
    if(cur_y < m_dimension - 1 && is_safe(cur_x, cur_y + 1) && internal_map[cur_x][cur_y + 1]->color == Node::white) {
      DFS_move(cur_x, cur_y + 1);
      matrix_DFS_visit(internal_map[cur_x][cur_y + 1]);
      DFS_move(cur_x, cur_y);
    }
    //explore down edge
    if(cur_x < m_dimension - 1 && is_safe(cur_x + 1, cur_y) && internal_map[cur_x + 1][cur_y]->color == Node::white) {
      DFS_move(cur_x + 1, cur_y);
      matrix_DFS_visit(internal_map[cur_x + 1][cur_y]);
      DFS_move(cur_x, cur_y);
    }
    //explore left edge
    if(cur_y > 0 && is_safe(cur_x, cur_y - 1) && internal_map[cur_x][cur_y - 1]->color == Node::white) {
      DFS_move(cur_x, cur_y - 1);
      matrix_DFS_visit(internal_map[cur_x][cur_y - 1]);
      DFS_move(cur_x, cur_y);
    }
    //explore up edge
    if(cur_x > 0 && is_safe(cur_x - 1, cur_y) && internal_map[cur_x - 1][cur_y]->color == Node::white) {
      DFS_move(cur_x - 1, cur_y);
      matrix_DFS_visit(internal_map[cur_x - 1][cur_y]);
      DFS_move(cur_x, cur_y);
    }
    u->color = Node::black;
  }
}

/*
* A method to allow our agent to move around in the matrix to stay in step with
* our DFS algorithm.
* @param x The vertical position in our matrix we are moving to.
* @param y The horizontal position in our matrix we are moving to.
*/
void Agent::DFS_move(unsigned int x, unsigned int y) {
  agent_x_position = x;
  agent_y_position = y;
  update_current(agent_x_position, agent_y_position);
}