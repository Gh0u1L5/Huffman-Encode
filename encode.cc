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
  vector<bool> output;

  while (cin.get(buffer)) {
    auto code = tree.encode(buffer);
    output.insert(output.end(), code.begin(), code.end());
    frequencies[(unsigned) buffer]++;
    if (++read == limit) {
      limit *= 2;
      tree.build(frequencies);
    }
  }

  buffer = '\0';
  for (unsigned i = 0; i < output.size(); i++) {
    buffer = (buffer << 1) | output[i];
    if ((i + 1) % 8 == 0) {
      putchar(buffer);
      buffer = '\0';
    }
  }
  putchar(buffer);

  return 0;
}
