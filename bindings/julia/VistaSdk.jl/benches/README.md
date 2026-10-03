# Vista SDK Julia - Performance Benchmarks

Benchmark framework: [BenchmarkTools.jl](https://github.com/JuliaCI/BenchmarkTools.jl)

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

| Component | Version |
| --------- | ------- |
| `julia`   | 1.13.1  |

---

## Running benchmarks

```bash
julia --project=. benches/codebook_methods.jl
julia --project=. benches/codebooks_lookup.jl
julia --project=. benches/gmod_load.jl
julia --project=. benches/gmod_lookup.jl
julia --project=. benches/gmod_path_parse.jl
julia --project=. benches/gmod_traversal.jl
julia --project=. benches/gmod_versioning_convert_path.jl
julia --project=. benches/local_id_parse.jl
julia --project=. benches/datachannel_list_lookup.jl
julia --project=. benches/datachannel_list_serialization.jl
julia --project=. benches/timeseries_data_serialization.jl
```

## Results

### Codebook methods

| Benchmark (ns/op)                     | `Linux julia` |
| ------------------------------------- | ------------: |
| `Quantity_hasStandardValue_hit`       |         91.00 |
| `Quantity_hasStandardValue_miss`      |        126.00 |
| `Quantity_createTag_standard`         |        141.00 |
| `Quantity_createTag_custom`           |        173.00 |
| `State_hasGroup_hit`                  |         61.00 |
| `State_hasGroup_miss`                 |         86.00 |
| `Position_validatePosition_simple`    |         68.00 |
| `Position_validatePosition_composite` |        389.00 |

### Codebooks lookup

| Benchmark (ns/op) | `Linux julia` |
| ----------------- | ------------: |
| `DictLookup`      |         50.00 |
| `Codebooks`       |         21.00 |

### Gmod cache access

| Benchmark (ns/op)       | `Linux julia` |
| ----------------------- | ------------: |
| `GmodCacheAccess_v3_4a` |         43.00 |
| `GmodCacheAccess_v3_7a` |         42.00 |

### Gmod node lookup

| Benchmark (ns/op) | `Linux julia` |
| ----------------- | ------------: |
| `DictLookup`      |         52.00 |
| `Gmod`            |        166.00 |

### Gmod path parsing

| Benchmark (ns/op)             | `Linux julia` |
| ----------------------------- | ------------: |
| `FromShortPath`               |        274.00 |
| `FromFullPath`                |       1896.00 |
| `FromShortPathIndividualized` |      1,091.00 |
| `FromFullPathIndividualized`  |       3355.00 |

### Gmod traversal

| Benchmark (ns/op)        |    `Linux julia` |
| ------------------------ | ---------------: |
| `FullTraversal/iterate`  |   566,727,720.00 |
| `FullTraversal/traverse` | 1,477,712,300.50 |

### Gmod versioning - path conversion

| Benchmark (ns/op)         | `Linux julia` |
| ------------------------- | ------------: |
| `ConvertPath`             |      4,605.00 |
| `ConvertPathWithLocation` |      2,938.00 |

### DataChannelList lookup

| Benchmark (ns/op) | `Linux julia` |
| ----------------- | ------------: |
| `ByShortId`       |         26.00 |
| `ByLocalId`       |         61.00 |

### LocalId parsing

| Benchmark (ns/op) | `Linux julia` |
| ----------------- | ------------: |
| `Simple`          |      2,012.00 |
| `Complex`         |      9,048.00 |

### DataChannelList serialization

| Benchmark (ns/op) | `Linux julia` |
| ----------------- | ------------: |
| `Serialize`       |      3,932.00 |
| `Deserialize`     |     62,444.50 |

### TimeSeriesData serialization

| Benchmark (ns/op) | `Linux julia` |
| ----------------- | ------------: |
| `Serialize`       |      3,010.00 |
| `Deserialize`     |     24,125.50 |

## See Also

- [Main SDK Documentation](../../../../README.md)
- [C++ Benchmarks](../../../cpp/benchmarks/README.md)

---

_Benchmarked on October 03, 2026_
