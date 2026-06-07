#ifndef HTABLE
#define HTABLE
#define DEFAULT_CAPACITY 8
#include <boost/container_hash/hash.hpp>
#include <iostream>

namespace volkovich {
  template < class Key, class Value, class Hash, class Equal >
  class HashTable {
    struct SipHash {
      std::size_t operator()(const std::string& key) const {
        uint64_t hash = siphash(key.data(), key.size(), secret_key);
        return static_cast< std::size_t >(hash);
      }

     private:
      static constexpr uint8_t secret_key[16] = {
          0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    };

    struct Record {
      Key key;
      Value value;
    };

    struct Slot {
      enum class State { EMPTY, OCCUPIED, TOMBSTONE };
      Record data;
      State state = State::EMPTY;
    };

    Slot* slots_;
    size_t buckets_count_, bucket_capacity_ = DEFAULT_CAPACITY, overflow_size_;
    Hash hashf_;
    Equal eq_;

    size_t homeBucket(Key key) const{
      return hashf_(key) % buckets_count_;
    };


   public:
    Record probe(K key) {};
    bool add(Key k, Value v) {
      if (has(k)) {
        return false;
      }
      Record res = probe(k);
    };
    Value drop(Key k);
    bool has(Key k) {
      return get(k);
    };
    void rehash(size_t slots);
    HashTable(size_t size = DEFAULT_SIZE) : size(size) {};
    ~HashTable();
    HashTable(HashTable&& other);
    HashTable(const HashTable& other);
    HashTable& operator=(HashTable&& other);
    HashTable operator=(const HashTable& other);
    void swap(HashTable& other);
    size_t table_size();
    bool isEmpty();
    Record* get(K key);
  };

}
#endif
