#include "FlatMap.hpp"
#include <chrono>
#include <iostream>
#include <unordered_map>

const size_t MAP_SIZE = 4096; // Power-of-two array capacity configuration
const size_t LOOKUP_RUNS = 15'000'000;

int main() {
  FlatMap<int64_t, int64_t> flat_map(MAP_SIZE);
  std::unordered_map<int64_t, int64_t> std_map;

  // Fill both structural systems with identical keys and values
  for (size_t i = 0; i < MAP_SIZE / 2; ++i) {
    flat_map.insert(i * 5, i * 100);
    std_map[i * 5] = i * 100;
  }

  std::cout
      << "Starting 15 Million Iteration Performance Lookup Comparison...\n\n";

  // --- BENCHMARK 1: Standard Chained Hash Map ---
  auto start_std = std::chrono::high_resolution_clock::now();
  int64_t sum_std = 0;
  for (size_t i = 0; i < LOOKUP_RUNS; ++i) {
    auto it = std_map.find((i % (MAP_SIZE / 2)) * 5);
    if (it != std_map.end())
      sum_std += it->second;
  }
  auto end_std = std::chrono::high_resolution_clock::now();
  auto std_time =
      std::chrono::duration_cast<std::chrono::milliseconds>(end_std - start_std)
          .count();
  std::cout << " Standard std::unordered_map Runtime : " << std_time << " ms\n";

  // --- BENCHMARK 2: Contiguous Flat Hash Map ---
  auto start_flat = std::chrono::high_resolution_clock::now();
  int64_t sum_flat = 0;
  for (size_t i = 0; i < LOOKUP_RUNS; ++i) {
    auto val = flat_map.find((i % (MAP_SIZE / 2)) * 5);
    if (val.has_value())
      sum_flat += val.value();
  }
  auto end_flat = std::chrono::high_resolution_clock::now();
  auto flat_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                       end_flat - start_flat)
                       .count();
  std::cout << " Cache-Conscious FlatMap Runtime     : " << flat_time
            << " ms\n\n";

  std::cout << "========================================================\n";
  double speedup = static_cast<double>(std_time) / flat_time;
  std::cout << " Contiguous Memory Execution Speedup : " << speedup
            << "x Faster\n";
  std::cout << "========================================================\n";

  std::cout << " (Verification Checksums - std: " << sum_std
            << " | flat: " << sum_flat << ")\n";
  return 0;
}
