# Vista SDK - C++ Benchmarking

**Windows:**

```bash
# Build benchmarks
cmake --build build --target BM_CodebooksLookup

# Execute with detailed output
.\build\bin\Release\BM_CodebooksLookup.exe --benchmark_min_time=10
```

**Linux:**

```bash
# Build benchmarks
cmake --build build --target BM_CodebooksLookup

# Execute with detailed output
./build/bin/Release/BM_CodebooksLookup --benchmark_min_time=10
```

## Test Environment

**Hardware Configuration:**

-   **CPU:** 12th Gen Intel Core i7-12800H (20 logical cores, 14 physical cores) @ 2.80 GHz
-   **RAM:** DDR4-3200 (32GB) - _will update with Linux system specs_
-   **CPU Cache:**
    -   L1 Data: 48 KiB (x10 cores)
    -   L1 Instruction: 32 KiB (x10 cores)
    -   L2 Unified: 1280 KiB (x10 cores)
    -   L3 Unified: 24576 KiB (x1 shared)

**Software Configuration:**

| Platform    | C++ Toolchain                              | C# Runtime               | OS Version |
| :---------- | :----------------------------------------- | :----------------------- | :--------- |
| **Windows** | Google Benchmark, MSVC 2022, Release build | .NET 8.0.16, RyuJIT AVX2 | Windows 10 |
| **Linux**   | TBC                                        | TBC                      | TBC        |

---

## Summary

| Operation Category          | C++ vs C# Performance |  Status  | Key Findings                      |
| :-------------------------- | :-------------------- | :------: | :-------------------------------- |
| **Hash Table Operations**   | **1.14-2.61x faster** |    ✅    | Superior low-level performance    |
| **Codebook Access**         | **2.40x faster**      |    ✅    | Hash maps dominate dictionaries   |
| **High-Level APIs**         | **4.09x slower**      |   ❌❌   | **Critical optimization needed**  |
| **String Hashing**          | **1.00-6.05x slower** |    ❌    | Severe optimization required      |
| **GMOD Loading**            | **1.15x faster**      |    ✅    | Competitive with 68x less memory  |
| **GMOD Lookup**             | **1.14-2.61x faster** |    ✅    | Hash table advantage maintained   |
| **GMOD Traversal**          | **1.7x slower**       |    ❌    | Requires optimization             |
| **Path Parsing**            | **2.0-34.7x slower**  |   ❌❌   | **Severe performance regression** |
| **Version Path Conversion** | **293x slower**       | ❌❌❌❌ | **CATASTROPHIC**                  |

---

## Codebooks Lookup

### Cross-Platform Performance Comparison

| Operation             | Windows C++ | vs Baseline | Linux C++ | vs Baseline | Notes                         |
| :-------------------- | :---------- | :---------: | :-------- | :---------: | :---------------------------- |
| **Array Lookup**      | 0.930 ns    |  **1.00x**  | _TBD_     |    _TBD_    | ⚡ Fastest possible operation |
| **Hash Table Lookup** | 2.04 ns     |  **2.19x**  | _TBD_     |    _TBD_    | 🔥 2.19x slower than baseline |
| **SDK Codebooks API** | 2.82 ns     |  **3.03x**  | _TBD_     |    _TBD_    | 🐌 3.03x slower than baseline |

### Codebooks Lookup Performance (Windows)

Performance comparison between C++ and C# implementations for codebook access operations:

| C++ Method            | C++ Time | C++ Implementation                    | C# Method     | C# Time | C# Implementation             | Performance Ratio   |
| :-------------------- | :------- | :------------------------------------ | :------------ | :------ | :---------------------------- | :------------------ |
| **HashTableLookup**   | 2.04 ns  | `std::unordered_map::find()`          | **Dict**      | 4.90 ns | `Dictionary.TryGetValue()`    | ✅ **2.40x faster** |
| **SdkApiCodebooks**   | 2.82 ns  | `codebooks_ref[enum_key]` + try/catch | **Codebooks** | 0.69 ns | `array[index]` + bounds check | ❌ **4.09x slower** |
| **SdkApiArrayLookup** | 0.930 ns | `std::array[index] != nullptr`        | _No equiv_    | _N/A_   | _N/A_                         | _N/A_               |

#### Detailed C++ Results (Windows)

