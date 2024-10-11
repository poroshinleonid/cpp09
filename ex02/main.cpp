#include "PmergeMe.hpp"

#include <algorithm>
#include <deque>
#include <iostream>
#include <string>
#include <vector>

int main(int argc, char **argv) {
  // for (int k = 0; k < 10; k++) {
  //   int pow = 1 << (k + 1);
  //   int sign;
  //   if (k & 1) {
  //     sign = -1;
  //   } else {
  //     sign = 1;
  //   }
  //   // int sign = ((k & 1) ? (-1) : (1));
  //   std::cout << (pow + sign) / 3 << " ";
  // }
  // std::cout << std::endl;

  // (void)argc;
  // (void)argv;
  // PmergeMe pm;
  // std::vector<size_t>  v = pm.generate_insertion_order(15);
  // std::cout << std::endl;
  // std::cout << std::endl;

  // for(size_t i = 0; i < v.size(); i++) {
  //   std::cout << v[i] << " ";
  // }
  // std::cout << std::endl;
  
  if (argc < 2) {
    std::cout << "Error: invalid arguments" << std::endl;
    std::cout << "usage: ./PmergeMe *numbers*" << std::endl;
    return 1;
  }
  PmergeMe pm;
  pm.sort(argc, argv);
  return 0;
}
