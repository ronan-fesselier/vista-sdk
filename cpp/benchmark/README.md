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

**C# Benchmarks:**

```bash
# Windows & Linux
dotnet run -c Release --project benchmark/Vista.SDK.Benchmarks/Vista.SDK.Benchmarks.csproj
```

---

## Test Environment

**Hardware Configuration:**

-   **CPU:** 12th Gen Intel Core i7-12800H (20 logical cores, 14 physical cores) @ 2.80 GHz (base) / 4.80 GHz (turbo)
-   **CPU Cache:**
    -   L1 Data: 48 KiB (x10 cores)
    -   L1 Instruction: 32 KiB (x10 cores)
    -   L2 Unified: 1280 KiB (x10 cores)
    -   L3 Unified: 24576 KiB (x1 shared)
-   **RAM:** DDR4-3200 (64GB)
-   **GPU:** NVIDIA RTX A2000 8GB GDDR6

**Software Configuration:**

| Platform    | Benchmark Framework     | C++ Compiler | C# Runtime               | OS Version                              |
| :---------- | :---------------------- | :----------- | :----------------------- | :-------------------------------------- |
| **Windows** | Google Benchmark v1.9.4 | MSVC 2022    | .NET 8.0.16, RyuJIT AVX2 | Windows 10                              |
| **Linux**   | Google Benchmark v1.9.4 | Clang 16.0.6 | .NET 8.0.17, RyuJIT AVX2 | LMDE 6 (faye) / Debian 6.1.140-1 x86_64 |

---

## Executive Summary

### Windows Platform Performance

| Operation Category           | C++ vs C# Performance            | Status | Key Findings                        |
| :--------------------------- | :------------------------------- | :----: | :---------------------------------- |
| **Hash Table Operations**    | **0.97-2.15x faster**            |   ✅   | C++ shows performance advantage     |
| **Codebook Access (Direct)** | **7.07x faster** vs C# baseline  |   ✅   | Strong C++ performance advantage    |
| **Codebook Access (API)**    | **1,408x slower** vs C# baseline |   ⚠️   | Expected method call overhead       |
| **String Hashing (Short)**   | **1.24-1.74x mixed**             |   ✅   | Mixed results, algorithm dependent  |
| **String Hashing (Long)**    | **1.15x slower to 1.30x faster** |   ✅   | C++ advantage on longer strings     |
| **GMOD Loading**             | **1.05x slower**                 |   ⚠️   | Minor performance gap               |
| **GMOD Lookup**              | **1.21-2.15x faster**            |   ✅   | C++ shows performance advantage     |
| **GMOD Traversal**           | **2.08x slower**                 |   ❌   | **Optimization opportunity**        |
| **Path Parsing**             | **8.36-11.0x slower**            |  ❌❌  | **Significant optimization needed** |
| **Version Path Conversion**  | **162x slower**                  | ❌❌❌ | **Critical performance gap**        |

### Linux Platform Performance

| Operation Category           | C++ vs C# Performance            | Status | Key Findings                                      |
| :--------------------------- | :------------------------------- | :----: | :------------------------------------------------ |
| **Hash Table Operations**    | **1.20-3.37x faster**            |   ✅   | Strong C++ advantage on Linux                     |
| **Codebook Access (Direct)** | **7.73x faster** vs C# baseline  |   ✅   | Excellent Linux performance boost                 |
| **Codebook Access (API)**    | **330x slower** vs C# baseline   |   ⚠️   | Much better than Windows, still expected overhead |
| **String Hashing (Short)**   | **1.03-2.40x mixed**             |   ✅   | Mixed results, some algorithms competitive        |
| **String Hashing (Long)**    | **1.02x slower to 1.81x faster** |   ✅   | Generally better performance than Windows         |
| **GMOD Loading**             | **2.43x faster**                 |   ✅   | Excellent performance advantage                   |
| **GMOD Lookup**              | **1.20-1.33x slower**            |   ❌   | Regression vs Windows, mixed vs C#                |
| **GMOD Traversal**           | **1.97x slower**                 |   ❌   | **Worse than Windows, needs optimization**        |
| **Path Parsing**             | **4.35-9.26x slower**            |  ❌❌  | **Improved but still significant gap**            |
| **Version Path Conversion**  | **117x slower**                  | ❌❌❌ | **Better than Windows but still critical**        |

### Cross-Platform Summary

