#include <boost/test/unit_test.hpp>

#include "stack.hpp"


BOOST_AUTO_TEST_CASE(stack_empty) {
  volkovich::Stack<int> q;
  BOOST_TEST(q.isEmpty());
}

BOOST_AUTO_TEST_CASE(stack_push)  {
  volkovich::Stack<int> s;
  s.push(2);
  s.push(5);
  s.push(7);
  BOOST_TEST(s.length() == 3);
}

BOOST_AUTO_TEST_CASE(stack_value_and_pop_lifo_order) {
  volkovich::Stack<int> s;
  int first = 2;
  int second = 5;
  int third = 7;
  s.push(first);
  s.push(second);
  s.push(third);

  BOOST_TEST(s.value() == 7);
  BOOST_TEST(s.pop() == 7);
  BOOST_TEST(s.pop() == 5);
  BOOST_TEST(s.pop() == 2);
  BOOST_TEST(s.isEmpty());
}

BOOST_AUTO_TEST_CASE(stack_pop_empty_throws) {
  volkovich::Stack<int> s;
  BOOST_CHECK_THROW(s.pop(), std::logic_error);
}
