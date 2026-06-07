#ifndef HTABLE
#define HTABLE
#define DEFAULT_CAPACITY 8
#define DEFAULT_BUCKET_COUNT 64
#define DEFAULT_OVERFLOW_SIZE 64
#include <boost/hash2/siphash.hpp>
#include <iostream>

namespace volkovich {
  template < class Key, class Value, class Hash, class Equal >
  class HashTable {
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
    size_t buckets_count_ = DEFAULT_BUCKET_COUNT, bucket_capacity_ = DEFAULT_CAPACITY,
           overflow_size_ = 0, real_size_ = 0;
    Hash hashf_;
    Equal eq_;

    size_t homeBucket(const Key& key) const {
      return hashf_(key) % buckets_count_;
    };

    Slot* get(const Key& key) {
      Slot* s = getFromBucket(key, homeBucket(key));
      return s ? s : getFromOverflow(key);
    }

    const Slot* get(const Key& key) const {
      const Slot* s = getFromBucket(key, homeBucket(key));
      return s ? s : getFromOverflow(key);
    }

    Slot* getFromBucket(const Key& key, size_t bucketNum) {
      if (bucketNum >= buckets_count_) {
        throw std::out_of_range("Bucket outside range");
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

    const Slot* getFromBucket(const Key& key, size_t bucketNum) const {
      if (bucketNum >= buckets_count_) {
        throw std::out_of_range("Bucket outside range");
      }
      for (size_t i = bucket_capacity_ * bucketNum; i < bucket_capacity_ * (bucketNum + 1); i++) {
        const Slot& s = slots_[i];
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

    const Slot* getFromOverflow(const Key& key) const {
      for (size_t i = 0; i < overflow_size_; i++) {
        const Slot& s = overflow_[i];
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

    Value* find(const Key& key) {
      Slot* s = get(key);
      if (!s) {
        return nullptr;
      }
      return &s->data.value;
    };

    const Value* find(const Key& key) const {
      const Slot* s = get(key);
      if (!s) {
        return nullptr;
      }
      return &s->data.value;
    };

    Value drop(const Key& k) {
      Slot* s = get(k);
      if (!s) {
        throw std::out_of_range("Key not found");
      }
      s->state = Slot::State::TOMBSTONE;
      real_size_--;
      return s->data.value;

    };
    bool has(Key k) {
      return get(k) != nullptr;
    };
    void rehash(size_t slots);

    HashTable(Hash hash_func, size_t bucket_count = DEFAULT_BUCKET_COUNT,
        size_t bucket_capacity = DEFAULT_CAPACITY, size_t overflow_size = DEFAULT_OVERFLOW_SIZE)
        : buckets_count_(bucket_count),
          bucket_capacity_(bucket_capacity),
          hashf_(hash_func),
          overflow_size_(overflow_size) {
      slots_ = new Slot[bucket_capacity * bucket_count];
      overflow_ = new Slot[overflow_size_];
    };

    ~HashTable() {
      delete[] slots_;
      delete[] overflow_;
    };

    HashTable(HashTable&& other)
        : buckets_count_(other.buckets_count_),
          bucket_capacity_(other.bucket_capacity_),
          slots_(other.slots_),
          overflow_(other.overflow_),
          real_size_(other.real_size_),
          hashf_(std::move(other.hashf_)),
          eq_(std::move(other.eq_)) {
      other.slots_ = nullptr;
      other.overflow_ = nullptr;
      other.real_size_ = 0;
      other.overflow_size_ = 0;
      other.buckets_count_ = 0;
    };

    HashTable(const HashTable& other)
        : buckets_count_(other.buckets_count_),
          bucket_capacity_(other.bucket_capacity_),
          hashf_(other.hashf_),
          slots_(other.slots_),
          overflow_(other.overflow_),
          real_size_(other.real_size_),
          hashf_(other.hashf_),
          eq_(other.eq_) {
      slots_ = new Slot[other.buckets_count_ * other.bucket_capacity_];
      for (size_t i = 0; i < other.buckets_count_ * other.bucket_capacity_; i++) {
        slots_[i] = other.slots_[i];
      }
      try {
        overflow_ = new Slot[other.overflow_size_];
      } catch (...) {
        delete[] slots_;
        delete[] overflow_;
        throw std::bad_alloc("Error while initializing HTable");
      }
      for (size_t i = 0; i < other.overflow_size_; i++) {
        overflow_[i] = other.overflow_[i];
      }
    };

    HashTable& operator=(HashTable&& other) {
      swap(other);
      return *this;
    };

    HashTable& operator=(const HashTable& other) {
      if (this == &other) return *this;
      HashTable tmp(other);
      swap(tmp);
      return *this;
    };

    void swap(HashTable& other) noexcept {
      std::swap(slots_, other.slots_);
      std::swap(overflow_, other.overflow_);
      std::swap(bucket_capacity_, other.bucket_capacity_);
      std::swap(buckets_count_, other.buckets_count_);
      std::swap(overflow_size_, other.overflow_size_);
      std::swap(real_size_, other.real_size_);
      std::swap(hashf_, other.hashf_);
      std::swap(eq_, other.eq_);
    };

    size_t table_size() const noexcept {
      return real_size_;
    };

    bool isEmpty() const noexcept {
      return real_size_ == 0;
    };
  };

}
#endif
