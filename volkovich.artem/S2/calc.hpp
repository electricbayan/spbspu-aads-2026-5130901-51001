#ifndef CALC_HPP
#define CALC_HPP
#include <iostream>
#include <limits>
#include <string>

#include "queue.hpp"
#include "stack.hpp"

typedef long long ll;
constexpr ll min = std::numeric_limits< ll >::min();
constexpr ll max = std::numeric_limits< ll >::max();

namespace volkovich {

  size_t getPriority(const std::string& s) {
    if (s == "*" || s == "/" || s == "%") {
      return 2;
    }
    if (s == "+" || s == "-") {
      return 1;
    } else {
      return 0;
    }
  }

  ll mod(ll v) {
    if (v > 0) {
      return v;
    }
    return static_cast<ll>(-(v+1))+1;
  }

  bool isNumber(const std::string& s) {
    size_t i = 0;
    if (s[i] == '-') {
      if (s.size() == 1) {
        return false;
      }
      i++;
    }
    for (; i < s.size(); i++) {
      if (!std::isdigit(static_cast< unsigned char >(s[i]))) {
        return false;
      }
    }
    return true;
  }

  Queue< std::string > createInfix(const std::string& input) {
    Queue< std::string > infix;
    size_t start = 0;
    for (size_t i = 0; i <= input.size(); i++) {
      if (i == input.size() || input[i] == ' ') {
        infix.push(input.substr(start, i - start));
        start = i + 1;
      }
    }
    return infix;
  }
  Queue< std::string > createPostfix(const std::string& input) {
    Queue< std::string > infix = createInfix(input);

    Queue< std::string > out;
    Stack< std::string > operators;
    while (!infix.isEmpty()) {
      std::string token = infix.pop();
      if (token.empty()) {
        continue;
      }
      if (isNumber(token)) {
        out.push(token);
      } else if (token == "(") {
        operators.push(token);
      } else if (token == ")") {
        while (!operators.isEmpty() && operators.value() != "(") {
          out.push(operators.value());
          operators.pop();
        }
        if (!operators.isEmpty()) {
          operators.pop();
        }
      } else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^" ||
                 token == "%") {
        while (!operators.isEmpty() && operators.value() != "(" &&
               ((token == "^" && getPriority(operators.value()) > getPriority(token)) ||
                   (token != "^" && getPriority(operators.value()) >= getPriority(token)))) {
          out.push(operators.value());
          operators.pop();
        }
        operators.push(token);
      }
    }
    while (!operators.isEmpty()) {
      out.push(operators.value());

      operators.pop();
    }
    return out;
  }

  ll calculatePostfix(Queue< std::string > postfix) {
    Stack< ll > numbers;
    while (!postfix.isEmpty()) {
      std::string token = postfix.pop();
      if (isNumber(token)) {
        numbers.push(std::stoll(token));
      } else {
        ll rhs = numbers.pop();
        ll lhs = numbers.pop();
        if (token == "+") {
          if ((rhs > 0 && lhs > max - rhs) || (rhs < 0 && lhs < min - rhs)) {
            throw std::overflow_error("Out of range numbers");
          }
          numbers.push(lhs + rhs);
        } else if (token == "-") {
          if ((rhs > 0 && lhs < min + rhs )|| (rhs < 0 && lhs > max + rhs)) {
            throw std::overflow_error("Out of range numbers");
          }
          numbers.push(lhs - rhs);
        } else if (token == "/") {
          if (rhs == 0) {
            throw std::logic_error("Division by zero");
          }
          if (lhs == min && rhs == -1) {
            throw std::overflow_error("Out of range numbers");
          }
          numbers.push(lhs / rhs);
        } else if (token == "*") {
          if ((lhs == -1 && rhs == min) || (rhs == -1 && lhs == min)) {
            throw std::overflow_error("Out of range numbers");
          }
          if (lhs > 0) {
            if ((rhs > 0 && lhs > max / rhs) || (rhs < 0 && rhs < min / lhs)) {
              throw std::overflow_error("Out of range numbers");
            }
          } else {
            if ((rhs > 0 && lhs < min / rhs) || (rhs < 0 && lhs < max / rhs)) {
              throw std::overflow_error("Out of range numbers");
            }
          }
          numbers.push(lhs * rhs);
        } else if (token == "%") {
          if (rhs == 0) {
            throw std::logic_error("Division by zero");
          }
          if (lhs == min && rhs == -1) {
            throw std::overflow_error("Out of range numbers");
          }
          ll res = lhs % rhs;
          if (res >= 0) {
            numbers.push(res);
          } else {
            numbers.push(mod(rhs) - mod(res));
          }

        }
      }
    }
    return numbers.pop();
  }
  ll calculate(const std::string& input) {
    Queue< std::string > postfix_queue = createPostfix(input);
    return calculatePostfix(postfix_queue);
  };
}

#endif
