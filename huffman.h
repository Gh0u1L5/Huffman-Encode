#ifndef __HUFFMAN_H__
#define __HUFFMAN_H__

#include <queue>
#include <vector>
#include <string>

class huffman_node {
  friend class huffman_tree;
  unsigned chr;
  huffman_node *left, *right;

public:
  huffman_node(unsigned chr): chr{chr}, left{nullptr}, right{nullptr} {}
  huffman_node(huffman_node *left, huffman_node *right): chr{0}, left{left}, right{right} {}
  ~huffman_node() { delete left; delete right; }
  bool isLeaf() const { return !left && !right; }
  void build_dict(std::vector<bool> *dictionary, std::vector<bool> &code) {
    if (this->isLeaf()) {
      dictionary[chr] = code;
    } else {
      code.push_back(false);
      left->build_dict(dictionary, code);
      code.pop_back();
      code.push_back(true);
      right->build_dict(dictionary, code);
      code.pop_back();
    }
  }
};

class huffman_record {
  friend class huffman_tree;
  unsigned weight;
  huffman_node *node;

public:
  huffman_record(unsigned weight, huffman_node *node): weight{weight}, node{node} {}
  bool operator<(const huffman_record &other) const { return this->weight > other.weight; }
  bool operator>(const huffman_record &other) const { return this->weight < other.weight; }
};

class huffman_tree {
  const unsigned size;
  unsigned WPL;
  huffman_node *root;
  std::vector<bool> *dictionary;

  mutable huffman_node *decode_ptr;

public:
  huffman_tree(const unsigned size): size{size}, WPL{0}, root{nullptr} {
    dictionary = new std::vector<bool>[size];
  }
  ~huffman_tree() { delete root; delete [] dictionary; }
  void clean() { WPL = 0; delete root; }
  int getWPL() const { return WPL; }
  std::vector<bool> *getDictionary() const { return dictionary; }

  void build(const unsigned *frequencies) {
    this->clean();
    std::priority_queue<huffman_record> queue;
    for (unsigned i = 0; i < size; i++) {
      queue.push(huffman_record{frequencies[i], new huffman_node{i}});
    }
    while (queue.size() > 1) {
      huffman_record left = queue.top(); queue.pop();
      huffman_record right = queue.top(); queue.pop();
      WPL += left.weight + right.weight;
      queue.push(huffman_record{left.weight + right.weight, new huffman_node{left.node, right.node}});
    }
    root = decode_ptr = queue.top().node;
    std::vector<bool> buffer;
    root->build_dict(dictionary, buffer);
  }

  std::vector<bool> &encode(char chr) const {
    return dictionary[(unsigned) chr];
  }

  int decode(bool bit) const {
    int result = -1;
    decode_ptr = !bit ? decode_ptr->left : decode_ptr->right;
    if (decode_ptr->isLeaf()) {
      result = decode_ptr->chr;
      decode_ptr = root;
    }
    return result;
  }
};

#endif
