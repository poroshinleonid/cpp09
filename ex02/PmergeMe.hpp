#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <chrono>
#include <cstdint>
#include <deque>
#include <string>
#include <vector>

class PmergeMe {
public:
  PmergeMe();
  PmergeMe(const PmergeMe &other);
  PmergeMe &operator=(const PmergeMe &other);
  ~PmergeMe();

public:
  bool are_uniq(int argc, char **argv);
  void sort(int argc, char **argv);
  void sort_vec(int argc, char **argv);
  void sort_deq(int argc, char **argv);

private:
  static uint32_t parse_number(char *c_str);
  void merge_insertion_sort(std::vector<uint32_t> &vec);
  void merge_insertion_sort(std::deque<uint32_t> &deq);

public:
  std::vector<size_t> generate_insertion_order(size_t size);

private:
  void fill(int argc, char **argv, std::vector<uint32_t> &vec);
  void fill(int argc, char **argv, std::deque<uint32_t> &deq);
  void print_result();
  size_t genereate_next_chunk_len(size_t old_chunk_index);
  void bin_insert(std::vector<uint32_t> &result, size_t upper_pos,
                  uint32_t lower_val);
  void bin_insert(std::deque<uint32_t> &result, size_t upper_pos,
                  uint32_t lower_val);

  std::vector<uint32_t>::iterator bin_search(std::vector<uint32_t> &vec,
                                             size_t begin, size_t end,
                                             uint32_t value);
  std::deque<uint32_t>::iterator bin_search(std::deque<uint32_t> &deq,
                                            size_t begin, size_t end,
                                            uint32_t value);

  std::string
  get_time_str(std::chrono::high_resolution_clock::time_point end,
               std::chrono::high_resolution_clock::time_point start);

private:
  std::vector<uint32_t> vec;
  std::deque<uint32_t> deq;
  std::string duration_deq;
  std::string duration_vec;
  std::vector<uint32_t> original_vec;
};

#endif // PMERGEME_HPP
