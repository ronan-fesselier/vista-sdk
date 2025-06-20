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
-   **RAM:** DDR4-3200 (32GB)
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

| Operation Category           | C++ vs C# Performance            | Status | Key Findings                                   |
| :--------------------------- | :------------------------------- | :----: | :--------------------------------------------- |
| **Hash Table Operations**    | **0.97-2.15x faster**            |   ✅   | C++ shows performance advantage                |
| **Codebook Access (Direct)** | **0.99x faster** vs C# baseline  |   ✅   | Performance parity                             |
| **Codebook Access (API)**    | **1,219x slower** vs C# baseline |   ⚠️   | Expected method call overhead                  |
| **String Hashing (Short)**   | **1.04-1.29x slower**            |   ✅   | C# implementation more efficient               |
| **String Hashing (Long)**    | **1.02x slower to 1.74x faster** |   ✅   | Mixed results, C++ advantage on longer strings |
| **GMOD Loading**             | **1.29x faster**                 |   ✅   | Good performance with less memory              |
| **GMOD Lookup**              | **1.21-2.15x faster**            |   ✅   | C++ shows performance advantage                |
| **GMOD Traversal**           | **1.73x slower**                 |   ❌   | **Optimization opportunity**                   |
| **Path Parsing**             | **9.38-20.5x slower**            |  ❌❌  | **Significant optimization needed**            |
| **Version Path Conversion**  | **162x slower**                  | ❌❌❌ | **Critical performance gap**                   |

---

## ChdDictionary

### CRC32 Hashing

Performance analysis of the CHD Dictionary hash function using CRC32 hardware acceleration across all Vista SDK codebook strings.

#### CRC32 Hash Function Performance (Windows)

| String                  | Length | Time    | Hash Rate     | ns/char | Performance   |
| :---------------------- | :----- | :------ | :------------ | :------ | :------------ |
| **Type**                | 4      | 1.24 ns | **809.0 M/s** | 0.310   | 🚀 Ultra-fast |
| **State**               | 5      | 1.32 ns | **757.7 M/s** | 0.264   | 🚀 Ultra-fast |
| **Detail**              | 6      | 1.31 ns | **762.9 M/s** | 0.218   | 🚀 Ultra-fast |
| **Content**             | 7      | 1.32 ns | **754.6 M/s** | 0.189   | 🚀 Ultra-fast |
| **Command**             | 7      | 1.34 ns | **744.2 M/s** | 0.191   | 🚀 Ultra-fast |
| **Quantity**            | 8      | 1.32 ns | **756.3 M/s** | 0.165   | 🚀 Ultra-fast |
| **Position**            | 8      | 1.31 ns | **764.9 M/s** | 0.164   | 🚀 Ultra-fast |
| **Calculation**         | 11     | 1.60 ns | **625.5 M/s** | 0.145   | 🚀 Ultra-fast |
| **ActivityType**        | 12     | 1.51 ns | **663.3 M/s** | 0.126   | 🚀 Ultra-fast |
| **FunctionalServices**  | 19     | 3.63 ns | **276.0 M/s** | 0.191   | 🔥 Very good  |
| **MaintenanceCategory** | 19     | 3.84 ns | **260.2 M/s** | 0.202   | 🔥 Very good  |

#### String Length Scaling Analysis

| Length | Time    | Hash Rate | ns/char | Efficiency    |
| :----- | :------ | :-------- | :------ | :------------ |
| 1      | 1.70 ns | 587.0 M/s | 1.704   | ⚡ Very fast  |
| 2      | 1.90 ns | 526.3 M/s | 0.950   | ⚡ Very fast  |
| 4      | 2.11 ns | 473.8 M/s | 0.528   | ⚡ Very fast  |
| 8      | 2.89 ns | 346.3 M/s | 0.361   | 🔥 Very good  |
| 16     | 4.05 ns | 246.6 M/s | 0.253   | 🔥 Good       |
| 32     | 7.17 ns | 139.7 M/s | 0.224   | ✅ Acceptable |

---

## Codebooks Lookup

### Cross-Platform Performance Comparison

Performance comparison between different C++ access methods for Vista SDK codebook operations:

| Operation             | Windows C++ | vs Baseline  | Linux C++ | vs Baseline | Notes                                   |
| :-------------------- | :---------- | :----------: | :-------- | :---------: | :-------------------------------------- |
| **SDK Instance**      | 0.688 ns    |  **1.00x**   | _TBD_     |    _TBD_    | 🚀 Ultra fast: fastest access           |
| **SDK Reference**     | 0.838 ns    |  **1.22x**   | _TBD_     |    _TBD_    | 🚀 Ultra fast                           |
| **Array Lookup**      | 1.62 ns     |  **2.35x**   | _TBD_     |    _TBD_    | ⚡ Fast baseline operation              |
| **Vector Lookup**     | 1.80 ns     |  **2.62x**   | _TBD_     |    _TBD_    | 🔥 Very fast linear search              |
| **Hash Table Lookup** | 2.11 ns     |  **3.07x**   | _TBD_     |    _TBD_    | 🔥 Excellent hash performance           |
| **Tree Lookup**       | 7.13 ns     |  **10.36x**  | _TBD_     |    _TBD_    | ⚠️ Red-black tree overhead              |
| **SDK API Method**    | 826 ns      |  **1,200x**  | _TBD_     |    _TBD_    | ⚠️ **Expected method call overhead**    |
| **VIS Singleton**     | 125,614 ns  | **182,574x** | _TBD_     |    _TBD_    | ⚠️ **Expected initialization overhead** |

#### STL Container Performance

| C++ Method       | Time    | Implementation                      | Performance vs Array | Status | Notes                           |
| :--------------- | :------ | :---------------------------------- | :------------------- | :----- | :------------------------------ |
| **Array**        | 1.62 ns | `std::array` linear search          | **Baseline**         | ✅     | ⚡ Fastest for 3 elements       |
| **Vector**       | 1.80 ns | `std::vector` linear search         | **1.11x slower**     | ✅     | 🔥 Minimal overhead             |
| **UnorderedMap** | 2.11 ns | `std::unordered_map::find()`        | **1.30x slower**     | ✅     | 🔥 Excellent hash performance   |
| **Map**          | 7.13 ns | `std::map::find()` (red-black tree) | **4.60x slower**     | ❌     | Tree overhead for small dataset |

#### Vista SDK Access Performance

| C++ Method             | Time           | Implementation                     | Performance vs Array | Status | Notes                                    |
| :--------------------- | :------------- | :--------------------------------- | :------------------- | :----- | :--------------------------------------- |
| **CodebooksInstance**  | **0.688 ns**   | Owned instance + direct access     | **2.35x faster**     | 🚀     | Ultra-fast: fastest method               |
| **CodebooksReference** | **0.838 ns**   | Reference wrapper + direct access  | **1.93x faster**     | 🚀     | Ultra-fast with minimal wrapper overhead |
| **CodebooksAPI**       | **826 ns**     | `codebook()` method call           | **1,200x slower**    | ⚠️     | Expected: bounds checking + validation   |
| **CodebooksVISCall**   | **125,614 ns** | `VIS::instance().codebooks()` call | **182,574x slower**  | ⚠️     | Expected: singleton + initialization     |

#### Detailed C++ Results (Windows)

| Benchmark                 | Time           | CPU            | Iterations | Performance Category       |
| :------------------------ | :------------- | :------------- | :--------- | :------------------------- |
| **BM_CodebooksInstance**  | **0.688 ns**   | **0.688 ns**   | 20.7B      | 🚀 **Ultra-fast**          |
| **BM_CodebooksReference** | **0.838 ns**   | **0.833 ns**   | 16.7B      | 🚀 **Ultra-fast**          |
| **BM_Array**              | **1.62 ns**    | **1.62 ns**    | 8.4B       | ⚡ **Fast**                |
| **BM_Vector**             | **1.80 ns**    | **1.80 ns**    | 7.6B       | ⚡ **Fast**                |
| **BM_UnorderedMap**       | **2.11 ns**    | **2.11 ns**    | 6.7B       | ⚡ **Fast**                |
| **BM_Map**                | **7.13 ns**    | **7.11 ns**    | 1.9B       | ⚠️ **Slow**                |
| **BM_CodebooksAPI**       | **826 ns**     | **826 ns**     | 16.9M      | ❌ **Expected: slow**      |
| **BM_CodebooksVISCall**   | **125,614 ns** | **125,558 ns** | 112K       | ❌ **Expected: very slow** |

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
| **GMOD Load** | 23.5 ms     |  **1.00x**  | _TBD_     |    _TBD_    | ⚡ Full GMOD construction |