| Operation Category           | Linux vs Windows (C++) | Status | Platform Recommendation                  |
| :--------------------------- | :--------------------- | :----: | :--------------------------------------- |
| **Hash Table Operations**    | **1.13-11.9x faster**  |   ✅   | **Linux strongly preferred**             |
| **Codebook Access (Direct)** | **1.47-3.99x faster**  |   ✅   | **Linux significantly better**           |
| **Codebook Access (API)**    | **11.9x faster**       |   ✅   | **Linux dramatically better**            |
| **String Hashing (Short)**   | **1.04-2.31x mixed**   |   ⚠️   | **Algorithm-dependent performance**      |
| **String Hashing (Long)**    | **1.12-1.32x faster**  |   ✅   | **Linux slightly better**                |
| **GMOD Loading**             | **1.88x faster**       |   ✅   | **Linux preferred**                      |
| **GMOD Lookup**              | **1.71-2.87x mixed**   |   ⚠️   | **Mixed results, platform-specific**     |
| **GMOD Traversal**           | **1.13x slower**       |   ❌   | **Windows slightly better**              |
| **Path Parsing**             | **2.01-2.21x faster**  |   ✅   | **Linux improvement but both need work** |
| **Version Path Conversion**  | **1.40x faster**       |   ✅   | **Linux better but both critical**       |

---

## Codebooks Lookup

### Cross-Platform Performance Comparison

Performance comparison between different C++ access methods for Vista SDK codebook operations:

| Operation             | Windows C++ | vs Baseline  | Linux C++ | vs Baseline  | Cross-Platform   | Notes |
| :-------------------- | :---------- | :----------: | :-------- | :----------: | :--------------- | :---- |
| **SDK Instance**      | 0.728 ns    |  **1.00x**   | 0.210 ns  |  **1.00x**   | **3.47x faster** |       |
| **SDK Reference**     | 0.872 ns    |  **1.20x**   | 0.210 ns  |  **1.00x**   | **4.15x faster** |       |
| **Array Lookup**      | 1.67 ns     |  **2.29x**   | 0.645 ns  |  **3.07x**   | **2.59x faster** |       |
| **Vector Lookup**     | 2.04 ns     |  **2.80x**   | 1.60 ns   |  **7.62x**   | **1.28x faster** |       |
| **Hash Table Lookup** | 2.19 ns     |  **3.01x**   | 1.40 ns   |  **6.67x**   | **1.56x faster** |       |
| **CHD Dictionary**    | 27.2 ns     |  **37.4x**   | TBD       |   **TBD**    | **TBD**          |       |
| **Tree Lookup**       | 8.25 ns     |  **11.3x**   | 2.82 ns   |  **13.43x**  | **2.93x faster** |       |
| **SDK API Method**    | 917 ns      |  **1,259x**  | 69.4 ns   |   **330x**   | **13.2x faster** |       |
| **VIS Singleton**     | 125,460 ns  | **172,309x** | 85,536 ns | **407,314x** | **1.47x faster** |       |

#### STL Container Performance

| C++ Method        | Windows Time | Linux Time | Performance Ratio | Implementation                      | Performance vs Array | Status | Notes |
| :---------------- | :----------- | :--------- | :---------------- | :---------------------------------- | :------------------- | :----- | :---- |
| **Array**         | 1.67 ns      | 0.645 ns   | **2.59x faster**  | `std::array` linear search          | **Baseline**         | ✅     |       |
| **Vector**        | 2.04 ns      | 1.60 ns    | **1.28x faster**  | `std::vector` linear search         | **1.22x slower**     | ✅     |       |
| **UnorderedMap**  | 2.19 ns      | 1.40 ns    | **1.56x faster**  | `std::unordered_map::find()`        | **1.31x slower**     | ✅     |       |
| **ChdDictionary** | 27.2 ns      | TBD        | **TBD**           | CHD perfect hash table              | **16.3x slower**     | ✅     |       |
| **Map**           | 8.25 ns      | 2.82 ns    | **2.93x faster**  | `std::map::find()` (red-black tree) | **4.94x slower**     | ⚠️     |       |

#### Vista SDK Access Performance

| C++ Method             | Windows Time   | Linux Time    | Performance Ratio | Implementation                     | Performance vs Array | Status | Notes |
| :--------------------- | :------------- | :------------ | :---------------- | :--------------------------------- | :------------------- | :----- | :---- |
| **CodebooksInstance**  | **0.728 ns**   | **0.210 ns**  | **3.47x faster**  | Owned instance + direct access     | **2.29x faster**     | 🚀     |       |
| **CodebooksReference** | **0.872 ns**   | **0.210 ns**  | **4.15x faster**  | Reference wrapper + direct access  | **1.91x faster**     | 🚀     |       |
| **CodebooksAPI**       | **917 ns**     | **69.4 ns**   | **13.2x faster**  | `codebook()` method call           | **549x slower**      | ⚠️     |       |
| **CodebooksVISCall**   | **125,460 ns** | **85,536 ns** | **1.47x faster**  | `VIS::instance().codebooks()` call | **75,122x slower**   | ⚠️     |       |

#### Detailed C++ Results

##### Windows Platform

