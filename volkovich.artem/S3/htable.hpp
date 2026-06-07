#ifndef HTABLE
#define HTABLE
#define DEFAULT_CAPACITY 8
#define DEFAULT_BUCKET_COUNT 64
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

    Slot *slots_{}, *overflow_{};
    size_t buckets_count_, bucket_capacity_ = DEFAULT_CAPACITY, overflow_size_, real_size_;
    Hash hashf_;
    Equal eq_;

    size_t homeBucket(const Key& key) const {
      return hashf_(key) % buckets_count_;
    };

    Slot* get(const Key& key) {
      Slot* s = getFromBucket(key, homeBucket(key));
      return s ? s : getFromOverflow(key);
    }

    Slot* getFromBucket(const Key& key, size_t bucketNum) {
      if (bucketNum > buckets_count_) {
        throw std::logic_error("Bucket outside range");
      }
      for (size_t i = bucket_capacity_ * bucketNum; i < bucket_capacity_ * (bucketNum + 1); i++) {
        Slot& s = slots_[i];
        if (s.state == Slot::State::EMPTY) {
          return nullptr;
        }
        if (eq_(s.data.key, key)) {
          return &s;
        }
      }
      return nullptr;
    }

    Slot* getFromOverflow(const Key& key) {
      for (size_t i = 0; i < overflow_size_; i++) {
        Slot& s = overflow_[i];
        if (eq_(s.data.key, key)) {
          return &s;
        }
      }
      return nullptr;
    };

    Slot* getEmptyBucketSpace(size_t bucketNum) {
      for (size_t i = bucket_capacity_ * bucketNum; i < bucket_capacity_ * (bucketNum + 1); i++) {
        Slot& s = slots_[i];
        if (s.state == Slot::State::EMPTY || s.state == Slot::State::TOMBSTONE) {
          return &s;
        }
      }
      return nullptr;
    }
    Slot* getEmptyOverflowSpace() {
      for (size_t i = 0; i < overflow_size_; i++) {
        Slot& s = overflow_[i];
        if (s.state == Slot::State::EMPTY || s.state == Slot::State::TOMBSTONE) {
          return &s;
        }
      }
      return nullptr;
    }

    double loadFactor() const {
      return static_cast< double >(real_size_) /
             static_cast< double >(bucket_capacity_ * buckets_count_ + overflow_size_);
    }

   public:
    bool add(Key k, Value v) noexcept {
      if (loadFactor() > 0.75) {
        rehash(buckets_count_ * 2);
      }
      if (has(k)) {
        return false;
      }
      size_t bucket = homeBucket(k);
      Slot* dest = getEmptyBucketSpace(bucket);
      if (!dest) {
        dest = getEmptyOverflowSpace();
      }
      if (!dest) {
        return false;
      }
      dest->data.key = k;
      dest->data.value = v;
      dest->state = Slot::State::OCCUPIED;
      real_size_++;
      return true;
    };
    Value drop(Key k);
    bool has(Key k) {
      return get(k);
    };
    void rehash(size_t slots);

    HashTable(size_t bucket_count = DEFAULT_BUCKET_COUNT, size_t bucket_capacity = DEFAULT_CAPACITY)
        : buckets_count_(bucket_count), bucket_capacity_(bucket_capacity) {
      slots_ = new Slot[bucket_capacity * bucket_count];
      overflow_ = new Slot[overflow_size_];
    };

    ~HashTable() {

    };
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
