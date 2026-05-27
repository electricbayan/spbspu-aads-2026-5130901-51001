#include "queue.hpp"
#include <string>
#include <iostream>

typedef long long ll;


namespace volkovich {
  Queue<std::string> createInfix(const std::string& input) {
    Queue<std::string> infix;
    size_t start = 0;
    for (size_t i = 0; i<= input.size() ; i++) {
      if (i == input.size() || input[i] == ' ') {
        infix.push(input.substr(start, i - start));
        start=i+1;
      }
    }
    return infix;
  }
  Queue<std::string> createPostfix(const std::string& input) {
    Queue<std::string> infix = createInfix(input);

  }
  ll calculate(const std::string& input) {
    Queue<std::string> postfix_queue = createPostfix(input);
  };
}