### GMOD Load Performance (Windows)

| C++ Method   | C++ Time | C++ Implementation                  | C# Method | C# Time | C# Implementation               | Performance Ratio   |
| :----------- | :------- | :---------------------------------- | :-------- | :------ | :------------------------------ | :------------------ |
| **gmodLoad** | 23.5 ms  | `VIS::loadGmodDto()` + construction | **Load**  | 30.4 ms | `Gmod.Load()` with full parsing | ✅ **1.29x faster** |

#### Detailed C++ Results (Windows)

| Benchmark    | Time    | CPU     | Iterations |
| :----------- | :------ | :------ | :--------- |
| **gmodLoad** | 23.5 ms | 23.5 ms | 490        |

#### Detailed C# Results (Windows)

| Method   | Mean     | Error    | StdDev   | Gen0   | Gen1   | Gen2  | Allocated |
| :------- | :------- | :------- | :------- | :----- | :----- | :---- | :-------- |
| **Load** | 30.40 ms | 0.607 ms | 1.078 ms | 1562.5 | 1500.0 | 562.5 | 15.41 MB  |

---

## GMOD Lookup

| Operation             | Windows C++ | vs Baseline | Linux C++ | vs Baseline | Notes                            |
| :-------------------- | :---------- | :---------: | :-------- | :---------: | :------------------------------- |
| **Frozen Dictionary** | 18.4 ns     |  **1.00x**  | _TBD_     |    _TBD_    | ⚡ Baseline lookup method        |
| **Hash Table Lookup** | 17.8 ns     |  **0.97x**  | _TBD_     |    _TBD_    | 🚀 Slightly faster               |
| **GMOD API Lookup**   | 22.2 ns     |  **1.21x**  | _TBD_     |    _TBD_    | ✅ Expected method call overhead |

### GMOD Lookup Performance (Windows)

| C++ Method     | C++ Time | C++ Implementation            | C# Method      | C# Time  | C# Implementation                | Performance Ratio   |
| :------------- | :------- | :---------------------------- | :------------- | :------- | :------------------------------- | :------------------ |
| **dict**       | 17.8 ns  | `std::unordered_map::find()`  | **Dict**       | 38.34 ns | `Dictionary.TryGetValue()`       | ✅ **2.15x faster** |
| **frozenDict** | 18.4 ns  | `std::unordered_map` (frozen) | **FrozenDict** | 15.21 ns | `FrozenDictionary.TryGetValue()` | ❌ **1.21x slower** |
| **gmod**       | 22.2 ns  | CHD implementation lookup     | **Gmod**       | 15.62 ns | Native GMOD API access           | ❌ **1.42x slower** |

#### Detailed C++ Results (Windows)

| Benchmark      | Time    | CPU     | Iterations |
| :------------- | :------ | :------ | :--------- |
| **Dict**       | 17.8 ns | 17.9 ns | 786M       |
| **FrozenDict** | 18.4 ns | 18.4 ns | 766M       |
| **Gmod**       | 22.2 ns | 22.1 ns | 627M       |

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
| **TryParse**                       | 31.5 μs     | _TBD_     |        |       |
| **TryParseFullPath**               | 8.67 μs     | _TBD_     |        |       |
| **TryParseIndividualized**         | 30.5 μs     | _TBD_     |        |       |
| **TryParseFullPathIndividualized** | 8.72 μs     | _TBD_     |        |       |

### GMOD Path Parsing Performance (Windows)

Performance comparison between C++ and C# implementations for GMOD path parsing operations:

| Operation                          | C++ (Google Benchmark) | C# (BenchmarkDotNet) | Performance Ratio | Status | Notes                      |
| :--------------------------------- | :--------------------- | :------------------- | :---------------- | :----: | :------------------------- |
| **TryParse**                       | 31.5 μs                | 3.36 μs              | **9.38x slower**  |  ❌❌  | Critical performance issue |
| **TryParseFullPath**               | 8.67 μs                | 571 ns               | **15.2x slower**  |  ❌❌  | Critical performance issue |
| **TryParseIndividualized**         | 30.5 μs                | 1.49 μs              | **20.5x slower**  |  ❌❌  | Critical performance issue |
| **TryParseFullPathIndividualized** | 8.72 μs                | 694 ns               | **12.6x slower**  |  ❌❌  | Critical performance issue |

