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

template <typename T> struct tree {
  node<T> *head = nullptr;

  tree(const T &in_data) {
    head = new node<T>;
    head->data = in_data;
  }

  //  ~tree() { clear_start(head); }

  // This does not work
  // Since there are "loops" in the tree, this does not work
  void clear_start(node<T> *node) {

    if (!node->children.empty()) {

      for (auto child : node->children) {
        clear_start(child);
      }
    }

    delete node;
  }
};