| Benchmark                 | Time           | CPU            | Iterations | Performance Category |
| :------------------------ | :------------- | :------------- | :--------- | :------------------- |
| **BM_CodebooksInstance**  | **0.728 ns**   | **0.728 ns**   | 18.4B      |                      |
| **BM_CodebooksReference** | **0.872 ns**   | **0.872 ns**   | 16.0B      |                      |
| **BM_Array**              | **1.67 ns**    | **1.66 ns**    | 8.53B      |                      |
| **BM_Vector**             | **2.04 ns**    | **2.04 ns**    | 7.00B      |                      |
| **BM_UnorderedMap**       | **2.19 ns**    | **2.19 ns**    | 6.45B      |                      |
| **BM_ChdDictionary**      | **27.2 ns**    | **27.2 ns**    | 506M       |                      |
| **BM_Map**                | **8.25 ns**    | **8.25 ns**    | 1.74B      |                      |
| **BM_CodebooksAPI**       | **917 ns**     | **916 ns**     | 14.7M      |                      |
| **BM_CodebooksVISCall**   | **125,460 ns** | **125,150 ns** | 111K       |                      |

##### Linux Platform

| Benchmark                 | Time          | CPU           | Iterations | Performance Category |
| :------------------------ | :------------ | :------------ | :--------- | :------------------- |
| **BM_CodebooksInstance**  | **0.210 ns**  | **0.210 ns**  | 66.8B      |                      |
| **BM_CodebooksReference** | **0.210 ns**  | **0.210 ns**  | 66.6B      |                      |
| **BM_Array**              | **0.645 ns**  | **0.644 ns**  | 21.8B      |                      |
| **BM_Vector**             | **1.60 ns**   | **1.60 ns**   | 8.9B       |                      |
| **BM_UnorderedMap**       | **1.40 ns**   | **1.40 ns**   | 10.0B      |                      |
| **BM_ChdDictionary**      |               |               |            |                      |
| **BM_Map**                | **2.82 ns**   | **2.82 ns**   | 5.0B       |                      |
| **BM_CodebooksAPI**       | **69.4 ns**   | **69.4 ns**   | 202M       |                      |
| **BM_CodebooksVISCall**   | **85,536 ns** | **85,529 ns** | 168K       |                      |

#### Detailed C# Results (Windows)

| Method     |      Mean |     Error |    StdDev |        Ratio | RatioSD | Rank | Allocated | Alloc Ratio |
| ---------- | --------: | --------: | --------: | -----------: | ------: | ---: | --------: | ----------: |
| FrozenDict | 4.9802 ns | 0.0699 ns | 0.0584 ns | 1.02x slower |   0.01x |    3 |         - |          NA |
| Dict       | 4.8993 ns | 0.0239 ns | 0.0212 ns |     baseline |         |    2 |         - |          NA |
| Codebooks  | 0.6937 ns | 0.0219 ns | 0.0195 ns | 7.07x faster |   0.21x |    1 |         - |          NA |

#### Detailed C# Results (Linux)

| Method         | Mean    | Error    | StdDev   | Ratio        | RatioSD | Rank | Allocated | Alloc Ratio |
| :------------- | :------ | :------- | :------- | :----------- | :------ | :--- | :-------- | :---------- |
| **FrozenDict** | 5.04 ns | 0.011 ns | 0.009 ns | 1.12x slower | 0.00x   | 3    | -         | NA          |
| **Dict**       | 4.49 ns | 0.013 ns | 0.011 ns | baseline     |         | 2    | -         | NA          |
| **Codebooks**  | 0.58 ns | 0.021 ns | 0.018 ns | 7.73x faster | 0.22x   | 1    | -         | NA          |

---

## GMOD Load

### Cross-Platform Performance Comparison

| Operation     | Windows C++ | vs Baseline | Linux C++ | vs Baseline | Cross-Platform   | Notes                     |
| :------------ | :---------- | :---------: | :-------- | :---------: | :--------------- | :------------------------ |
| **GMOD Load** | 36.8 ms     |  **1.00x**  | 12.5 ms   |  **1.00x**  | **2.94x faster** | ⚡ Full GMOD construction |

### GMOD Load Performance Comparison

| C++ Method   | Windows Time | Linux Time | Performance Ratio | C# Method | C# Time (Win) | C# Time (Linux) | C++ vs C# (Windows) | C++ vs C# (Linux)   |
| :----------- | :----------- | :--------- | :---------------- | :-------- | :------------ | :-------------- | :------------------ | :------------------ |
| **gmodLoad** | 36.8 ms      | 12.5 ms    | **2.94x faster**  | **Load**  | 30.40 ms      | 29.68 ms        | ❌ **1.21x slower** | ✅ **2.38x faster** |

#### Detailed C++ Results

##### Windows Platform

| Benchmark    | Time    | CPU     | Iterations |
| :----------- | :------ | :------ | :--------- |
| **gmodLoad** | 36.8 ms | 36.8 ms | 320        |

##### Linux Platform

| Benchmark    | Time    | CPU     | Iterations |
| :----------- | :------ | :------ | :--------- |
| **gmodLoad** | 12.5 ms | 12.5 ms | 1,029      |

#### Detailed C# Results (Windows)

| Method   | Mean     | Error    | StdDev   | Gen0   | Gen1   | Gen2  | Allocated |
| :------- | :------- | :------- | :------- | :----- | :----- | :---- | :-------- |
| **Load** | 30.40 ms | 0.607 ms | 1.078 ms | 1562.5 | 1500.0 | 562.5 | 15.41 MB  |

