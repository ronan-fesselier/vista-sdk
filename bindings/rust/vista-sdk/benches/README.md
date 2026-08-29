# Vista SDK Rust - Performance Benchmarks

Benchmark framework: [criterion](https://github.com/bheisler/criterion.rs) v0.8.2

---

## Test Environment

| Component       | Specification                 |
| --------------- | ----------------------------- |
| **CPU**         | 12th Gen Intel Core i7-12800H |
| **Base Clock**  | 2.80 GHz                      |
| **Turbo Clock** | 4.80 GHz                      |
| **RAM**         | 32 GB DDR5-4800               |
| **OS**          | Linux 6.12 (Debian 13)        |

## Toolchain

| Compiler | Version                       | Profile   |
| -------- | ----------------------------- | --------- |
| `rustc`  | 1.98.1 (48a229cea 2026-09-01) | `release` |

---

## Results

### Codebook methods

| Benchmark (ns/op)                     | `Linux rustc` |
| ------------------------------------- | ------------: |
| `Quantity_hasStandardValue_hit`       |         74.81 |
| `Quantity_hasStandardValue_miss`      |        122.21 |
| `Quantity_createTag_standard`         |        105.25 |
| `Quantity_createTag_custom`           |        147.34 |
| `State_hasGroup_hit`                  |         53.68 |
| `State_hasGroup_miss`                 |         86.79 |
| `Position_validatePosition_simple`    |         57.38 |
| `Position_validatePosition_composite` |        353.88 |

### Codebooks lookup

| Benchmark (ns/op) | `Linux rustc` |
| ----------------- | ------------: |
| `StdUnorderedMap` |         51.74 |
| `StdMap`          |         66.11 |
| `Codebooks`       |         10.23 |

### Gmod cache access

| Benchmark (ns/op)       | `Linux rustc` |
| ----------------------- | ------------: |
| `GmodCacheAccess_v3_4a` |         34.98 |
| `GmodCacheAccess_v3_7a` |         35.06 |

### Gmod node lookup

| Benchmark (ns/op) | `Linux rustc` |
| ----------------- | ------------: |
| `StdUnorderedMap` |         45.56 |
| `Gmod`            |        168.48 |

### Gmod path parsing

| Benchmark (ns/op)             | `Linux rustc` |
| ----------------------------- | ------------: |
| `FromShortPath`               |        702.06 |
| `FromFullPath`                |        689.46 |
| `FromShortPathIndividualized` |      1,236.50 |
| `FromFullPathIndividualized`  |      1,096.20 |

### Gmod traversal

| Benchmark (ns/op) |  `Linux rustc` |
| ----------------- | -------------: |
| `FullTraversal`   | 143,980,000.00 |

### Gmod versioning - path conversion

| Benchmark (ns/op)         | `Linux rustc` |
| ------------------------- | ------------: |
| `ConvertPath`             |      3,112.70 |
| `ConvertPathWithLocation` |      3,089.70 |

### DataChannelList lookup

| Benchmark (ns/op) | `Linux rustc` |
| ----------------- | ------------: |
| `ByShortId`       |         25.90 |
| `ByLocalId`       |         65.16 |

### LocalId parsing

| Benchmark (ns/op) | `Linux rustc` |
| ----------------- | ------------: |
| `Simple`          |      1,708.60 |
| `Complex`         |      6,471.60 |

### DataChannelList serialization

| Benchmark (ns/op) | `Linux rustc` |
| ----------------- | ------------: |
| `Serialize`       |      3,613.40 |
| `Deserialize`     |     58,636.00 |

### TimeSeriesData serialization

| Benchmark (ns/op) | `Linux rustc` |
| ----------------- | ------------: |
| `Serialize`       |      2,775.40 |
| `Deserialize`     |     19,200.00 |

## See Also

- [Main SDK Documentation](../README.md)
- [C++ Benchmarks](../../../cpp/benchmarks/README.md)

---

_Benchmarked on August 29, 2026_
