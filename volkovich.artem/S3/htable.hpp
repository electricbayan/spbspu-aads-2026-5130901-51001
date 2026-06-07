#ifndef HTABLE
#define HTABLE
#define DEFAULT_SIZE 8
#include <boost/container_hash/hash.hpp>
#include <iostream>

namespace volkovich {
  template < class Key, class Value, class Hash, class Equal >
  class HashTable {
    struct Record {
      Key key;
      Value value;
      bool occupied = false;
    };
    struct Bucket {
     private:
      size_t size = 0;
      size_t capacity = DEFAULT_SIZE;
      Record* data;
      void grow() {
        capacity *= 2;
        Record* tmp = new Record[capacity];
        for (size_t i = 0; i < size; i++) {
          tmp[i] = std::move(data[i]);
        }
        delete[] data;
        data = tmp;
      };

      Record* buckets;

     public:
      Bucket(size_t capacity) : capacity(capacity) {
        Record* data = new Record[capacity];
      }
      ~Bucket() {
        delete[] data;
      }
      void insert(Record rec) {
        if (size == capacity) {
          grow();
        }
        data[size] = rec;
        size++;
      };

   } public: bool add(Key k, Value v) {
    if (has(k)) {
      return false;
    }
    Record res = emptySlot(k);
   };
    Value drop(Key k);
    bool has(Key k);
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
    Record emptySlot(K key);
  };

}
#endif