#### Detailed C# Results (Linux)

| Method   |     Mean |    Error |   StdDev | Rank |      Gen0 |      Gen1 |     Gen2 | Allocated |
| -------- | -------: | -------: | -------: | ---: | --------: | --------: | -------: | --------: |
| **Load** | 29.68 ms | 0.200 ms | 0.187 ms |    1 | 1593.7500 | 1562.5000 | 625.0000 |  15.41 MB |

---

## GMOD Lookup

### Cross-Platform Performance Comparison

| Operation             | Windows C++ | vs Baseline | Linux C++ | vs Baseline | Cross-Platform   | Notes                            |
| :-------------------- | :---------- | :---------: | :-------- | :---------: | :--------------- | :------------------------------- |
| **Frozen Dictionary** | 17.0 ns     |  **1.00x**  | 51.2 ns   |  **1.00x**  | **3.01x slower** | ⚡ Baseline lookup method        |
| **Hash Table Lookup** | 18.1 ns     |  **1.06x**  | 51.1 ns   |  **1.00x**  | **2.82x slower** | ⚡ Similar                       |
| **GMOD API Lookup**   | 32.3 ns     |  **1.90x**  | 13.0 ns   |  **0.25x**  | **2.48x faster** | ✅ Expected method call overhead |

### GMOD Lookup Performance Comparison

#### Cross-Platform C++ Results

| C++ Method     | Windows Time | Linux Time | Performance Ratio | C# Method (Win) | C# Time (Win) | C# Method (Linux) | C# Time (Linux) | C++ vs C# (Windows) | C++ vs C# (Linux)   |
| :------------- | :----------- | :--------- | :---------------- | :-------------- | :------------ | :---------------- | :-------------- | :------------------ | :------------------ |
| **dict**       | 18.1 ns      | 51.1 ns    | **2.82x slower**  | **Dict**        | 38.34 ns      | **Dict**          | 37.85 ns        | ✅ **2.12x faster** | ❌ **1.35x slower** |
| **frozenDict** | 17.0 ns      | 51.2 ns    | **3.01x slower**  | **FrozenDict**  | 15.21 ns      | **FrozenDict**    | 15.81 ns        | ❌ **1.12x slower** | ❌ **3.24x slower** |
| **gmod**       | 32.3 ns      | 13.0 ns    | **2.48x faster**  | **Gmod**        | 15.62 ns      | **Gmod**          | 17.07 ns        | ❌ **2.07x slower** | ✅ **1.31x faster** |

#### Detailed C++ Results

##### Windows Platform

| Benchmark      | Time    | CPU     | Iterations |
| :------------- | :------ | :------ | :--------- |
| **Dict**       | 18.1 ns | 18.1 ns | 689M       |
| **FrozenDict** | 17.0 ns | 17.0 ns | 815M       |
| **Gmod**       | 32.3 ns | 32.3 ns | 439M       |

##### Linux Platform

| Benchmark      | Time    | CPU     | Iterations |
| :------------- | :------ | :------ | :--------- |
| **Dict**       | 51.1 ns | 51.1 ns | 275M       |
| **FrozenDict** | 51.2 ns | 51.2 ns | 274M       |
| **Gmod**       | 13.0 ns | 13.0 ns | 1.08B      |

#### Detailed C# Results (Windows)

| Method         | Mean     | Error    | StdDev   | Ratio        | RatioSD | Rank | Allocated | Alloc Ratio |
| :------------- | :------- | :------- | :------- | :----------- | :------ | :--- | :-------- | :---------- |
| **Dict**       | 38.34 ns | 0.227 ns | 0.201 ns | baseline     |         | 3    | -         | NA          |
| **FrozenDict** | 15.21 ns | 0.039 ns | 0.035 ns | 2.52x faster | 0.02x   | 1    | -         | NA          |
| **Gmod**       | 15.62 ns | 0.255 ns | 0.226 ns | 2.46x faster | 0.04x   | 2    | -         | NA          |

#### Detailed C# Results (Linux)

| Method         | Mean     | Error    | StdDev   | Ratio        | RatioSD | Rank | Allocated | Alloc Ratio |
| :------------- | :------- | :------- | :------- | :----------- | :------ | :--- | :-------- | :---------- |
| **Dict**       | 37.85 ns | 0.366 ns | 0.343 ns | baseline     |         | 3    | -         | NA          |
| **Gmod**       | 17.07 ns | 0.210 ns | 0.186 ns | 2.22x faster | 0.03x   | 2    | -         | NA          |
| **FrozenDict** | 15.81 ns | 0.348 ns | 0.401 ns | 2.40x faster | 0.06x   | 1    | -         | NA          |

---

## GMOD Path Parsing

### Cross-Platform Performance Comparison

