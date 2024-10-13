#include "PmergeMe.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <deque>
#include <iostream>
#include <stdint.h>
#include <string>
#include <unistd.h>
#include <unordered_map>
#include <utility>
#include <vector>
#include <sstream>

PmergeMe::PmergeMe() {}
PmergeMe::PmergeMe(const PmergeMe &other) { (void)other; }
PmergeMe &PmergeMe::operator=(const PmergeMe &other) {
  (void)other;
  return *this;
}
PmergeMe::~PmergeMe() {}

/*          The sorting functions          */

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
  print_result();
}

void PmergeMe::sort_vec(int argc, char **argv) {
  PmergeMe::fill(argc, argv, vec);
  std::chrono::high_resolution_clock::time_point start =
      std::chrono::high_resolution_clock::now();
  if (vec.empty()) {
    vec.clear();
    return;
  }
  merge_insertion_sort(vec);
  std::chrono::high_resolution_clock::time_point end =
      std::chrono::high_resolution_clock::now();
  duration_vec = get_time_str(end, start);
  return;
}

void PmergeMe::sort_deq(int argc, char **argv) {
  PmergeMe::fill(argc, argv, deq);
  std::chrono::high_resolution_clock::time_point start =
      std::chrono::high_resolution_clock::now();
  if (deq.empty()) {
    deq.clear();
    return;
  }
  merge_insertion_sort(deq);
  std::chrono::high_resolution_clock::time_point end =
      std::chrono::high_resolution_clock::now();
  duration_deq = get_time_str(end, start);
  return;
}

void PmergeMe::merge_insertion_sort(std::vector<uint32_t> &vec) {
  if (vec.size() < 2) {
    return;
  }
  if (vec.size() == 2) {
    if (vec[0] > vec[1]) {
      std::swap(vec[0], vec[1]);
    }
    return;
  }
  uint32_t orphan;
  bool is_odd = vec.size() % 2 == 1;
  if (is_odd) {
    orphan = vec[vec.size() - 1];
    vec.pop_back();
  }
  std::vector<uint32_t> upper;
  std::unordered_map<uint32_t, uint32_t> lower_map;
  for (size_t i = 0; i < vec.size() - 1; i++, i++) {
    std::pair<uint32_t, uint32_t> min_max = std::minmax(vec[i], vec[i + 1]);
    upper.push_back(min_max.second);
    lower_map[min_max.second] = min_max.first;
  }
  merge_insertion_sort(upper);
  std::vector<uint32_t> result(upper);
  result.insert(result.begin(), lower_map[upper[0]]);
  lower_map.erase(upper[0]);
  std::vector<size_t> insertion_order = generate_insertion_order(upper.size());
  for (size_t i = 0, j = 2; i < insertion_order.size(); i++, j++) {
    size_t &cur_upper_index = insertion_order[i];
    if (cur_upper_index >= upper.size()) {
      continue;
    }
    bin_insert(result, cur_upper_index + j, lower_map[upper[cur_upper_index]]);
  }
  if (is_odd) {
    bin_insert(result, result.size() + 1, orphan);
  }
  (void)orphan;
  vec = result;
  return;
}

void PmergeMe::merge_insertion_sort(std::deque<uint32_t> &deq) {
  if (deq.size() < 2) {
    return;
  }
  if (deq.size() == 2) {
    if (deq[0] > deq[1]) {
      std::swap(deq[0], deq[1]);
    }
    return;
  }
  uint32_t orphan;
  bool is_odd = deq.size() % 2 == 1;
  if (is_odd) {
    orphan = deq[deq.size() - 1];
    deq.pop_back();
  }
  std::deque<uint32_t> upper;
  std::unordered_map<uint32_t, uint32_t> lower_map;
  for (size_t i = 0; i < deq.size() - 1; i++, i++) {
    std::pair<uint32_t, uint32_t> min_max = std::minmax(deq[i], deq[i + 1]);
    upper.push_back(min_max.second);
    lower_map[min_max.second] = min_max.first;
  }
  merge_insertion_sort(upper);
  std::deque<uint32_t> result(upper);
  result.insert(result.begin(), lower_map[upper[0]]);
  lower_map.erase(upper[0]);
  std::vector<size_t> insertion_order = generate_insertion_order(upper.size());
  for (size_t i = 0, j = 2; i < insertion_order.size(); i++, j++) {
    size_t &cur_upper_index = insertion_order[i];
    if (cur_upper_index >= upper.size()) {
      continue;
    }
    bin_insert(result, cur_upper_index + j, lower_map[upper[cur_upper_index]]);
  }
  if (is_odd) {
    bin_insert(result, result.size() + 1, orphan);
  }
  (void)orphan;
  deq = result;
  return;
}

/*          The utility functions          */

