#include "PmergeMe.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <deque>
#include <iostream>
#include <stdint.h>
#include <string>
#include <vector>

PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe &other) { (void)other; }
PmergeMe &PmergeMe::operator=(const PmergeMe &other) {
  (void)other;
  return *this;
}
PmergeMe::~PmergeMe() {}

uint32_t PmergeMe::parse_number(char *c_str) {
  unsigned long long l_result;
  std::string s(c_str);
  l_result = std::stoul(s, NULL, 10);
  // FIX: use sstream instead and ensure that it is empty after >>
  uint32_t result;
  if (l_result > UINT32_MAX) {
    throw std::overflow_error("Positive integer is too big");
  }
  result = static_cast<uint32_t>(l_result);
  return result;
}

void PmergeMe::fill(int argc, char **argv, std::vector<uint32_t> &vec) {
  try {
    std::transform(argv + 1, argv + argc, std::back_inserter(vec),
                   parse_number);
  } catch (std::exception &e) {
    vec.clear();
  }
}

void PmergeMe::fill(int argc, char **argv, std::deque<uint32_t> &deq) {
  try {
    std::transform(argv + 1, argv + argc, std::back_inserter(deq),
                   parse_number);
  } catch (std::exception &e) {
    deq.clear();
  }
}

void PmergeMe::sort_vec(int argc, char **argv) {
  std::chrono::high_resolution_clock::time_point start =
      std::chrono::high_resolution_clock::now();
  PmergeMe::fill(argc, argv, vec);
  if (vec.empty()) {
    return;
  }
  merge_insertion_sort(vec);
  std::chrono::high_resolution_clock::time_point end =
      std::chrono::high_resolution_clock::now();
  duration_vec =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
  PmergeMe::fill(argc, argv, original_vec);
}

void PmergeMe::sort_deq(int argc, char **argv) {
  std::chrono::high_resolution_clock::time_point start =
      std::chrono::high_resolution_clock::now();
  PmergeMe::fill(argc, argv, deq);
  if (deq.empty()) {
    return;
  }
  merge_insertion_sort(deq);
  std::chrono::high_resolution_clock::time_point end =
      std::chrono::high_resolution_clock::now();
  duration_deq =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
}

void PmergeMe::sort(int argc, char **argv) {
  sort_vec(argc, argv);
  if (vec.empty()) {
    std::cout << "Error" << std::endl;
    return;
  }
  sort_deq(argc, argv);
  if (deq.empty()) {
    std::cout << "Error" << std::endl;
    return;
  }
  std::cout << "Before: ";
  for (std::vector<uint32_t>::iterator it = original_vec.begin();
       it != original_vec.end(); it++) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  std::cout << "After: ";
  for (std::vector<uint32_t>::iterator it = vec.begin(); it != vec.end();
       it++) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  std::cout << "Time to process a range of " << vec.size()
            << " elements with std::vector : " << duration_vec.count() << "us"
            << std::endl;
  std::cout << "Time to process a range of " << deq.size()
            << " elements with std::deque : " << duration_deq.count() << "us"
            << std::endl;
}

void PmergeMe::merge_insertion_sort(std::vector<uint32_t> &vec) {
  (void)vec;
  std::sort(vec.begin(), vec.end());
  return;
}

void PmergeMe::merge_insertion_sort(std::deque<uint32_t> &deq) {
  (void)deq;
  std::sort(deq.begin(), deq.end());
  return;
}
