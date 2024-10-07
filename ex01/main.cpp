#include "RPN.hpp"

#include <iostream>
#include <string>

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cout << "Error: invalid arguments" << std::endl;
    std::cout << "usage: ./RPN \"expression in polish notation\"" << std::endl;
    return 1;
  }
  std::string expr(argv[1]);
  return RPN::calculate(expr);
}