void PmergeMe::print_result() {
  std::cout << "Before: ";
  for (std::vector<uint32_t>::iterator it = vec.begin(); it != vec.end();
       it++) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  std::cout << "After : ";
  for (std::vector<uint32_t>::iterator it = vec.begin(); it != vec.end();
       it++) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
  std::cout << "Time to process a range of " << vec.size()
            << " elements with std::vector : " << duration_vec << std::endl;
  std::cout << "Time to process a range of " << deq.size()
            << " elements with std::deque : " << duration_deq << std::endl;
}

uint32_t PmergeMe::parse_number(char *c_str) {
  unsigned long long l_result;
  std::string s(c_str);
  l_result = std::stoul(s, NULL, 10);
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
    std::cout << "Can't parse the input" << std::endl;
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

std::string
PmergeMe::get_time_str(std::chrono::high_resolution_clock::time_point end,
                       std::chrono::high_resolution_clock::time_point start) {
  std::chrono::nanoseconds duration_ns =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);
  std::ostringstream ss;
  long long total_nanoseconds = duration_ns.count();
  long long seconds = total_nanoseconds / 1'000'000'000;
  total_nanoseconds %= 1'000'000'000;
  long long milliseconds = total_nanoseconds / 1'000'000;
  total_nanoseconds %= 1'000'000;
  long long microseconds = total_nanoseconds / 1'000;
  long long nanoseconds_remaining = total_nanoseconds % 1'000;
  if (seconds > 0) {
    ss << seconds << " seconds ";
  }
  if (milliseconds > 0 || seconds > 0) {
    ss << milliseconds << " milliseconds ";
  }
  if (microseconds > 0 || milliseconds > 0 || seconds > 0) {
    ss << microseconds << " microseconds ";
  }
  if (nanoseconds_remaining > 0 || microseconds > 0 || milliseconds > 0 ||
      seconds > 0) {
    ss << nanoseconds_remaining << " nanoseconds";
  }
  return ss.str();
}

size_t PmergeMe::genereate_next_chunk_len(size_t old_chunk_index) {
  int pow = 1 << (old_chunk_index + 1);
  int sign = ((old_chunk_index & 1) ? (-1) : (1));
  int result = (pow + sign) / 3;
  return result;
}

std::vector<size_t> PmergeMe::generate_insertion_order(size_t size) {
  std::vector<size_t> sequence;
  if (size == 1) {
    sequence.push_back(1);
    return sequence;
  }
  if (size == 2) {
    sequence.push_back(2);
    sequence.push_back(1);
    return sequence;
  }
  if (size == 3) {
    sequence.push_back(2);
    sequence.push_back(1);
    sequence.push_back(3);
    return sequence;
  }
  if (size == 4) {
    sequence.push_back(2);
    sequence.push_back(1);
    sequence.push_back(4);
    sequence.push_back(3);
    return sequence;
  }

  size_t chunk_start_pos = 0;
  size_t chunk_len = 2;
  size_t chunk_index = 2;
  size_t cur_ai;
  while (sequence.size() < size) {
    cur_ai = chunk_start_pos + chunk_len;
    chunk_start_pos += chunk_len;
    while (chunk_len > 0) {
      if (cur_ai <= size) {
        sequence.push_back(cur_ai);
      }
      chunk_len--;
      cur_ai--;
    }
    chunk_len = genereate_next_chunk_len(chunk_index);
    chunk_index++;
  }
  return sequence;
}

void PmergeMe::bin_insert(std::vector<uint32_t> &result, size_t upper_pos,
                          uint32_t lower_val) {
  std::vector<uint32_t>::iterator pos =
      bin_search(result, 0, upper_pos, lower_val);
  if (pos == result.end()) {
    result.push_back(lower_val);
  } else {
    result.insert(pos, lower_val);
  }
}

void PmergeMe::bin_insert(std::deque<uint32_t> &result, size_t upper_pos,
                          uint32_t lower_val) {
  std::deque<uint32_t>::iterator pos =
      bin_search(result, 0, upper_pos, lower_val);
  if (pos == result.end()) {
    result.push_back(lower_val);
  } else {
    result.insert(pos, lower_val);
  }
}

std::vector<uint32_t>::iterator PmergeMe::bin_search(std::vector<uint32_t> &vec,
                                                     size_t begin, size_t end,
                                                     uint32_t value) {
  (void)begin;
  std::vector<uint32_t>::iterator it = vec.begin() + begin;
  std::vector<uint32_t>::iterator ite = vec.begin() + end - 1;
  std::vector<uint32_t>::iterator res_it;
  res_it = std::lower_bound(it, ite, value);
  return res_it;
}

std::deque<uint32_t>::iterator PmergeMe::bin_search(std::deque<uint32_t> &deq,
                                                    size_t begin, size_t end,
                                                    uint32_t value) {
  (void)begin;
  std::deque<uint32_t>::iterator it = deq.begin() + begin;
  std::deque<uint32_t>::iterator ite = deq.begin() + end - 1;
  std::deque<uint32_t>::iterator res_it;
  res_it = std::lower_bound(it, ite, value);
  return res_it;
}