| Operation                          | Windows C++ | Linux C++ | Performance Ratio | Status | Notes |
| :--------------------------------- | :---------- | :-------- | :---------------- | :----: | :---- |
| **TryParse**                       | 36.6 μs     | 14.6 μs   | **2.51x slower**  |   ✅   |       |
| **TryParseFullPath**               | 9.37 μs     | 4.31 μs   | **2.17x slower**  |   ✅   |       |
| **TryParseIndividualized**         | 36.6 μs     | 13.8 μs   | **2.65x slower**  |   ✅   |       |
| **TryParseFullPathIndividualized** | 9.35 μs     | 3.99 μs   | **2.34x slower**  |   ✅   |       |

### GMOD Path Parsing Performance Comparison

#### Cross-Platform C++ Results

| C++ Method                         | Windows Time | Linux Time | Performance Ratio | C# Method (Win)                    | C# Time (Win) | C# Method (Linux)                  | C# Time (Linux) | C++ vs C# (Windows) | C++ vs C# (Linux)   |
| :--------------------------------- | :----------- | :--------- | :---------------- | :--------------------------------- | :------------ | :--------------------------------- | :-------------- | :------------------ | :------------------ |
| **tryParse**                       | 36.6 μs      | 14.6 μs    | **2.51x slower**  | **TryParse**                       | 3.77 μs       | **TryParse**                       | 3.55 μs         | ❌ **9.71x slower** | ❌ **4.11x slower** |
| **tryParseFullPath**               | 9.37 μs      | 4.31 μs    | **2.17x slower**  | **TryParseFullPath**               | 571 ns        | **TryParseFullPath**               | 630 ns          | ❌ **16.4x slower** | ❌ **6.84x slower** |
| **tryParseIndividualized**         | 36.6 μs      | 13.8 μs    | **2.65x slower**  | **TryParseIndividualized**         | 1.49 μs       | **TryParseIndividualized**         | 1.60 μs         | ❌ **24.6x slower** | ❌ **8.63x slower** |
| **tryParseFullPathIndividualized** | 9.35 μs      | 3.99 μs    | **2.34x slower**  | **TryParseFullPathIndividualized** | 694 ns        | **TryParseFullPathIndividualized** | 788 ns          | ❌ **13.5x slower** | ❌ **5.06x slower** |

#### Detailed C++ Results

##### Windows Platform

| Benchmark                          | Time    | CPU     | Iterations |
| :--------------------------------- | :------ | :------ | :--------- |
| **tryParse**                       | 36.6 μs | 36.5 μs | 393K       |
| **tryParseFullPath**               | 9.37 μs | 9.37 μs | 1.52M      |
| **tryParseIndividualized**         | 36.6 μs | 36.6 μs | 385K       |
| **tryParseFullPathIndividualized** | 9.35 μs | 9.33 μs | 1.49M      |

##### Linux Platform

| Benchmark                          | Time    | CPU     | Iterations |
| :--------------------------------- | :------ | :------ | :--------- |
| **tryParse**                       | 14.6 μs | 14.6 μs | 942K       |
| **tryParseFullPath**               | 4.31 μs | 4.31 μs | 3.26M      |
| **tryParseIndividualized**         | 13.8 μs | 13.8 μs | 1.02M      |
| **tryParseFullPathIndividualized** | 3.99 μs | 3.99 μs | 3.53M      |

#### Detailed C# Results (Windows)

| Method                             | Mean       | Error    | StdDev   | Categories    | Gen0   | Allocated |
| :--------------------------------- | :--------- | :------- | :------- | :------------ | :----- | :-------- |
| **TryParse**                       | 3,360.8 ns | 41.66 ns | 38.97 ns | No location   | 0.2213 | 2,792 B   |
| **TryParseFullPath**               | 571.2 ns   | 7.45 ns  | 5.82 ns  | No location   | 0.0601 | 760 B     |
| **TryParseIndividualized**         | 1,492.2 ns | 28.77 ns | 51.14 ns | With location | 0.2251 | 2,832 B   |
| **TryParseFullPathIndividualized** | 694.1 ns   | 4.33 ns  | 3.84 ns  | With location | 0.0935 | 1,176 B   |

#### Detailed C# Results (Linux)

| Method                             | Categories    |       Mean |    Error |   StdDev |   Gen0 | Allocated |
| ---------------------------------- | ------------- | ---------: | -------: | -------: | -----: | --------: |
| **TryParseFullPath**               | No location   |   630.4 ns |  2.53 ns |  2.11 ns | 0.0601 |     760 B |
| **TryParse**                       | No location   | 3,548.7 ns | 14.72 ns | 11.49 ns | 0.2213 |    2792 B |
| **TryParseFullPathIndividualized** | With location |   787.9 ns |  2.30 ns |  1.92 ns | 0.0935 |    1176 B |
| **TryParseIndividualized**         | With location | 1,598.6 ns | 13.64 ns | 12.76 ns | 0.2251 |    2832 B |

---

## GMOD Traversal

### Cross-Platform Performance Comparison

