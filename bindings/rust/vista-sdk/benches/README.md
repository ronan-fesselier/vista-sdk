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
| `Quantity_hasStandardValue_hit`       |         72.80 |
| `Quantity_hasStandardValue_miss`      |        116.55 |
| `Quantity_createTag_standard`         |        102.23 |
| `Quantity_createTag_custom`           |        144.77 |
| `State_hasGroup_hit`                  |         51.33 |
| `State_hasGroup_miss`                 |         82.10 |
| `Position_validatePosition_simple`    |         54.89 |
| `Position_validatePosition_composite` |        349.33 |

### Codebooks lookup

| Benchmark (ns/op) | `Linux rustc` |
| ----------------- | ------------: |
| `StdUnorderedMap` |         51.74 |
| `StdMap`          |         66.11 |
| `Codebooks`       |         10.23 |

### Gmod cache access

| Benchmark (ns/op)       | `Linux rustc` |
| ----------------------- | ------------: |
| `GmodCacheAccess_v3_4a` |         33.57 |
| `GmodCacheAccess_v3_7a` |         34.33 |

### Gmod node lookup

| Benchmark (ns/op) | `Linux rustc` |
| ----------------- | ------------: |
| `StdUnorderedMap` |         41.97 |
| `Gmod`            |        158.25 |

### Gmod path parsing

| Benchmark (ns/op)             | `Linux rustc` |
| ----------------------------- | ------------: |
| `FromShortPath`               |        636.38 |
| `FromFullPath`                |        662.79 |
| `FromShortPathIndividualized` |      1,166.60 |
| `FromFullPathIndividualized`  |      1,056.20 |

### Gmod traversal

| Benchmark (ns/op) |  `Linux rustc` |
| ----------------- | -------------: |
| `FullTraversal`   | 133,770,000.00 |

### Gmod versioning - path conversion

| Benchmark (ns/op)         | `Linux rustc` |
| ------------------------- | ------------: |
| `ConvertPath`             |      2,799.50 |
| `ConvertPathWithLocation` |      2,822.20 |

### DataChannelList lookup

| Benchmark (ns/op) | `Linux rustc` |
| ----------------- | ------------: |
| `ByShortId`       |         21.56 |
| `ByLocalId`       |         54.90 |

### LocalId parsing

| Benchmark (ns/op) | `Linux rustc` |
| ----------------- | ------------: |
| `Simple`          |      1,459.40 |
| `Complex`         |      5,896.60 |

### DataChannelList serialization

| Benchmark (ns/op) | `Linux rustc` |
| ----------------- | ------------: |
| `Serialize`       |      3,094.90 |
| `Deserialize`     |     53,335.00 |

### TimeSeriesData serialization

| Benchmark (ns/op) | `Linux rustc` |
| ----------------- | ------------: |
| `Serialize`       |      2,388.50 |
| `Deserialize`     |     17,424.00 |

## See Also

- [Main SDK Documentation](../README.md)
- [C++ Benchmarks](../../../cpp/benchmarks/README.md)

---

_Benchmarked on August 29, 2026_
