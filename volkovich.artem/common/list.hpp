#ifndef LIST_HPP
#define LIST_HPP
#include <list>
#include <utility>
#include "iter.hpp"
#include "item.hpp"
#include <cstddef>
#include <stdexcept>

namespace volkovich
{
  template <class T>
  class List
  {
  private:
    Item<T> fake_node;
    size_t list_len = 0;

  public:
    Item<T> *tail = nullptr;
    Item<T> *head = nullptr;
    List() : fake_node()
    {
      fake_node.next = nullptr;
    };

    ~List()
    {
      clear();
    };

    List(const List &other) : fake_node()
    {
      copy(other);
    };

    List(List &&other) : fake_node()
    {
      swap(other);
    };

    List &operator=(List other)
    {
      swap(other);
      return *this;
    };

    bool isEmpty() const noexcept
    {
      return fake_node.next == nullptr;
    };

    T popFront()
    {
      if (!head)
        throw std::logic_error("Empty list");
      Item<T> *old_head = head;
      fake_node.next = old_head->next;
      head = fake_node.next;
      if (head == nullptr)
      {
        tail = nullptr;
      }
      T deleted_value = old_head->data;
      delete old_head;
      list_len--;
      return deleted_value;
    };

    void popBack()
    {
      if (fake_node.next == nullptr)
      {
        return;
      }

      if (fake_node.next == tail)
      {
        delete tail;
        head = nullptr;
        tail = nullptr;
        fake_node.next = nullptr;
        list_len = 0;
        return;
      }

      Item<T> *prev = &fake_node;
      Item<T> *cur = fake_node.next;
      while (cur->next)
      {
        prev = cur;
        cur = prev->next;
      }
      prev->next = nullptr;
      delete cur;
      tail = prev;
      list_len--;
    };

    void pushFront(T &&v)
    {
      Item<T> *newItem = new Item<T>(std::move(v));
      newItem->next = head;
      fake_node.next = newItem;
      head = newItem;
      if (list_len == 0)
      {
        tail = newItem;
      }
      list_len++;
    };

    void pushFront(const T &v)
    {
      Item<T> *newItem = new Item<T>(v);
      newItem->next = head;
      fake_node.next = newItem;
      head = newItem;
      if (list_len == 0)
      {
        tail = newItem;
      }
      list_len++;
    };

    void pushBack(T &&v)
    {
      Item<T> *newItem = new Item<T>(std::move(v));
      newItem->next = nullptr;
      if (list_len == 0)
      {
        tail = head = newItem;
        fake_node.next = head;
      }
      else
      {
        tail->next = newItem;
        tail = newItem;
      }
      list_len++;
    };

    void pushBack(const T &v)
    {
      Item<T> *newItem = new Item<T>(v);
      newItem->next = nullptr;
      if (list_len == 0)
      {
        tail = head = newItem;
        fake_node.next = head;
      }
      else
      {
        tail->next = newItem;
        tail = newItem;
      }
      list_len++;
    };

    LIter<T> insertAfter(LIter<T> iter, T &&v)
    {
      Item<T> *newItem = new Item<T>(std::move(v));
      newItem->next = iter.item->next;
      iter.item->next = newItem;

      if (iter.item == &fake_node)
      {
        head = newItem;
      }

      if (newItem->next == nullptr)
      {
        tail = newItem;
      }

      list_len++;
      return LIter<T>(newItem);
    }

    LIter<T> insertAfter(LIter<T> iter, const T &v)
    {
      Item<T> *newItem = new Item<T>(v);
      newItem->next = iter.item->next;
      iter.item->next = newItem;

      if (iter.item == &fake_node)
      {
        head = newItem;
      }

      if (newItem->next == nullptr)
      {
        tail = newItem;
      }

      list_len++;
      return LIter<T>(newItem);
    }

    void deleteAfter()
    {
    }

    void clear() noexcept
    {
      while (!isEmpty())
      {
        popFront();
      }
    };

    LIter<T> fakeBegin() noexcept
    {
      return LIter<T>(&fake_node);
    };

    LIter<T> begin() noexcept
    {
      return LIter<T>(fake_node.next);
    };

    LCIter<T> begin() const noexcept
    {
      return LCIter<T>(fake_node.next);
    };

    void swap(List &other) noexcept
    {
      using std::swap;
      swap(head, other.head);
      swap(tail, other.tail);
      swap(list_len, other.list_len);
      fake_node.next = head;
      other.fake_node.next = other.head;
    };

    void copy(const List &other)
    {
      clear();
      LIter<T> elem = fakeBegin();
      list_len = 0;
      for (LCIter<T> other_iter = other.begin(); other_iter != LCIter<T>(nullptr); ++other_iter)
      {
        elem = insertAfter(elem, *other_iter);
        list_len++;
      }
      head = fake_node.next;
      tail = head;
      if (tail)
      {
        while (tail->next)
        {
          tail = tail->next;
        }
      }
    };

    size_t size() noexcept
    {
      return list_len;
    };
  };
}

#endif