| Operation          | Windows C++ | Linux C++ | Performance Ratio | Status | Notes |
| :----------------- | :---------- | :-------- | :---------------- | :----: | :---- |
| **Full Traversal** | 297 ms      | 318 ms    | **1.07x slower**  |   ✅   |       |

### GMOD Traversal Performance Comparison

#### Cross-Platform C++ Results

| C++ Method        | Windows Time | Linux Time | Performance Ratio | C# Method (Win)   | C# Time (Win) | C# Method (Linux) | C# Time (Linux) | C++ vs C# (Windows) | C++ vs C# (Linux)   |
| :---------------- | :----------- | :--------- | :---------------- | :---------------- | :------------ | :---------------- | :-------------- | :------------------ | :------------------ |
| **fullTraversal** | 297 ms       | 318 ms     | **1.07x slower**  | **FullTraversal** | 135.3 ms      | **FullTraversal** | 161.3 ms        | ❌ **2.20x slower** | ❌ **1.97x slower** |

#### Detailed C++ Results

##### Windows Platform

| Benchmark         | Time   | CPU    | Iterations |
| :---------------- | :----- | :----- | :--------- |
| **FullTraversal** | 297 ms | 294 ms | 48         |

##### Linux Platform

| Benchmark         | Time   | CPU    | Iterations |
| :---------------- | :----- | :----- | :--------- |
| **FullTraversal** | 318 ms | 318 ms | 44         |

#### Detailed C# Results (Windows)

| Method            | Mean     | Error   | StdDev  | Allocated |
| :---------------- | :------- | :------ | :------ | :-------- |
| **FullTraversal** | 162.9 ms | 0.97 ms | 0.91 ms | 5.3 KB    |

#### Detailed C# Results (Linux)

| Method            |     Mean |   Error |  StdDev | Allocated |
| ----------------- | -------: | ------: | ------: | --------: |
| **FullTraversal** | 161.3 ms | 1.29 ms | 1.20 ms |    5.3 KB |

---

## GMOD Versioning Path Conversion

### Cross-Platform Performance Comparison

| Operation        | Windows C++ | Linux C++ | Performance Ratio | Status | Notes |
| :--------------- | :---------- | :-------- | :---------------- | :----: | :---- |
| **Convert Path** | 30.4 μs     | 172 μs    | **5.66x slower**  |   ⚠️   |       |

### GMOD Versioning Path Conversion Performance Comparison

#### Cross-Platform C++ Results

| C++ Method      | Windows Time | Linux Time | Performance Ratio | C# Method (Win) | C# Time (Win) | C# Method (Linux) | C# Time (Linux) | C++ vs C# (Windows) | C++ vs C# (Linux)  |
| :-------------- | :----------- | :--------- | :---------------- | :-------------- | :------------ | :---------------- | :-------------- | :------------------ | :----------------- |
| **convertPath** | 30.4 μs      | 172 μs     | **5.66x slower**  | **ConvertPath** | 1.489 μs      | **ConvertPath**   | 1.464 μs        | ❌ **20.4x slower** | ❌ **117x slower** |

#### Detailed C++ Results

##### Windows Platform

| Benchmark       | Time    | CPU     | Iterations |
| :-------------- | :------ | :------ | :--------- |
| **convertPath** | 30.4 μs | 30.3 μs | 459K       |

##### Linux Platform

| Benchmark       | Time   | CPU    | Iterations |
| :-------------- | :----- | :----- | :--------- |
| **convertPath** | 172 μs | 172 μs | 80,599     |

#### Detailed C# Results (Windows)

| Method          | Mean     | Error     | StdDev    | Gen0   | Allocated |
| :-------------- | :------- | :-------- | :-------- | :----- | :-------- |
| **ConvertPath** | 1.489 μs | 0.0108 μs | 0.0090 μs | 0.2575 | 3.17 KB   |

#### Detailed C# Results (Linux)

| Method          | Mean     | Error     | StdDev    | Gen0   | Allocated |
| :-------------- | :------- | :-------- | :-------- | :----- | :-------- |
| **ConvertPath** | 1.464 μs | 0.0115 μs | 0.0107 μs | 0.2575 | 3.17 KB   |

---

## Short String Hashing

### Cross-Platform Performance Comparison

| Operation         | Windows C++ | Linux C++ | Performance Ratio | Status | Notes                      |
| :---------------- | :---------- | :-------- | :---------------- | :----: | :------------------------- |
| **bcl (400)**     | 1.13 ns     | 2.72 ns   | **2.41x slower**  |   ✅   | Linux significantly slower |
| **bcl (H346)**    | 2.49 ns     | 2.52 ns   | **1.01x slower**  |   ✅   | Similar performance        |
| **bclOrd (400)**  | 1.69 ns     | 0.931 ns  | **1.81x faster**  |   ✅   | Linux advantage            |
| **bclOrd (H346)** | 2.78 ns     | 2.02 ns   | **1.38x faster**  |   ✅   | Strong Linux performance   |
| **Larson (400)**  | 1.48 ns     | 1.60 ns   | **1.08x slower**  |   ✅   | Similar performance        |
| **Larson (H346)** | 4.55 ns     | 3.43 ns   | **1.33x faster**  |   ✅   | Linux improvement          |
| **crc32 (400)**   | 1.43 ns     | 1.38 ns   | **1.04x faster**  |   ✅   | Slight Linux advantage     |
| **crc32 (H346)**  | 4.27 ns     | 4.22 ns   | **1.01x faster**  |   ✅   | Marginal improvement       |
| **fnv (400)**     | 1.36 ns     | 1.17 ns   | **1.16x faster**  |   ✅   | Linux faster               |
| **fnv (H346)**    | 3.11 ns     | 2.61 ns   | **1.19x faster**  |   ✅   | Good Linux performance     |