| Benchmark             | Time     | CPU      | Iterations | Memory Usage |
| :-------------------- | :------- | :------- | :--------- | :----------- |
| **HashTableLookup**   | 2.04 ns  | 2.04 ns  | 6.9B       | 0 KB         |
| **SdkApiCodebooks**   | 2.82 ns  | 2.82 ns  | 5.0B       | 0 KB         |
| **SdkApiArrayLookup** | 0.930 ns | 0.930 ns | 15.1B      | 0 KB         |

#### Detailed C# Results (Windows)

| Method     |      Mean |     Error |    StdDev |        Ratio | RatioSD | Rank | Allocated | Alloc Ratio |
| ---------- | --------: | --------: | --------: | -----------: | ------: | ---: | --------: | ----------: |
| FrozenDict | 4.9802 ns | 0.0699 ns | 0.0584 ns | 1.02x slower |   0.01x |    3 |         - |          NA |
| Dict       | 4.8993 ns | 0.0239 ns | 0.0212 ns |     baseline |         |    2 |         - |          NA |
| Codebooks  | 0.6937 ns | 0.0219 ns | 0.0195 ns | 7.07x faster |   0.21x |    1 |         - |          NA |

---

## GMOD Load

### Cross-Platform Performance Comparison

| Operation     | Windows C++ | vs Baseline | Linux C++ | vs Baseline | Notes                     |
| :------------ | :---------- | :---------: | :-------- | :---------: | :------------------------ |
| **GMOD Load** | 26.5 ms     |  **1.00x**  | _TBD_     |    _TBD_    | ⚡ Full GMOD construction |

### GMOD Load Performance (Windows)

| C++ Method   | C++ Time | C++ Implementation                  | C# Method | C# Time | C# Implementation               | Performance Ratio   |
| :----------- | :------- | :---------------------------------- | :-------- | :------ | :------------------------------ | :------------------ |
| **GmodLoad** | 26.5 ms  | `VIS::loadGmodDto()` + construction | **Load**  | 30.4 ms | `Gmod.Load()` with full parsing | ✅ **1.15x faster** |

#### Detailed C++ Results (Windows)

| Benchmark    | Time    | CPU     | Iterations | Memory Usage |
| :----------- | :------ | :------ | :--------- | :----------- |
| **GmodLoad** | 26.5 ms | 26.4 ms | 484        | 224 KB       |

#### Detailed C# Results (Windows)

| Method   | Mean     | Error    | StdDev   | Gen0   | Gen1   | Gen2  | Allocated |
| :------- | :------- | :------- | :------- | :----- | :----- | :---- | :-------- |
| **Load** | 30.40 ms | 0.607 ms | 1.078 ms | 1562.5 | 1500.0 | 562.5 | 15.41 MB  |

---

## GMOD Lookup

### Cross-Platform Performance Comparison

| Operation             | Windows C++ | vs Baseline | Linux C++ | vs Baseline | Notes                    |
| :-------------------- | :---------- | :---------: | :-------- | :---------: | :----------------------- |
| **Frozen Dictionary** | 13.3 ns     |  **1.00x**  | _TBD_     |    _TBD_    | ⚡ Fastest lookup method |
| **Hash Table Lookup** | 14.7 ns     |  **1.11x**  | _TBD_     |    _TBD_    | 🔥 Slightly slower       |
| **GMOD API Lookup**   | 173 ns      |  **13.0x**  | _TBD_     |    _TBD_    | 🐌 Needs optimization    |

### GMOD Lookup Performance (Windows)

| C++ Method     | C++ Time | C++ Implementation            | C# Method      | C# Time  | C# Implementation                | Performance Ratio   |
| :------------- | :------- | :---------------------------- | :------------- | :------- | :------------------------------- | :------------------ |
| **FrozenDict** | 13.3 ns  | `std::unordered_map` (frozen) | **FrozenDict** | 15.21 ns | `FrozenDictionary.TryGetValue()` | ✅ **1.14x faster** |
| **Dict**       | 14.7 ns  | `std::unordered_map::find()`  | **Dict**       | 38.34 ns | `Dictionary.TryGetValue()`       | ✅ **2.61x faster** |
| **Gmod**       | 173 ns   | CHD implementation lookup     | **Gmod**       | 15.62 ns | Native GMOD API access           | ❌ **11.1x slower** |

#### Detailed C++ Results (Windows)

