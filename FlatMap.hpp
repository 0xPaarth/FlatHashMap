#pragma once
#include <functional>
#include <optional>
#include <vector>
template <typename K, typename V> class FlatMap {
private:
  struct Entry {
    K key;
    V value;
    bool is_occupied = false;
  };
  std::vector<Entry> table_;
  size_t capacity_;
  size_t mask_;
  size_t hash(const K &key) const {
    return (std::hash<K>{}(key) * 11400714819323198485ULL) & mask_;
  }

public:
  explicit FlatMap(size_t capacity) : capacity_(capacity), mask_(capacity - 1) {
    table_.resize(capacity_);
  }
  bool insert(const K &key, const V &value) {
    size_t idx = hash(key);
    while (table_[idx].is_occupied) {
      if (table_[idx].key == key) {
        table_[idx].value = value;
        return true;
      }
      idx = (idx + 1) & mask_;
    }
    table_[idx] = {key, value, true};
    return true;
  }
  std::optional<V> find(const K &key) const {
    size_t idx = hash(key);
    size_t start_idx = idx;
    while (table_[idx].is_occupied) {
      if (table_[idx].key == key)
        return table_[idx].value;
      idx = (idx + 1) & mask_;
      if (idx == start_idx)
        break;
    }
    return std::nullopt;
  }
};
