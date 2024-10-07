#include "PmergeMe.hpp"

#include <algorithm>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Error: invalid arguments" << std::endl;
    std::cout << "usage: ./PmergeMe *numbers*" << std::endl;
    return 1;
  }
  PmergeMe pm;
  pm.sort(argc, argv);
  return 0;
}