| Benchmark      | Time    | CPU     | Iterations | Memory Usage |
| :------------- | :------ | :------ | :--------- | :----------- |
| **Dict**       | 14.7 ns | 14.6 ns | 963M       | 0 KB         |
| **FrozenDict** | 13.3 ns | 13.3 ns | 1,018M     | 0 KB         |
| **Gmod**       | 173 ns  | 173 ns  | 81M        | 0 KB         |

#### Detailed C# Results (Windows)

| Method         | Mean     | Error    | StdDev   | Ratio        | RatioSD | Rank | Allocated | Alloc Ratio |
| :------------- | :------- | :------- | :------- | :----------- | :------ | :--- | :-------- | :---------- |
| **Dict**       | 38.34 ns | 0.227 ns | 0.201 ns | baseline     |         | 3    | -         | NA          |
| **FrozenDict** | 15.21 ns | 0.039 ns | 0.035 ns | 2.52x faster | 0.02x   | 1    | -         | NA          |
| **Gmod**       | 15.62 ns | 0.255 ns | 0.226 ns | 2.46x faster | 0.04x   | 2    | -         | NA          |

---

## GMOD Path Parsing

### Cross-Platform Performance Comparison

| Operation                          | Windows C++ | Linux C++ | Status | Notes |
| :--------------------------------- | :---------- | :-------- | :----: | :---- |
| **TryParse**                       | 6.75 μs     | _TBD_     |        |       |
| **TryParseFullPath**               | 19.8 μs     | _TBD_     |        |       |
| **TryParseIndividualized**         | 4.21 μs     | _TBD_     |        |       |
| **TryParseFullPathIndividualized** | 20.2 μs     | _TBD_     |        |       |

### GMOD Path Parsing Performance (Windows)

Performance comparison between C++ and C# implementations for GMOD path parsing operations:

| Operation                          | C++ (Google Benchmark) | C# (BenchmarkDotNet) | Performance Ratio | Status | Notes                                       |
| :--------------------------------- | :--------------------- | :------------------- | :---------------- | :----: | :------------------------------------------ |
| **TryParse**                       | 6.75 μs                | 3.36 μs              | **2.0x slower**   |   ❌   | C++ path parsing overhead                   |
| **TryParseFullPath**               | 19.8 μs                | 571 ns               | **34.7x slower**  | ❌❌❌ | Critical performance issue - full paths     |
| **TryParseIndividualized**         | 4.21 μs                | 1.49 μs              | **2.8x slower**   |   ❌   | C++ location parsing overhead               |
| **TryParseFullPathIndividualized** | 20.2 μs                | 694 ns               | **29.1x slower**  | ❌❌❌ | Critical performance issue - individualized |

#### Detailed C++ Results (Windows)

| Benchmark                          | Time    | CPU     | Iterations | Memory Usage |
| :--------------------------------- | :------ | :------ | :--------- | :----------- |
| **TryParse**                       | 6.75 μs | 6.75 μs | 2,103K     | 0 KB         |
| **TryParseFullPath**               | 19.8 μs | 19.8 μs | 711K       | 0 KB         |
| **TryParseIndividualized**         | 4.21 μs | 4.21 μs | 3,343K     | 0 KB         |
| **TryParseFullPathIndividualized** | 20.2 μs | 20.2 μs | 695K       | 0 KB         |

#### Detailed C# Results (Windows)

| Method                             | Mean       | Error    | StdDev   | Categories    | Gen0   | Allocated |
| :--------------------------------- | :--------- | :------- | :------- | :------------ | :----- | :-------- |
| **TryParse**                       | 3,360.8 ns | 41.66 ns | 38.97 ns | No location   | 0.2213 | 2,792 B   |
| **TryParseFullPath**               | 571.2 ns   | 7.45 ns  | 5.82 ns  | No location   | 0.0601 | 760 B     |
| **TryParseIndividualized**         | 1,492.2 ns | 28.77 ns | 51.14 ns | With location | 0.2251 | 2,832 B   |
| **TryParseFullPathIndividualized** | 694.1 ns   | 4.33 ns  | 3.84 ns  | With location | 0.0935 | 1,176 B   |

---

## GMOD Traversal

### Cross-Platform Performance Comparison

| Operation          | Windows C++ | Linux C++ | Status | Notes |
| :----------------- | :---------- | :-------- | :----: | :---- |
| **Full Traversal** | 277 ms      | _TBD_     |        |       |

### GMOD Traversal Performance (Windows)

