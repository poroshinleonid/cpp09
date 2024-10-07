#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <deque>
#include <vector>

class PmergeMe {
public:
  PmergeMe();
  PmergeMe(const PmergeMe &other);
  PmergeMe &operator=(const PmergeMe &other);
  ~PmergeMe();
public:
  void sort_vec(int argc, char **argv);
  void sort_deq(int argc, char **argv);

private:
  void fill(int argc, char **argv, std::vector<uint32_t> &vec);
  void fill(int argc, char **argv, std::deque<uint32_t> &deq);
  static uint32_t parse_number(char *c_str);
  void merge_insertion_sort(std::vector<uint32_t> vec);
  void merge_insertion_sort(std::deque<uint32_t> deq);

private:
  std::vector<uint32_t> vec;
  std::deque<uint32_t> deq;
};

#endif // PMERGEME_HPP
