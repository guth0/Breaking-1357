#include <iostream>
#include <map>
#include <set>
#include <string>

#include "src/solution.cpp"
#include "src/state.cpp"
#include "src/tree.cpp"

node<state> *make_move(std::set<node<state> *> solved,
                         std::map<std::string, node<state> *> map,
                         node<state> *current_state) {

  if (current_state->children.size() == 0) {
    // if this hits, then someone has won
    return nullptr;
  }

  // if the current state is in our set, iterate over the children of the
  // current state, if any of them are in the set, then choose that one if the
  // current state is not in the set, then itereate over the children of the
  // current state, and choose the one with highest percentage of children in
  // the set

  if (solved.find(current_state) != solved.end()) {

    // find a child of the current_state in the set and return it
    for (node<state> *child : current_state->children) {
      if (solved.find(child) != solved.end()) {
        return child;
        // since we are in the solved set, it does not matter which
        // node we choose, as long as it is in the set as well
      }
    }

    throw std::logic_error(
        "A node in the solved set did not have a child also in the solved set");
    // all states in the set (except for the winning position) must have at
    // least one child also in the set
    // therefore, we will always hit the previous if statement

  } else {

    float best_chance = 0;
    int best_pos = 0;
                                 
    // iterate over all the children to see which one has the highest percentage
    // of children in the solved set and return that one
    for (int i = 0; i < current_state->children.size(); ++i) {
      node<state> *child = current_state->children[i];
      float curr_chance = 0;
      for (node<state> *grandchild : child->children) {
        if (solved.find(grandchild) != solved.end()) {
          curr_chance += 1;
        }
      }
      curr_chance /= child->children.size();

      if (curr_chance > best_chance) {
        best_chance = curr_chance;
        best_pos = i;
      }
    }
    // if none of the grandchildren are in the set,
    // it just picks the first move in the vector
    return current_state->children[best_pos];
  }
}

int main(int argc, char *argv[]) {

  if (argc != 2) {
    throw std::invalid_argument("Choose player 1 or 2 to play as");
    return 1;
  }

  if (argv[1][0] != '1' && argv[1][0] != '2') {
    throw std::invalid_argument("Argument must be either 1 or 2");
    return 1;
  }

  char player_num = argv[1][0] - 48; // ascii trickery
  char bot_num = (player_num == 1) ? 2 : 1;

  // map to store the nodes
  // makes it so there is only one copy of any given state
  std::map<std::string, node<state> *> map;

  // make the first state
  state start;

  node<state> * head = new node<state>(start);

  // add the first node to the map
  map[start.rows] = head;

  // generate all nodes and edges (recursively)
  give_children(head, map);

  // create a set to store all the states that guarentee wins
  std::set<node<state> *> solved;

  // fill the set
  solve(bot_num, map, solved);

  // NOW START THE GAME
  // get the state of the game (if p1, then we know the first one)
  // get our next move, print that to the terminal
  // ask for the next state

  node<state> *curr_state = head;

  if (bot_num == 1) {
    curr_state = make_move(solved, map, curr_state);
  }


  std::string temp = "0000X";
  temp[0] = temp[1] = temp[2] = temp[3] = 0;

  temp[4] = 1;
  node<state> *p1_win = map[temp];

  temp[4] = 2;
  node<state> *p2_win = map[temp];


  while (curr_state != p1_win && curr_state != p2_win &&
         curr_state != nullptr) {
    std::cout << "Current state: "
              << curr_state->data.to_str().substr(0, 4)
              << std::endl
              << "Enter next state: ";

    std::string newstate = "0000X";
    std::cin >> newstate;

    std::transform(newstate.begin(), newstate.end(), newstate.begin(),
                   [](char c) { return c - 48; });

    newstate += bot_num;

    while (newstate.size() != 5 || !curr_state->data.next_to(newstate)) {
      std::cout << "Invalid Argument, try again." << std::endl;
      std::cout << "Enter next state: ";
      std::cin.clear();
      std::cin >> newstate;
      std::transform(newstate.begin(), newstate.end(), newstate.begin(),
                     [](char c) { return c - 48; });

      newstate += bot_num;
    }

    curr_state = make_move(solved, map, map[newstate]);
  }

  if (curr_state == nullptr) {
    std::cout << "You have been defeated" << std::endl;
  } else {
    std::cout << "You have bested me" << std::endl;
  }

  return 0;
}