#### Detailed C++ Results (Windows)

| Benchmark                          | Time    | CPU     | Iterations |
| :--------------------------------- | :------ | :------ | :--------- |
| **tryParse**                       | 31.5 μs | 31.6 μs | 448K       |
| **tryParseFullPath**               | 8.67 μs | 8.67 μs | 1.61M      |
| **tryParseIndividualized**         | 30.5 μs | 30.5 μs | 459K       |
| **tryParseFullPathIndividualized** | 8.72 μs | 8.71 μs | 1.61M      |

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
| **Full Traversal** | 282 ms      | _TBD_     |        |       |

### GMOD Traversal Performance (Windows)

Performance comparison between C++ and C# implementations for GMOD node lookup operations:

| Operation          | C++ (Google Benchmark) | C# (BenchmarkDotNet) | Performance Ratio | Status | Notes                                |
| :----------------- | :--------------------- | :------------------- | :---------------- | :----: | :----------------------------------- |
| **Full Traversal** | 282 ms                 | 162.9 ms             | **1.73x slower**  |   ❌   | Significant optimization opportunity |

#### Detailed C++ Results (Windows)

| Benchmark         | Time   | CPU    | Iterations |
| :---------------- | :----- | :----- | :--------- |
| **FullTraversal** | 282 ms | 266 ms | 50-51      |

#### Detailed C# Results (Windows)

| Method            | Mean     | Error   | StdDev  | Allocated |
| :---------------- | :------- | :------ | :------ | :-------- |
| **FullTraversal** | 162.9 ms | 0.97 ms | 0.91 ms | 5.3 KB    |

---

## GMOD Versioning Path Conversion

### Cross-Platform Performance Comparison

| Operation        | Windows C++ | Linux C++ | Status | Notes |
| :--------------- | :---------- | :-------- | :----: | :---- |
| **Convert Path** | 241 μs      | _TBD_     |        |       |

### GMOD Versioning Path Conversion Performance (Windows)

Performance comparison between C++ and C# implementations for GMOD version path conversion operations:

| Operation        | C++ (Google Benchmark) | C# (BenchmarkDotNet) | Performance Ratio | Status | Notes            |
| :--------------- | :--------------------- | :------------------- | :---------------- | :----: | :--------------- |
| **Convert Path** | 241 μs                 | 1.489 μs             | **162x slower**   | ❌❌❌ | **CATASTROPHIC** |

#### Detailed C++ Results (Windows)

| Benchmark       | Time   | CPU    | Iterations |
| :-------------- | :----- | :----- | :--------- |
| **convertPath** | 241 μs | 219 μs | 64,000     |

#### Detailed C# Results (Windows)

| Method          | Mean     | Error     | StdDev    | Gen0   | Allocated |
| :-------------- | :------- | :-------- | :-------- | :----- | :-------- |
| **ConvertPath** | 1.489 μs | 0.0108 μs | 0.0090 μs | 0.2575 | 3.17 KB   |

---

## Short String Hashing

### Cross-Platform Performance Comparison

| Operation         | Windows C++ | Linux C++ | Status | Notes |
| :---------------- | :---------- | :-------- | :----: | :---- |
| **bcl (400)**     | 1.18 ns     | _TBD_     |        |       |
| **bcl (H346)**    | 2.61 ns     | _TBD_     |        |       |
| **bclOrd (400)**  | 1.77 ns     | _TBD_     |        |       |
| **bclOrd (H346)** | 2.92 ns     | _TBD_     |        |       |
| **Larson (400)**  | 1.54 ns     | _TBD_     |        |       |
| **Larson (H346)** | 4.85 ns     | _TBD_     |        |       |
| **crc32 (400)**   | 1.57 ns     | _TBD_     |        |       |
| **crc32 (H346)**  | 4.51 ns     | _TBD_     |        |       |
| **fnv (400)**     | 1.48 ns     | _TBD_     |        |       |
| **fnv (H346)**    | 3.39 ns     | _TBD_     |        |       |

### Short String Hashing Performance (Windows)

