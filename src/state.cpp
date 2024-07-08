#pragma once

#include <algorithm>
#include <string>

// Need to add "bool turn" to mark who's turn it is

class state {
public:
  state() {
    rows.resize(5);

    for (int i = 0; i < 4; ++i) {
      rows[i] = (2 * i) + 1;
    }

    rows[4] = 1; // the player's turn is signified by either a 1 or 2 at the end
                 // of the string
  }

  state(const std::string &in_rows) {
    rows.resize(5);

    for (int i = 0; i < 4; ++i) {
      rows[i] = in_rows[i];
    }

    rows[4] = 1; // the player's turn is signified by either a 1 or 2 at the end
                 // of the string
  }

  bool next_to(const state &other) const {
    return (count_diff(other) == 1 && other.rows[4] != rows[4]);
  }

  bool next_to(const std::string &other) const {
    return (count_diff(other) == 1 && other[4] != rows[4]);
  }

  bool equal_to(const state &other) const { return (count_diff(other) == 0); }

  std::string to_str() {
    std::string retstr = rows;
    std::transform(retstr.begin(), retstr.end(), retstr.begin(),
                   [](char c) { return c + 48; });

    return retstr;
  }

  std::string rows;

private:
  unsigned count_diff(const state &other) const {
    unsigned counter = 0;

    for (int i = 0; i < 4; ++i) {
      if (rows[i] != other.rows[i]) {
        ++counter;
      }
    }

    return counter;
  }

  unsigned count_diff(const std::string &other) const {
    unsigned counter = 0;

    for (int i = 0; i < 4; ++i) {
      if (rows[i] != other[i]) {
        ++counter;
      }
    }

    return counter;
  }
};

bool operator==(const state &left, const state &right) {
  return (left.equal_to(right));
}
