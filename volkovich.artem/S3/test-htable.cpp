#include <boost/test/unit_test.hpp>

#include <set>
#include <string>

#include "../common/htable.hpp"

using Table = volkovich::HashTable<std::string, int, volkovich::SipHash, std::equal_to<std::string>>;

static size_t countElements(const Table& ht) {
  size_t n = 0;
  for (auto it = ht.begin(); it != ht.end(); ++it) {
    ++n;
  }
  return n;
}

static std::set<std::string> collectKeys(const Table& ht) {
  std::set<std::string> keys;
  for (auto it = ht.begin(); it != ht.end(); ++it) {
    keys.insert(it->key);
  }
  return keys;
}

BOOST_AUTO_TEST_CASE(empty_table) {
  Table ht(volkovich::SipHash{});
  BOOST_TEST(ht.isEmpty());
  BOOST_TEST(ht.table_size() == 0u);
  BOOST_TEST(ht.find("missing") == nullptr);
  BOOST_TEST(!ht.has("missing"));
  BOOST_CHECK(ht.begin() == ht.end());
}

BOOST_AUTO_TEST_CASE(add_and_find) {
  Table ht(volkovich::SipHash{});
  BOOST_TEST(ht.add("one", 1));
  BOOST_TEST(ht.add("two", 2));
  BOOST_TEST(ht.add("three", 3));
  BOOST_TEST(!ht.isEmpty());
  BOOST_TEST(ht.table_size() == 3u);
  BOOST_TEST(ht.has("one"));
  BOOST_TEST(ht.has("two"));
  BOOST_TEST(ht.has("three"));
  BOOST_TEST(*ht.find("one") == 1);
  BOOST_TEST(*ht.find("two") == 2);
  BOOST_TEST(*ht.find("three") == 3);
  BOOST_TEST(ht.find("missing") == nullptr);
}

BOOST_AUTO_TEST_CASE(add_duplicate_returns_false) {
  Table ht(volkovich::SipHash{});
  BOOST_TEST(ht.add("key", 10));
  BOOST_TEST(!ht.add("key", 20));
  BOOST_TEST(ht.table_size() == 1u);
  BOOST_TEST(*ht.find("key") == 10);
}

BOOST_AUTO_TEST_CASE(drop_existing_key) {
  Table ht(volkovich::SipHash{});
  ht.add("a", 1);
  ht.add("b", 2);
  BOOST_TEST(ht.drop("a") == 1);
  BOOST_TEST(ht.table_size() == 1u);
  BOOST_TEST(!ht.has("a"));
  BOOST_TEST(ht.has("b"));
  BOOST_TEST(ht.find("a") == nullptr);
}

BOOST_AUTO_TEST_CASE(drop_missing_key_throws) {
  Table ht(volkovich::SipHash{});
  BOOST_CHECK_THROW(ht.drop("missing"), std::out_of_range);
}

BOOST_AUTO_TEST_CASE(add_after_drop_reuses_slot) {
  Table ht(volkovich::SipHash{});
  ht.add("x", 1);
  ht.drop("x");
  BOOST_TEST(ht.add("x", 2));
  BOOST_TEST(ht.table_size() == 1u);
  BOOST_TEST(*ht.find("x") == 2);
}

BOOST_AUTO_TEST_CASE(iterator_visits_all_elements) {
  Table ht(volkovich::SipHash{});
  ht.add("a", 1);
  ht.add("b", 2);
  ht.add("c", 3);
  BOOST_TEST(countElements(ht) == 3u);
  const std::set<std::string> expected{"a", "b", "c"};
  BOOST_CHECK(collectKeys(ht) == expected);
}

BOOST_AUTO_TEST_CASE(const_iterator) {
  Table ht(volkovich::SipHash{});
  ht.add("foo", 42);
  ht.add("bar", 7);
  const Table& cref = ht;
  BOOST_TEST(countElements(cref) == 2u);
  const std::set<std::string> expected{"bar", "foo"};
  BOOST_CHECK(collectKeys(cref) == expected);
}

BOOST_AUTO_TEST_CASE(copy_constructor) {
  Table ht(volkovich::SipHash{});
  ht.add("k1", 1);
  ht.add("k2", 2);
  Table copy(ht);
  BOOST_TEST(copy.table_size() == ht.table_size());
  BOOST_TEST(collectKeys(copy) == collectKeys(ht));
  BOOST_TEST(*copy.find("k1") == 1);
  BOOST_TEST(*copy.find("k2") == 2);
  copy.add("k3", 3);
  BOOST_TEST(ht.table_size() == 2u);
  BOOST_TEST(copy.table_size() == 3u);
}

BOOST_AUTO_TEST_CASE(copy_assignment) {
  Table ht(volkovich::SipHash{});
  ht.add("a", 1);
  Table other(volkovich::SipHash{});
  other.add("b", 2);
  other = ht;
  BOOST_TEST(other.table_size() == 1u);
  BOOST_TEST(*other.find("a") == 1);
  BOOST_TEST(other.find("b") == nullptr);
}

BOOST_AUTO_TEST_CASE(move_constructor) {
  Table ht(volkovich::SipHash{});
  ht.add("m1", 10);
  ht.add("m2", 20);
  Table moved(std::move(ht));
  BOOST_TEST(moved.table_size() == 2u);
  BOOST_TEST(ht.table_size() == 0u);
  BOOST_TEST(ht.isEmpty());
  BOOST_TEST(*moved.find("m1") == 10);
  BOOST_TEST(*moved.find("m2") == 20);
}

BOOST_AUTO_TEST_CASE(move_assignment) {
  Table ht(volkovich::SipHash{});
  ht.add("x", 5);
  Table dst(volkovich::SipHash{});
  dst = std::move(ht);
  BOOST_TEST(dst.table_size() == 1u);
  BOOST_TEST(*dst.find("x") == 5);
  BOOST_TEST(ht.isEmpty());
}

BOOST_AUTO_TEST_CASE(rehash_keeps_elements) {
  Table ht(volkovich::SipHash{}, 4, 16, 2);
  for (int i = 0; i < 50; ++i) {
    BOOST_TEST(ht.add("key" + std::to_string(i), i));
  }
  BOOST_TEST(ht.table_size() == 50u);
  BOOST_TEST(countElements(ht) == 50u);
  for (int i = 0; i < 50; ++i) {
    const std::string key = "key" + std::to_string(i);
    BOOST_TEST(ht.has(key));
    BOOST_TEST(*ht.find(key) == i);
  }
}

BOOST_AUTO_TEST_CASE(overflow_storage) {
  Table ht(volkovich::SipHash{}, 2, 8, 2);
  BOOST_TEST(ht.add("b0k0", 0));
  BOOST_TEST(ht.add("b0k1", 1));
  BOOST_TEST(ht.add("b1k0", 2));
  BOOST_TEST(ht.add("b1k1", 3));
  BOOST_TEST(ht.add("overflow", 99));
  BOOST_TEST(ht.table_size() == 5u);
  BOOST_TEST(*ht.find("overflow") == 99);
  BOOST_TEST(countElements(ht) == 5u);
}

BOOST_AUTO_TEST_CASE(find_const_overload) {
  Table ht(volkovich::SipHash{});
  ht.add("const", 123);
  const Table& cref = ht;
  const int* value = cref.find("const");
  BOOST_REQUIRE(value != nullptr);
  BOOST_TEST(*value == 123);
  BOOST_TEST(cref.find("absent") == nullptr);
}
