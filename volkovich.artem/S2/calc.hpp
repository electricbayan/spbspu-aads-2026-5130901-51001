#ifndef CALC_HPP
#define CALC_HPP
#include <iostream>
#include <string>

#include "queue.hpp"
#include "stack.hpp"

typedef long long ll;

namespace volkovich {

  size_t getPriority(const std::string& s) {
    if (s == "^") {
      return 3;
    }
    if (s == "*" || s == "/" || s == "%") {
      return 2;
    }
    if (s == "+" || s == "-") {
      return 1;
    } else {
      return 0;
    }
  }

  bool isNumber(const std::string& s) {
    size_t i=0;
    if (s[i] == '-') {
      if (s.size() == 1) {
        return false;
      }
      i++;
    }
    for (;i<s.size();i++) {
      if (!std::isdigit(static_cast<unsigned char>(s[i]))) {
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
      } else if (token == "+" || token == "-" || token == "*" || token == "/" || token == "^" || token == "%"){
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
  ll calculate(const std::string& input) {
    Queue< std::string > postfix_queue = createPostfix(input);
    ll res{};
    size_t len = postfix_queue.length();
    for (size_t i =0;i<len;i++) {
      std::cout<<postfix_queue.pop();
    }

    std::cout<<'\n';
    return res;
  };
}

#endif
