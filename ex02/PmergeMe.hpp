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
  // std::vector<uint32_t>::iterator parse_number(char *c_str);
  static uint32_t parse_number(char *c_str);

private:
  std::vector<uint32_t> vec;
  std::deque<uint32_t> deq;
};

#endif // PMERGEME_HPP
