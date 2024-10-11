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
#include <unordered_set>
#include <utility>
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
  // FIX move uniq check up the stack
  if (vec.empty() ||
      std::unordered_set<uint32_t>(vec.begin(), vec.end()).size() !=
          vec.size()) {
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
  if (deq.empty() ||
      std::unordered_set<uint32_t>(deq.begin(), deq.end()).size() !=
          deq.size()) {
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

  std::cout << "After : ";
  for (std::vector<uint32_t>::iterator it = vec.begin(); it != vec.end();
       it++) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;
  
  //FIX - out numbers to a file and run some script that ensures they are sorted and no numbers lost and etc

  std::cout << "Time to process a range of " << vec.size()
            << " elements with std::vector : " << duration_vec.count() << "us"
            << std::endl;
  std::cout << "Time to process a range of " << deq.size()
            << " elements with std::deque : " << duration_deq.count() << "us"
            << std::endl;
}

int get_tk(int k) {
  int pow = 1 < (k + 1);
  int sign = ((k & 1) ? (-1) : (1));
  return (pow + sign) / 3;
}

size_t PmergeMe::genereate_next_chunk_len(size_t old_chunk_index) {
  int pow = 1 << (old_chunk_index + 1);
  int sign = ((old_chunk_index & 1) ? (-1) : (1));
  int result = (pow + sign) / 3 + 1;
  // std::cout << "generated chunk_len " << result << " from index " << old_chunk_index << std::endl;
  return result;
}

// FIX
// let's say seq is 4,3;6,5;12,11...
// we have 5 total elements
// so instead of 4,3;6,5
// it should  be 4,3;5
// so when one chunk is finished,
// before creating new chunk we check
// and if the biggest index in this chunk is greater than vec.size()
//  (in our case its 6 when size is only 5)
//  then we should lower the biggest number of the chunk down to size

//returns indices as in the book minus one (the first we move to the beginning in advance)
std::vector<size_t> PmergeMe::generate_insertion_order(size_t size) {
  // std::cout << "Generating insertion order of size " << size << std::endl;
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
  size_t start = 3;
  size_t chunk_len = 2;
  size_t chunk_index = 2;
  while (sequence.size() < size && start < size) {
      // std::cout << "start=" << start << ", chunk_len=" << chunk_len << ", size()=" << sequence.size() << ", size=" << size <<std::endl;
    for (size_t i = start + chunk_len - 1; i >= start; i--) {
      sequence.push_back(i - 1);
      if (sequence.size() > size) {
        // std::cout << "Insertion sequence: ";
        // for (std::vector<size_t>::iterator it = sequence.begin();
        //      it != sequence.end(); it++) {
        //   std::cout << *it << " ";
        // }
        // std::cout << std::endl;
        return sequence;
      }
    }
        // std::cout << "Insertion sequence: ";
        // for (std::vector<size_t>::iterator it = sequence.begin();
        //      it != sequence.end(); it++) {
        //   std::cout << *it << " ";
        // }
        // std::cout << std::endl;
    start += chunk_len;
    chunk_len = genereate_next_chunk_len(chunk_index);
    if (start + chunk_len > size) {
      // std::cout << "REDUCING CHUNK LEN" << "start=" << start << ", chunk_len=" << chunk_len << ", size()=" << sequence.size() << ", size=" << size <<std::endl;
      chunk_len = size - start;
      // std::cout << "reduced chunk_len " << chunk_len << std::endl;
    }
    chunk_index++;
    // std::cout << size << " " << sequence.size() << std::endl;
  }
  // std::cout << "Insertion sequence: ";
  // for (std::vector<size_t>::iterator it = sequence.begin();
  //      it != sequence.end(); it++) {
  //   std::cout << *it << " ";
  // }
  // std::cout << std::endl;
  return sequence;
}

// inserts lower_val into result
// searches bin_search the position
// from result[0] to result[upper_pos] (the latter not included);
// lower_val is not needed
void PmergeMe::bin_insert(std::vector<uint32_t> &result, size_t upper_pos,
                          uint32_t lower_val) {
  // std::cout << "calling bin_insert on UP=" << upper_pos
  //           << ", val= " << lower_val << ", vec: ";
  // for (std::vector<uint32_t>::iterator it = result.begin(); it != result.end();
  //      it++) {
  //   std::cout << *it << " ";
  // }
  // std::cout << std::endl;
  // std::cout << "result befor insert: ";
  // for (std::vector<uint32_t>::iterator it = result.begin(); it != result.end();
  //      it++) {
  //   std::cout << *it << " ";
  // }
  // std::cout << std::endl;

  std::vector<uint32_t>::iterator pos =
      bin_search(result, 0, upper_pos, lower_val);
  if (pos == result.end()) {
    result.push_back(lower_val);
  } else {
    result.insert(pos, lower_val);
  }

  // std::cout << "result after insert: ";
  // for (std::vector<uint32_t>::iterator it = result.begin(); it != result.end();
  //      it++) {
  //   std::cout << *it << " ";
  // }
  // std::cout << std::endl;
}

// should return the element BEFORE which we will insert our value
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

void PmergeMe::merge_insertion_sort(std::vector<uint32_t> &vec) {
  std::cout << "NEW FUNCTION CALLED ON: ";
  // std::cout << "Performing sort on vec: ";
  for (std::vector<uint32_t>::iterator it = vec.begin(); it != vec.end();
       it++) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

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

  // std::cout << "result beforee insert: ";
  // for (std::vector<uint32_t>::iterator it = result.begin(); it != result.end();
  //      it++) {
  //   std::cout << *it << " ";
  // }
  // std::cout << std::endl;

  // std::cout << "Generating insertion order for vector result of size "
  //           << upper.size() << std::endl;
  std::vector<size_t> insertion_order =
      generate_insertion_order(upper.size());
  std::cout << "Generated insertion order: ";
  for (std::vector<size_t>::iterator it = insertion_order.begin(); it != insertion_order.end();
       it++) {
    std::cout << *it << " ";
  }
  std::cout << std::endl; 

  for (size_t i = 0, j = 2; i < insertion_order.size(); i++, j++) {
    size_t &cur_upper_index = insertion_order[i];
    // size_t &cur_upper_index = insertion_order[i];
    if (cur_upper_index >= upper.size()) {
      std::cout << "cur upper index: " << cur_upper_index << ", upper size: " << upper.size() << ", " << std::endl;
      continue;
    }
    // what does i do here?
    bin_insert(result, result.size(),
               lower_map[upper[cur_upper_index]]);
    
    // bin_insert(result, insertion_order[i] + 3,
    //            lower_map[upper[cur_upper_index]]);
  }


  if (is_odd) {
    // std::cout << "Orphan insert: " << orphan
    //           << ", result.size() = " << result.size() << ", result: ";
    // for (std::vector<uint32_t>::iterator it = result.begin();
    //      it != result.end(); it++) {
    //   std::cout << *it << " ";
    // }
    // std::cout << std::endl;

    bin_insert(result, result.size(), orphan);
  }
  (void)orphan;
  vec = result;

  std::cout << "STACK DUMP: ";
  for (std::vector<uint32_t>::iterator it = vec.begin(); it != vec.end();
       it++) {
    std::cout << *it << " ";
  }
  std::cout << std::endl;

  return;
}

void PmergeMe::merge_insertion_sort(std::deque<uint32_t> &deq) {
  (void)deq;
  std::sort(deq.begin(), deq.end());
  return;
}
