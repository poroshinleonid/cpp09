#ifndef RPN_HPP
#define RPN_HPP

#include <stack>

typedef long long ll;

class RPN {
private:
  RPN();
  RPN(const RPN &other);
  RPN &operator=(const RPN &other);
  ~RPN();

public:
  static int calculate(const std::string &expr);

private:
  static int process_token(std::stack<ll> &stack_, const std::string &token);

private:
  static int substract(std::stack<ll> &stack_);
  static int add(std::stack<ll> &stack_);
  static int multiply(std::stack<ll> &stack_);
  static int divide(std::stack<ll> &stack_);
};

#endif // RPN_HPP