Performance comparison between C++ and C# implementations for GMOD node lookup operations:

| Operation          | C++ (Google Benchmark) | C# (BenchmarkDotNet) | Performance Ratio | Status | Notes                                |
| :----------------- | :--------------------- | :------------------- | :---------------- | :----: | :----------------------------------- |
| **Full Traversal** | 277 ms                 | 162.9 ms             | **1.7x slower**   |   ❌   | Significant optimization opportunity |

#### Detailed C++ Results (Windows)

| Benchmark         | Time   | CPU    | Iterations | Memory Usage |
| :---------------- | :----- | :----- | :--------- | :----------- |
| **FullTraversal** | 277 ms | 277 ms | 50-51      | 0 KB         |

#### Detailed C# Results (Windows)

| Method            | Mean     | Error   | StdDev  | Allocated |
| :---------------- | :------- | :------ | :------ | :-------- |
| **FullTraversal** | 162.9 ms | 0.97 ms | 0.91 ms | 5.3 KB    |

---

## GMOD Versioning Path Conversion

### Cross-Platform Performance Comparison

| Operation        | Windows C++ | Linux C++ | Status | Notes |
| :--------------- | :---------- | :-------- | :----: | :---- |
| **Convert Path** | 436 μs      | _TBD_     |        |       |

### GMOD Versioning Path Conversion Performance (Windows)

Performance comparison between C++ and C# implementations for GMOD version path conversion operations:

| Operation        | C++ (Google Benchmark) | C# (BenchmarkDotNet) | Performance Ratio |  Status  | Notes            |
| :--------------- | :--------------------- | :------------------- | :---------------- | :------: | :--------------- |
| **Convert Path** | 436 μs                 | 1.489 μs             | **293x slower**   | ❌❌❌❌ | **CATASTROPHIC** |

#### Detailed C++ Results (Windows)

| Benchmark       | Time   | CPU    | Iterations | Memory Usage |
| :-------------- | :----- | :----- | :--------- | :----------- |
| **ConvertPath** | 436 μs | 435 μs | 32,464     | 0 KB         |

#### Detailed C# Results (Windows)

| Method          | Mean     | Error     | StdDev    | Gen0   | Allocated |
| :-------------- | :------- | :-------- | :-------- | :----- | :-------- |
| **ConvertPath** | 1.489 μs | 0.0108 μs | 0.0090 μs | 0.2575 | 3.17 KB   |

---

## Short String Hashing

### Cross-Platform Performance Comparison

| Operation          | Windows C++ | Linux C++ | Status | Notes |
| :----------------- | :---------- | :-------- | :----: | :---- |
| **Bcl (400)**      | 1.21 ns     | _TBD_     |        |       |
| **Bcl (H346)**     | 3.33 ns     | _TBD_     |        |       |
| **BclOrd (400)**   | 1.76 ns     | _TBD_     |        |       |
| **BclOrd (H346)**  | 3.07 ns     | _TBD_     |        |       |
| **Larsson (400)**  | 2.37 ns     | _TBD_     |        |       |
| **Larsson (H346)** | 17.6 ns     | _TBD_     |        |       |
| **CRC32 (400)**    | 2.13 ns     | _TBD_     |        |       |
| **CRC32 (H346)**   | 19.8 ns     | _TBD_     |        |       |
| **FNV (400)**      | 2.37 ns     | _TBD_     |        |       |
| **FNV (H346)**     | 17.7 ns     | _TBD_     |        |       |

### Short String Hashing Performance (Windows)

Performance comparison between C++ and C# implementations for hash function operations:

#### String "400" (3 characters)

| Algorithm          | C++ (Google Benchmark) | C# (BenchmarkDotNet) | Performance Ratio | Status | Notes                                      |
| :----------------- | :--------------------- | :------------------- | :---------------- | :----: | :----------------------------------------- |
| **Bcl**            | 1.21 ns                | 1.156 ns             | **1.05x slower**  |   ✅   | Competitive performance                    |
| **BclOrd**         | 1.76 ns                | 1.536 ns             | **1.15x slower**  |   ✅   | Slightly slower but acceptable             |
| **Larsson**        | 2.37 ns                | 1.225 ns             | **1.93x slower**  |   ❌   | Character-by-character processing overhead |
| **Crc32Intrinsic** | 2.13 ns                | 1.209 ns             | **1.76x slower**  |   ❌   | Hardware intrinsic overhead                |
| **Fnv**            | 2.37 ns                | 1.220 ns             | **1.94x slower**  |   ❌   | Manual implementation inefficiency         |

