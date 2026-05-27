#ifndef QUEUE_HPP
#define QUEUE_HPP

#include "../common/list.hpp"

namespace volkovich
{
  template <typename T>
  class Queue
  {
    List<T> data;

  public:
    bool isEmpty() const noexcept
    {
      return data.isEmpty();
    };

    size_t length() const noexcept
    {
      return data.size();
    };

    void push(T &&v)
    {
      data.pushFront(std::move(v));
    };

    void push(const T &v)
    {
      data.pushFront(v);
    };

    T pop() {
      if (isEmpty) {
        throw std::logic_error("Queue is empty");
      }
      return data.popFront();
    };

    void swap(Queue& other) {
      data.swap(other.data);
    }
    T drop();
  };
}
#endif
