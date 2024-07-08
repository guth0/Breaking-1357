#include <iostream>
#include <map>
#include <set>
#include <string>

#include "src/debug_tools.cpp"
#include "src/solution.cpp"
#include "src/state.cpp"
#include "src/tree.cpp"

int main(int argc, char *argv[]) {

  // map to store the nodes
  // makes it so there is only one copy of any given state
  std::map<std::string, node<state *> *> map;

  // make the first state
  state *start = new state;

  // create the state tree
  node<state *> * head = new node<state *>(start);

  // add the first node to the map
  std::string start_string = "13571";
  std::transform(start_string.begin(), start_string.end(), start_string.begin(),
                 [](char c) { return c - 48; });
  map[start_string] = head;

  // generate all nodes and edges (recursively)
  give_children(head, map);

  // verify map and tree
  int num_missing = check_missing(map);

  std::cout << num_missing << " Missing nodes (should be 16)" << std::endl;

  int num_invliad = check_relatives(map);

  std::cout << num_invliad << " Invalid nodes" << std::endl;

  // create a set to store all the states that guarentee wins
  std::set<node<state *> *> solved1;
  std::set<node<state *> *> solved2;

  // fill the set
  solve(1, map, solved1);
  solve(2, map, solved2);

  // if a set contains the starting position, then that player can ALWAYS win
  if (solved1.count(head) != 0) {
    std::cout << "P1 can always win!" << std::endl;
  }
  if (solved2.count(head) != 0) {
    std::cout << "P2 can always win!" << std::endl;
  }

  // delete all heap data
  for (auto [key, data] : map) {
    delete data->data;
    delete data;
  }

  return 0;
}