### Short String Hashing Performance Comparison

#### Cross-Platform C++ Results

| Algorithm         | Windows Time | Linux Time | Performance Ratio | C# Baseline (Win) | C# Baseline (Linux) | C++ vs C# (Windows) | C++ vs C# (Linux)   |
| :---------------- | :----------- | :--------- | :---------------- | :---------------- | :------------------ | :------------------ | :------------------ |
| **bcl (400)**     | 1.13 ns      | 2.72 ns    | **2.41x slower**  | 1.135 ns          | 1.214 ns            | ✅ **1.00x same**   | ❌ **2.24x slower** |
| **bcl (H346)**    | 2.49 ns      | 2.52 ns    | **1.01x slower**  | 4.551 ns          | 4.560 ns            | ✅ **1.83x faster** | ✅ **1.81x faster** |
| **bclOrd (400)**  | 1.69 ns      | 0.931 ns   | **1.81x faster**  | 1.514 ns          | 1.368 ns            | ❌ **1.12x slower** | ✅ **1.47x faster** |
| **bclOrd (H346)** | 2.78 ns      | 2.02 ns    | **1.38x faster**  | 3.127 ns          | 2.340 ns            | ✅ **1.12x faster** | ✅ **1.16x faster** |
| **Larson (400)**  | 1.48 ns      | 1.60 ns    | **1.08x slower**  | 1.219 ns          | 1.244 ns            | ❌ **1.21x slower** | ❌ **1.29x slower** |
| **Larson (H346)** | 4.55 ns      | 3.43 ns    | **1.33x faster**  | 3.349 ns          | 3.494 ns            | ❌ **1.36x slower** | ✅ **1.02x faster** |
| **crc32 (400)**   | 1.43 ns      | 1.38 ns    | **1.04x faster**  | 1.215 ns          | 1.235 ns            | ❌ **1.18x slower** | ❌ **1.12x slower** |
| **crc32 (H346)**  | 4.27 ns      | 4.22 ns    | **1.01x faster**  | 3.259 ns          | 3.441 ns            | ❌ **1.31x slower** | ❌ **1.23x slower** |
| **fnv (400)**     | 1.36 ns      | 1.17 ns    | **1.16x faster**  | 1.205 ns          | 1.291 ns            | ❌ **1.13x slower** | ✅ **1.10x faster** |
| **fnv (H346)**    | 3.11 ns      | 2.61 ns    | **1.19x faster**  | 3.337 ns          | 3.741 ns            | ✅ **1.07x faster** | ✅ **1.43x faster** |

#### Detailed C++ Results

##### Windows Platform

| Benchmark               | Input      | Time    | CPU     | Iterations |
| :---------------------- | :--------- | :------ | :------ | :--------- |
| **bcl_400**             | 400        | 1.13 ns | 1.13 ns | 17.7B      |
| **bcl_H346_11112**      | H346.11112 | 2.49 ns | 2.49 ns | 5.65B      |
| **bclOrd_400**          | 400        | 1.69 ns | 1.69 ns | 8.30B      |
| **bclOrd_H346_11112**   | H346.11112 | 2.78 ns | 2.78 ns | 5.05B      |
| **Larson_400**          | 400        | 1.48 ns | 1.48 ns | 9.46B      |
| **Larson_H346_11112**   | H346.11112 | 4.55 ns | 4.55 ns | 3.08B      |
| **crc32Intrinsic_400**  | 400        | 1.43 ns | 1.43 ns | 9.78B      |
| **crc32Intrinsic_H346** | H346.11112 | 4.27 ns | 4.27 ns | 3.28B      |
| **fnv_400**             | 400        | 1.36 ns | 1.36 ns | 10.3B      |
| **fnv_H346_11112**      | H346.11112 | 3.11 ns | 3.11 ns | 4.50B      |

##### Linux Platform

