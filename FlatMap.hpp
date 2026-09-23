#pragma once
#include <functional>
#include <optional>
#include <stdexcept>
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
  size_t size_ = 0;
  static constexpr double MAX_LOAD_FACTOR = 0.75;
  size_t hash(const K &key) const {
    return (std::hash<K>{}(key) * 11400714819323198485ULL) & mask_;
  }
  void rehash(size_t new_capacity) {
    std::vector<Entry> old_table = std::move(table_);
    capacity_ = new_capacity;
    mask_ = capacity_ - 1;
    table_.assign(capacity_, Entry{});
    size_ = 0;
    for (const auto &entry : old_table) {
      if (entry.is_occupied)
        insert(entry.key, entry.value);
    }
  }

public:
  explicit FlatMap(size_t capacity) : capacity_(capacity), mask_(capacity - 1) {
    if ((capacity & (capacity - 1)) != 0)
      throw std::invalid_argument("Capacity must be a power of 2");
    table_.resize(capacity_);
  }
  bool insert(const K &key, const V &value) {
    if (size_ >= capacity_ * MAX_LOAD_FACTOR)
      rehash(capacity_ * 2);
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