#### String "H346.11112" (10 characters)

| Algorithm          | C++ (Google Benchmark) | C# (BenchmarkDotNet) | Performance Ratio | Status | Notes                              |
| :----------------- | :--------------------- | :------------------- | :---------------- | :----: | :--------------------------------- |
| **Bcl**            | 3.33 ns                | 4.521 ns             | **1.36x faster**  |   ✅   | C++ advantage for longer strings   |
| **BclOrd**         | 3.07 ns                | 3.072 ns             | **1.00x equal**   |   ✅   | Nearly identical performance       |
| **Larsson**        | 17.6 ns                | 3.405 ns             | **5.17x slower**  |   ❌   | Severe performance degradation     |
| **Crc32Intrinsic** | 19.8 ns                | 3.274 ns             | **6.05x slower**  |   ❌   | Critical performance issue         |
| **Fnv**            | 17.7 ns                | 3.276 ns             | **5.40x slower**  |   ❌   | Manual hash algorithm inefficiency |

#### Detailed C++ Results (Windows)

| Benchmark                       | Time    | CPU     | Iterations | Memory Usage |
| :------------------------------ | :------ | :------ | :--------- | :----------- |
| **Short Strings (400)**         |         |         |            |              |
| **Bcl_400**                     | 1.21 ns | 1.21 ns | 11.6B      | 0 KB         |
| **BclOrd_400**                  | 1.76 ns | 1.76 ns | 8.0B       | 0 KB         |
| **Larsson_400**                 | 2.37 ns | 2.36 ns | 5.9B       | 0 KB         |
| **Crc32Intrinsic_400**          | 2.13 ns | 2.13 ns | 6.5B       | 0 KB         |
| **Fnv_400**                     | 2.37 ns | 2.37 ns | 5.8B       | 0 KB         |
| **Longer Strings (H346.11112)** |         |         |            |              |
| **Bcl_H346_11112**              | 3.33 ns | 3.33 ns | 4.2B       | 18.0KB       |
| **BclOrd_H346_11112**           | 3.07 ns | 3.07 ns | 4.5B       | 0 KB         |
| **Larsson_H346_11112**          | 17.6 ns | 17.6 ns | 800M       | 0 KB         |
| **Crc32Intrinsic_H346_11112**   | 19.8 ns | 19.8 ns | 716M       | 0 KB         |
| **Fnv_H346_11112**              | 17.7 ns | 17.7 ns | 792M       | 0 KB         |

#### Detailed C# Results (Windows)

| Method                          | Input      | Mean     | Error     | StdDev    | Ratio        | Allocated |
| :------------------------------ | :--------- | :------- | :-------- | :-------- | :----------- | :-------- |
| **Short Strings (400)**         |            |          |           |           |              |           |
| **Bcl**                         | 400        | 1.156 ns | 0.0178 ns | 0.0149 ns | baseline     | -         |
| **BclOrd**                      | 400        | 1.536 ns | 0.0147 ns | 0.0137 ns | 1.33x slower | -         |
| **Larsson**                     | 400        | 1.225 ns | 0.0098 ns | 0.0087 ns | 1.06x slower | -         |
| **Crc32Intrinsic**              | 400        | 1.209 ns | 0.0068 ns | 0.0060 ns | 1.05x slower | -         |
| **Fnv**                         | 400        | 1.220 ns | 0.0300 ns | 0.0266 ns | 1.06x slower | -         |
| **Longer Strings (H346.11112)** |            |          |           |           |              |           |
| **Bcl**                         | H346.11112 | 4.521 ns | 0.0194 ns | 0.0172 ns | baseline     | -         |
| **BclOrd**                      | H346.11112 | 3.072 ns | 0.0299 ns | 0.0250 ns | 1.47x faster | -         |
| **Larsson**                     | H346.11112 | 3.405 ns | 0.0126 ns | 0.0118 ns | 1.33x faster | -         |
| **Crc32Intrinsic**              | H346.11112 | 3.274 ns | 0.0083 ns | 0.0073 ns | 1.38x faster | -         |
| **Fnv**                         | H346.11112 | 3.276 ns | 0.0165 ns | 0.0146 ns | 1.38x faster | -         |