| Benchmark               | Input      | Time     | CPU      | Iterations |
| :---------------------- | :--------- | :------- | :------- | :--------- |
| **bcl_400**             | 400        | 2.72 ns  | 2.72 ns  | 5.42B      |
| **bcl_H346_11112**      | H346.11112 | 2.52 ns  | 2.52 ns  | 5.63B      |
| **bclOrd_400**          | 400        | 0.931 ns | 0.931 ns | 15.09B     |
| **bclOrd_H346_11112**   | H346.11112 | 2.02 ns  | 2.02 ns  | 6.96B      |
| **Larson_400**          | 400        | 1.60 ns  | 1.60 ns  | 8.73B      |
| **Larson_H346_11112**   | H346.11112 | 3.43 ns  | 3.43 ns  | 4.08B      |
| **crc32Intrinsic_400**  | 400        | 1.38 ns  | 1.38 ns  | 10.17B     |
| **crc32Intrinsic_H346** | H346.11112 | 4.22 ns  | 4.22 ns  | 3.34B      |
| **fnv_400**             | 400        | 1.17 ns  | 1.17 ns  | 11.84B     |
| **fnv_H346_11112**      | H346.11112 | 2.61 ns  | 2.61 ns  | 5.39B      |

#### Detailed C# Results (Windows)

| Method                  | Input      | Mean     | Error     | StdDev    | Ratio        | RatioSD | Rank | Allocated | Alloc Ratio |
| :---------------------- | :--------- | :------- | :-------- | :-------- | :----------- | :------ | :--- | :-------- | :---------- |
| **Bcl_400**             | 400        | 1.135 ns | 0.0108 ns | 0.0101 ns | baseline     |         | 1    | -         | NA          |
| **BclOrd_400**          | 400        | 1.514 ns | 0.0064 ns | 0.0060 ns | 1.33x slower | 0.01x   | 2    | -         | NA          |
| **Larson_400**          | 400        | 1.219 ns | 0.0036 ns | 0.0034 ns | 1.07x slower | 0.01x   | 3    | -         | NA          |
| **Crc32Intrinsic_400**  | 400        | 1.215 ns | 0.0040 ns | 0.0037 ns | 1.07x slower | 0.01x   | 4    | -         | NA          |
| **Fnv_400**             | 400        | 1.205 ns | 0.0033 ns | 0.0031 ns | 1.06x slower | 0.01x   | 5    | -         | NA          |
| **Bcl_H346**            | H346.11112 | 4.551 ns | 0.0143 ns | 0.0134 ns | baseline     |         | 1    | -         | NA          |
| **BclOrd_H346**         | H346.11112 | 3.127 ns | 0.0074 ns | 0.0070 ns | 1.46x faster | 0.01x   | 2    | -         | NA          |
| **Larson_H346**         | H346.11112 | 3.349 ns | 0.0098 ns | 0.0092 ns | 1.36x faster | 0.01x   | 3    | -         | NA          |
| **Crc32Intrinsic_H346** | H346.11112 | 3.259 ns | 0.0093 ns | 0.0087 ns | 1.40x faster | 0.01x   | 4    | -         | NA          |
| **Fnv_H346**            | H346.11112 | 3.337 ns | 0.0116 ns | 0.0108 ns | 1.36x faster | 0.01x   | 5    | -         | NA          |

#### Detailed C# Results (Linux)

| Method             | Input      |     Mean |     Error |    StdDev |        Ratio | RatioSD | Rank | Allocated | Alloc Ratio |
| :----------------- | :--------- | -------: | --------: | --------: | -----------: | ------: | ---: | --------: | ----------: |
| **BclOrd**         | 400        | 1.368 ns | 0.0264 ns | 0.0234 ns | 1.13x slower |   0.03x |    4 |         - |          NA |
| **Fnv**            | 400        | 1.291 ns | 0.0092 ns | 0.0077 ns | 1.06x slower |   0.01x |    3 |         - |          NA |
| **Larsson**        | 400        | 1.244 ns | 0.0097 ns | 0.0091 ns | 1.03x slower |   0.01x |    2 |         - |          NA |
| **Crc32Intrinsic** | 400        | 1.235 ns | 0.0053 ns | 0.0044 ns | 1.02x slower |   0.01x |    2 |         - |          NA |
| **Bcl**            | 400        | 1.214 ns | 0.0146 ns | 0.0137 ns |     baseline |         |    1 |         - |          NA |
|                    |            |          |           |           |              |         |      |           |             |
| **Bcl**            | H346.11112 | 4.560 ns | 0.0101 ns | 0.0079 ns |     baseline |         |    5 |         - |          NA |
| **Fnv**            | H346.11112 | 3.741 ns | 0.1091 ns | 0.1633 ns | 1.26x faster |   0.03x |    4 |         - |          NA |
| **Larsson**        | H346.11112 | 3.494 ns | 0.0172 ns | 0.0161 ns | 1.31x faster |   0.01x |    3 |         - |          NA |
| **Crc32Intrinsic** | H346.11112 | 3.441 ns | 0.0109 ns | 0.0091 ns | 1.32x faster |   0.00x |    2 |         - |          NA |
| **BclOrd**         | H346.11112 | 2.340 ns | 0.0407 ns | 0.0340 ns | 1.95x faster |   0.03x |    1 |         - |          NA |

---

_Last updated: July 20, 2025_
