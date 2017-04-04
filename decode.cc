#include <iostream>
#include "huffman.h"
using namespace std;

const unsigned ALPHABET_SIZE = 128;
unsigned frequencies[ALPHABET_SIZE]{};

int main(int argc, char* argv[]) {
  huffman_tree tree{ALPHABET_SIZE};
  fill_n(frequencies, ALPHABET_SIZE, 1);
  tree.build(frequencies);
  fill_n(frequencies, ALPHABET_SIZE, 0);

  unsigned read = 0, limit = 128;
  char buffer = '\0';

  while (cin.get(buffer)) {
    for (int i = 128; i != 0; i = (i >> 1)) {
      auto chr = tree.decode(buffer & i);
      if (chr != -1) {
        cout << (char) chr;
        frequencies[chr]++;
        read++;
      }
      if (read == limit) {
        limit *= 2;
        tree.build(frequencies);
      }
    }
  }

  return 0;
}