Performance comparison between C++ and C# implementations for hash function operations:

#### String "400" (3 characters)

| Algorithm          | C++ (Google Benchmark) | C# (BenchmarkDotNet) | Performance Ratio | Status | Notes |
| :----------------- | :--------------------- | :------------------- | :---------------- | :----: | :---- |
| **Bcl**            | 1.18 ns                | 1.135 ns             | **1.04x slower**  |        |       |
| **BclOrd**         | 1.77 ns                | 1.514 ns             | **1.17x slower**  |        |       |
| **Larson**         | 1.54 ns                | 1.219 ns             | **1.26x slower**  |        |       |
| **Crc32Intrinsic** | 1.57 ns                | 1.215 ns             | **1.29x slower**  |        |       |
| **Fnv**            | 1.48 ns                | 1.205 ns             | **1.23x slower**  |        |       |

#### String "H346.11112" (10 characters)

| Algorithm          | C++ (Google Benchmark) | C# (BenchmarkDotNet) | Performance Ratio | Status | Notes |
| :----------------- | :--------------------- | :------------------- | :---------------- | :----: | :---- |
| **Bcl**            | 2.61 ns                | 4.551 ns             | **1.74x faster**  |        |       |
| **BclOrd**         | 2.92 ns                | 3.127 ns             | **1.07x faster**  |        |       |
| **Larson**         | 4.85 ns                | 3.349 ns             | **1.45x slower**  |        |       |
| **Crc32Intrinsic** | 4.51 ns                | 3.259 ns             | **1.38x slower**  |        |       |
| **Fnv**            | 3.39 ns                | 3.337 ns             | **1.02x slower**  |        |       |

#### Detailed C++ Results (Windows)

| Benchmark                       | Input      | Time    | CPU     | Iterations |
| :------------------------------ | :--------- | :------ | :------ | :--------- |
| **short strings (400)**         |            |         |         |            |
| bcl_400                         | 400        | 1.55 ns | 1.55 ns | 8.6B       |
| bclOrd_400                      | 400        | 1.79 ns | 1.79 ns | 7.8B       |
| Larson_400                      | 400        | 1.70 ns | 1.70 ns | 8.4B       |
| crc32Intrinsic_400              | 400        | 1.51 ns | 1.51 ns | 9.4B       |
| fnv_400                         | 400        | 1.65 ns | 1.66 ns | 8.4B       |
| **longer strings (H346.11112)** |            |         |         |            |
| bcl_H346_11112                  | H346.11112 | 3.76 ns | 3.75 ns | 4.5B       |
| bclOrd_H346_11112               | H346.11112 | 3.05 ns | 3.05 ns | 4.6B       |
| Larson_H346_11112               | H346.11112 | 4.61 ns | 4.61 ns | 3.0B       |
| crc32Intrinsic_H346_11112       | H346.11112 | 4.34 ns | 4.33 ns | 3.2B       |
| fnv_H346_11112                  | H346.11112 | 3.26 ns | 3.25 ns | 4.3B       |

#### Detailed C# Results (Windows)

| Benchmark                 | Input      | Time    | CPU     | Iterations |
| :------------------------ | :--------- | :------ | :------ | :--------- |
| **short strings (400)**   |            |         |         |            |
| bcl_400                   | 400        | 1.18 ns | 1.18 ns | 12.1B      |
| bclOrd_400                | 400        | 1.77 ns | 1.77 ns | 8.0B       |
| Larson_400                | 400        | 1.54 ns | 1.54 ns | 9.1B       |
| crc32Intrinsic_400        | 400        | 1.57 ns | 1.56 ns | 8.9B       |
| fnv_400                   | 400        | 1.48 ns | 1.48 ns | 9.3B       |
| bcl_H346_11112            | H346.11112 | 2.61 ns | 2.61 ns | 5.4B       |
| bclOrd_H346_11112         | H346.11112 | 2.92 ns | 2.92 ns | 4.7B       |
| Larson_H346_11112         | H346.11112 | 4.85 ns | 4.83 ns | 2.9B       |
| crc32Intrinsic_H346_11112 | H346.11112 | 4.51 ns | 4.51 ns | 3.1B       |
| fnv_H346_11112            | H346.11112 | 3.39 ns | 3.39 ns | 4.1B       |

---

_Last updated: June 21, 2025_
