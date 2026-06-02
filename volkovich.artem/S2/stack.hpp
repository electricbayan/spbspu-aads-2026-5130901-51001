#ifndef STACK_HPP
#define STACK_HPP
#include <stdexcept>

#include "../common/list.hpp"

namespace volkovich {
  template < typename T >
  class Stack {
    List< T > data;

   public:
    bool isEmpty() {
      return data.isEmpty();
    };
    size_t length() {
      return data.size();
    };
    void push(T&& v) {
      data.pushFront(std::move(v));
    };
    void push(const T& v) {
      data.pushFront(v);
    };
    T pop() {
      if (isEmpty()) {
        throw std::logic_error("Stack is empty");
      }
      T d = std::move(data.head->data);
      data.popFront();
      return d;
    }

    T value() {
      return data.head->data;
    }

    void swap(Stack& other) {
      data.swap(other.data);
    }
  };
}

#endif
