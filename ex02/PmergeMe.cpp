#include "PmergeMe.hpp"

#include <algorithm>
#include <deque>
#include <string>
#include <vector>
#include <iostream>

PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe &other) {}
PmergeMe &PmergeMe::operator=(const PmergeMe &other) {}
PmergeMe::~PmergeMe() {}

std::vector<uint32_t>::iterator PmergeMe::parse_number(char *c_str) {
  unsigned long l_result;
  std::string s(c_str);
  l_result = std::stoul(s, NULL, 10);
  uint32_t result;
  if (l_result > UINT_MAX) {
    throw std::overflow_error("Positive integer is too big");
  }
}

void PmergeMe::fill(int argc, char **argv, std::vector<uint32_t> &vec) {
  try {
    std::transform(argv + 1, argv + argc, std::back_inserter(vec),
                   parse_number);
  } catch (std::exception &e) {
    vec.clear();
  }
}

void PmergeMe::fill(int argc, char **argv, std::vector<uint32_t> &deq) {
  try {
    std::transform(argv + 1, argv + argc, std::back_inserter(deq),
                   parse_number);
  } catch (std::exception &e) {
    deq.clear();
  }
}

void PmergeMe::sort_vec(int argc, char **argv) {
  std::vector<unsigned int> vec;
  PmergeMe::fill(argc, argv, vec);
  if (vec.empty()) {
    std::cout << "Error" << std::endl;
    return;
  }
}

void PmergeMe::sort_deq(int argc, char **argv) {}
