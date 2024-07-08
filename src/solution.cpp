#include <map>
#include <set>

#include "state.cpp"
#include "tree.cpp"

void give_children(node<state> *seed,
                   std::map<std::string, node<state> *> &map) {

  // get the starting state as a string
  std::string state_str = seed->data.rows;

  // change who's turn it is to the other player
  state_str.back() = (state_str.back() == 1) ? 2 : 1;

  // Each node will have exactly 1 + 3 + 5 + 7 = 16 edges
  // And the number of children a node has is the sum of the sticks it holds
  // So the number of parents must be 16 - the sum of the sticks
  unsigned char sum = state_str[0] + state_str[1] + state_str[2] + state_str[3];

  // reserve space in the vectors for better performance
  seed->children.reserve(sum);
  seed->parents.reserve(16 - sum);

  for (unsigned row = 0; row < 4; ++row) {
    std::string new_str = state_str;

    for (unsigned sticks = 0; sticks < state_str[row]; ++sticks) {
      new_str[row] -= 1;

      if (map.count(new_str) == 0) {

        // create new state
        state new_state;
        new_state.rows = new_str;
        node<state> *new_node = new node<state>(new_state);

        // add state node to the map
        map[new_str] = new_node;

        // add the state to the parent
        seed->children.push_back(new_node);

        // add parent to new_state
        new_node->parents.push_back(seed);

        // generate children for the new state
        give_children(new_node, map);

      } else { // if the state has already been made

        // add the child to the parent
        seed->children.push_back(map[new_str]);

        // add the parent to the child
        map[new_str]->parents.push_back(seed);

        // If the state has already been made, then it does not have to have its
        // children generated
      }
    }
  }
}

void start_solve(std::set<node<state> *> &solved, node<state> *start) {

  // check every parent for validity
  for (node<state> *parent : start->parents) {
    bool valid = true;
    // if every child of the parent is in the set, it is valid
    for (node<state> *child : parent->children) {
      if (solved.find(child) == solved.end()) {
        valid = false;                                    
        break;
      }
    }
    if (valid) {
      // add the parent to the set
      solved.insert(parent);

      // add all grandparents to the set (since it is our turn)
      for (node<state> *grandparent : parent->parents) {
        solved.insert(grandparent);
      }

      // start solving on the grandparents
      for (node<state> *grandparent : parent->parents) {
        start_solve(solved, grandparent);
      }
      // two seperate loops so that "solved" is properly populated
    }
  }
}

void solve(const char player_to_check,
           std::map<std::string, node<state> *> &map,
           std::set<node<state> *> &solved) {

  // get the pointer of the "player_to_check wins" state
  std::string p_win = "0000X";
  std::transform(p_win.begin(), p_win.end(), p_win.begin(),
                 [](char c) { return c - 48; });

  p_win[4] = player_to_check;

  // insert the winning node
  solved.insert(map[p_win]);

  // add the states before this that gaurentee wins
  std::string temp;
  temp.resize(5);

  temp[4] = (player_to_check == 1) ? 2 : 1;
  temp[0] = temp[1] = temp[2] = temp[3] = 0;

  for (int i = 0; i < 4; ++i) {
    std::string winner = temp;

    winner[i] = 1;

    solved.insert(map[winner]);

    // insert all the parents of these states since it is our turn
    // begin the recursive solution on them as well
    for (node<state> *n : map[winner]->parents) {
      solved.insert(n);
      start_solve(solved, n);
    }
  }
}
