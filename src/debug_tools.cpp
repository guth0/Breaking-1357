#pragma once

#include <iostream>
#include <map>
#include <set>
#include <string>

#include "state.cpp"
#include "tree.cpp"

// returns the number of missing states, should be 16 (since there are 16 impossible states)
[[nodiscard]] int
check_missing(const std::map<std::string, node<state *> *> &map) {
  std::string tester;
  tester.resize(5);

  // I know this is gross but it is just for debugging to no need for max
  // efficiency/readablility

  int counter = 0;

  for (int player = 1; player <= 2; ++player) {
    tester[4] = player;
    for (int row1 = 0; row1 <= 1; ++row1) {
      tester[0] = row1;
      for (int row2 = 0; row2 <= 3; ++row2) {
        tester[1] = row2;
        for (int row3 = 0; row3 <= 5; ++row3) {
          tester[2] = row3;
          for (int row4 = 0; row4 <= 7; ++row4) {
            tester[3] = row4;
            if (map.count(tester) == 0) {
              /*
              std::string inverse;
              inverse.resize(4);

              for (int i = 0; i < 5; ++i) {
                std::cout << (int)(tester[i]);
                if (i < 4) {
                  inverse[i] = (2 * i + 1) - tester[i];
                }
              }

              std::cout << " -- ";

              for (int i = 0; i < 4; ++i) {
                std::cout << (int)(inverse[i]);
              }
              std::cout << std::endl;
              */

              ++counter;
            }
          }
        }
      }
    }
  }

  return counter;
}

// returns the number of invalid nodes
[[nodiscard]] int
check_relatives(const std::map<std::string, node<state *> *> &map) {
  std::string start_string = "1357";
  std::transform(start_string.begin(), start_string.end(), start_string.begin(),
                 [](char c) { return c - 48; });

  // There are 16 impossible states.
  // They are the ones that only have 1 distict number of moves to get to it.
  // These are moves that have only 1 taken from any/all of the rows.
  // These generate impossible moves because if a state (just the rows) requires
  // exactly 3 moves to get to, then it cannot end with a "1" because player 1
  // makes every odd move.
  //
  // Below, there is something called "inverse", this is a
  // string that represents how many sticks have been taken from each row.
  // "0012"'s inverse is "1345" Since the impossible states can only have 0 or 1
  // taken from any row, the inverses of all of them also match up to the range
  // [0, 15] represented in base 2: 0000, 0001, 0010, ... , 1111

  // generate an array of all the impossible states
  std::vector<std::string> impossibles;
  impossibles.reserve(16);

  std::string str = "0000X"; // Initalize as length 5 string

  for (int row1 = 0; row1 < 2; ++row1) {
    str[0] = start_string[0] - row1;
    for (int row2 = 0; row2 < 2; ++row2) {
      str[1] = start_string[1] - row2;
      for (int row3 = 0; row3 < 2; ++row3) {
        str[2] = start_string[2] - row3;
        for (int row4 = 0; row4 < 2; ++row4) {
          str[3] = start_string[3] - row4;

          str[4] = ((row1 + row2 + row3 + row4) % 2 == 0) ? 2 : 1;

          impossibles.push_back(str);
        }
      }
    }
  }

  int counter = 0;

  for (auto [key, node] : map) {
    if (node->relatives() != 16) {

      int difference = 16 - node->relatives();

      for (std::string inp : impossibles) {
        if (node->data->next_to(inp)) {
          --difference;
        }
      }

      if (difference != 0) {
        /*
        std::string str = node->data->to_str();
        std::string inverse = "0000"; // initialize as a length 4 string

        for (int i = 0; i < 4; ++i) {
          inverse[i] = (48 + (2 * i + 1)) - node->data->rows[i];
        }

        // A node is allowed to have less than 16 relatives if one of its
        expected
        // parents is one of the 16 impossible states are the ones with inverses
        // that contain only 1s or 0s

        std::cout << str << " : " << node->relatives() << " :: " << inverse
          << " ::: " << node->parents.size() << " + "
          << node->children.size() << std::endl;
        */

        counter += 1;
      }
    }
  }
  return (counter);
}

[[nodiscard]] int check_crossover(std::set<node<state *> *> set1,
                                  std::set<node<state *> *> set2) {
  int counter = 0;

  for (node<state *> *sn : set1) {
    if (set2.count(sn) != 0) {
      // printf("Crossover @ %p\n", sn);
      counter += 1;
    }
  }
  return counter;
}
