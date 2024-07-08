#pragma once

#include <vector>

template <typename U> struct node {

  node() = default;

  node(const U &in_data) { data = in_data; }

  U data;
  std::vector<node *> parents;
  std::vector<node *> children;

  bool operator==(const node &right) { return (data == right.data); }

  int relatives()
  {
    return parents.size() + children.size();
  }
};

