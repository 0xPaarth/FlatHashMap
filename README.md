
# FlatHashMap
=======
# Cache-Conscious Flat Hash Map

A high-performance, contiguous-memory hash table built from scratch in C++20. This repository demonstrates how data-oriented design patterns can be leveraged to eliminate pointer-chasing and CPU cache misses inside associative data structures.

## Performance Benchmarks

- Workload Scale: 15,000,000 lookup operations
- Standard std::unordered_map Runtime: ~233 ms to 266 ms
- Cache-Conscious FlatMap Runtime: ~34 ms to 53 ms
- Execution Speedup Factor: 2.0x to 7.8x Faster

By replacing node chaining (linked lists) with linear probing inside a single contiguous array, lookups are executed within predictable hardware limits, maximizing L1/L2 cache hits per clock cycle.

## The Architectural Bottleneck

A standard `std::unordered_map` relies on bucket chaining. When structural collisions happen, the map creates a linked list of nodes dynamically allocated across the heap. When navigating these node structures via pointer hops, the CPU frequently experiences cache invalidation stalls (Cache Misses) while waiting for the main memory bus to return data.

This implementation resolves the pointer bottleneck by utilizing two systems optimization patterns:
1. **Flat Data Contiguity:** Keys, values, and state flags are packed tightly inside a sequential array matrix structure (`std::vector<Entry>`). When a key is pulled into memory, its adjacent lookup data is automatically cached inside the same 64-byte physical hardware cache line.
2. **Linear Probing Operations:** Structural collisions are resolved by scanning forward to the next sequential index slot rather than creating a nested node, ensuring memory fetches remain entirely sequential.
3. **Bitwise Mask Hashing:** Capacity sizes are strictly constrained to powers of two, allowing index wrap-around operations to bypass expensive mathematical modulo operations (`%`) in favor of rapid bitwise operations (`&`).

## Build and Execution

To compile with aggressive optimization pipelines and hardware vectorization flags on Arch Linux:

```bash
g++ -O3 -std=c++20 main.cpp -o map_benchmark
./map_benchmark
```
