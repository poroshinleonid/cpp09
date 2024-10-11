#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <chrono>
#include <cstdint>
#include <deque>
#include <vector>

class PmergeMe {
public:
  PmergeMe();
  PmergeMe(const PmergeMe &other);
  PmergeMe &operator=(const PmergeMe &other);
  ~PmergeMe();

public:
  void sort(int argc, char **argv);
  void sort_vec(int argc, char **argv);
  void sort_deq(int argc, char **argv);

private:
  void fill(int argc, char **argv, std::vector<uint32_t> &vec);
  void fill(int argc, char **argv, std::deque<uint32_t> &deq);
  static uint32_t parse_number(char *c_str);
  void merge_insertion_sort(std::vector<uint32_t> &vec);
  void merge_insertion_sort(std::deque<uint32_t> &deq);
public:
  std::vector<size_t> generate_insertion_order(size_t size);
private:
  size_t genereate_next_chunk_len(size_t old_chunk_index);
  void bin_insert(std::vector<uint32_t> &result, size_t upper_pos,
                  uint32_t lower_val);
  std::vector<uint32_t>::iterator bin_search(std::vector<uint32_t> &vec,
                                             size_t begin, size_t end,
                                             uint32_t value);

private:
  std::vector<uint32_t> vec;
  std::vector<uint32_t> original_vec;
  std::deque<uint32_t> deq;
  std::chrono::nanoseconds duration_vec;
  std::chrono::nanoseconds duration_deq;
};

#endif // PMERGEME_HPP
