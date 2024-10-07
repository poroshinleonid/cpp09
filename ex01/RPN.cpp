#include "RPN.hpp"

#include <stack>

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

int RPN::substract(std::stack<ll> &stack_) {
  if (stack_.size() < 2) {
    return EXIT_FAILURE;
  }
  ll operand2 = stack_.top();
  stack_.pop();
  ll operand1 = stack_.top();
  stack_.pop();
  ll result = operand1 - operand2;
  stack_.push(result);
  return EXIT_SUCCESS;
}

int RPN::add(std::stack<ll> &stack_) {
  if (stack_.size() < 2) {
    return EXIT_FAILURE;
  }
  ll operand2 = stack_.top();
  stack_.pop();
  ll operand1 = stack_.top();
  stack_.pop();
  ll result = operand1 + operand2;
  stack_.push(result);
  return EXIT_SUCCESS;
}

int RPN::multiply(std::stack<ll> &stack_) {
  if (stack_.size() < 2) {
    return EXIT_FAILURE;
  }
  ll operand2 = stack_.top();
  stack_.pop();
  ll operand1 = stack_.top();
  stack_.pop();
  ll result = operand1 * operand2;
  stack_.push(result);
  return EXIT_SUCCESS;
}

int RPN::divide(std::stack<ll> &stack_) {
  if (stack_.size() < 2) {
    return EXIT_FAILURE;
  }
  ll operand2 = stack_.top();
  stack_.pop();
  ll operand1 = stack_.top();
  stack_.pop();
  if (operand2 == 0) {
    return EXIT_FAILURE;
  }
  ll result = operand1 / operand2;
  stack_.push(result);
  return EXIT_SUCCESS;
}

int RPN::process_token(std::stack<ll> &stack_, const std::string &token) {
  if (token.size() != 1) {
    return EXIT_FAILURE;
  }
  if (isdigit(token[0])) {
    stack_.push(token[0] - '0');
    return EXIT_SUCCESS;
  }
  switch (token[0]) {
  case '/':
    return divide(stack_);
  case '*':
    return multiply(stack_);
  case '+':
    return add(stack_);
  case '-':
    return substract(stack_);
  default:
    return EXIT_FAILURE;
  }
}

int RPN::calculate(const std::string &expr) {
  std::istringstream iss(expr);
  std::string token;
  std::stack<ll> stack_;
  while (iss >> token) {
    if (process_token(stack_, token) == EXIT_FAILURE) {
      std::cout << "Error" << std::endl;
      return EXIT_FAILURE;
    }
  }
  if (stack_.size() != 1) {
    std::cout << "Error" << std::endl;
    return EXIT_FAILURE;
  }
  long long result = stack_.top();
  std::cout << result << std::endl;
  return EXIT_SUCCESS;
}
