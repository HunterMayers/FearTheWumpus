#include <iostream>
#include <iomanip>
#include "agent.h"
#include "tinyRandomMap.h"

using namespace std;

/*
* This function interfaces with our map pointer
* to get the current status of our curr position.
* */
unsigned char Agent::get_bits(unsigned int x, unsigned int y) {
  //gold - breeze - stench
  //1 == breeze 0000 0001
  //5 == stench 0000 0101
  //2 == pit    0000 0010
  //8 == wumpus 0000 1000
  unsigned char a = m_map->get(x,y);
  unsigned char b = 0;
  if(a == 1) {
    b |= 0x1;
  }
  if(a == 5) {
    b |= 0x2;
  }
  if(a == 2) {
    b |= 0x3;
  }
  if(a == 8) {
    b |= 0x4;
  }
  return b;
}

/*
 * This function is for testing you can move around
 * */
void Agent::move(char move) {
  if(move == 'h') {
    if(agent_y_position > 0) {
      --agent_y_position;
    }
  }
  if(move == 'k') {
    if(agent_x_position > 0) {
      --agent_x_position;
    }
  }
  if(move == 'j') {
    if(agent_x_position < m_dimension - 1) {
      ++agent_x_position;
    }
  }
  if(move == 'l') {
    if(agent_y_position < m_dimension - 1) {
      ++agent_y_position;
    }
  }
  update_current(agent_x_position, agent_y_position);
}

/*
 * Initialize starting position and internal map
 * */
Agent::Agent(unsigned int dimension, tinyRandomMap *m) {
  m_map = m;
  m_dimension = dimension;
  internal_map.resize(dimension);
  for(unsigned int i = 0; i < dimension; ++i) {
    internal_map[i].resize(dimension);
    for(unsigned int j = 0; j < dimension; ++j) {
      internal_map[i][j] = new Node(i, j, i * dimension + j);
    }
  }
  //set up initial player position
  agent_x_position = dimension - 1;
  agent_y_position = 0;
  update_current(agent_x_position, agent_y_position);
}

/*
 * Free up any nodes once the agent is done
 * */
Agent::~Agent() {
  for(unsigned int i = 0; i < internal_map.size(); ++i) {
    for(unsigned int j = 0; j < internal_map[i].size(); ++j) {
      delete internal_map[i][j];
    }
  }
}

/*
 * Prints out the map in ascii characters
 * */
void Agent::print_nodes() {
  for(unsigned int i = 0; i < internal_map.size(); ++i) {
    for(unsigned int j = 0; j < internal_map[i].size(); ++j) {
      if(agent_x_position == i && agent_y_position == j) {
        cout << "AA" << ' ';
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
 * Creates a node intializing starting values
 * */
Agent::Node::Node(unsigned int i, unsigned int j, int value) {
  m_value = value;
  node_x_position = i;
  node_y_position = j;
  wumpus = unknown;
  pit = unknown;
  safe = false;
  parent = NULL;
  color = white;
}

/*
 * Updates the current node based on current location factors
 * */
void Agent::update_current(unsigned int cur_x, unsigned int cur_y) {
  unsigned char bits = get_bits(cur_x, cur_y);
  bool breeze, stench, wumpus, pit;
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

void Agent::found_wumpus() {
  for(unsigned int i = 0; i < internal_map.size(); ++i) {
    for(unsigned int j = 0; j < internal_map[i].size(); ++j) {
      if(internal_map[i][j]->wumpus != Node::present) {
        internal_map[i][j]->wumpus = Node::clear;
      }
    }
  }
}

void Agent::print_m_map() {
  for(unsigned int i = 0; i < m_dimension; ++i) {
    for(unsigned int j = 0; j < m_dimension; ++j) {
      cout << (unsigned int)(m_map->get(i,j)) << ' ';
    }
    cout << '\n';
  }
}
