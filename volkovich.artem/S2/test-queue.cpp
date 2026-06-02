
#include <boost/test/unit_test.hpp>

#include "queue.hpp"


BOOST_AUTO_TEST_CASE(queue_empty) {
  volkovich::Queue<int> q;
  BOOST_TEST(q.isEmpty());
}

BOOST_AUTO_TEST_CASE(queue_push)  {
  volkovich::Queue<int> q;
  q.push(2);
  q.push(5);
  q.push(7);
  BOOST_TEST(q.length() == 3);
}

BOOST_AUTO_TEST_CASE(queue_pop) {
  volkovich::Queue<int> q;
  q.push(1);
  q.push(2);
  q.push(3);
  int popped = q.pop();

}

// BOOST_AUTO_TEST_CASE()
