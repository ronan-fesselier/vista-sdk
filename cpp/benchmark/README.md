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

| Operation Category           | C++ vs C# Performance            | Status | Key Findings                       |
| :--------------------------- | :------------------------------- | :----: | :--------------------------------- |
| **Hash Table Operations**    | **0.97-2.30x faster**            |   ✅   | C++ shows performance advantage    |
| **Codebook Access (Direct)** | **7.07x faster** vs C# baseline  |   ✅   | Strong C++ performance advantage   |
| **Codebook Access (API)**    | **1,408x slower** vs C# baseline |   ⚠️   | Expected method call overhead      |
| **String Hashing (Short)**   | **1.24-1.74x mixed**             |   ✅   | Mixed results, algorithm dependent |
| **String Hashing (Long)**    | **1.15x slower to 1.30x faster** |   ✅   | C++ advantage on longer strings    |
| **GMOD Loading**             | **1.00x same**                   |   ✅   | Achieved C# performance parity     |
| **GMOD Lookup**              | **1.14-2.30x faster**            |   ✅   | C++ shows performance advantage    |
| **GMOD Traversal**           | **1.97x slower**                 |   ❌   | **Optimization opportunity**       |
| **Path Parsing**             | **5.17-15.2x slower**            |  ❌❌  | **Optimization needed**            |
| **Version Path Conversion**  | **19.6x slower**                 | ❌❌❌ | **Critical performance gap**       |

### Linux Platform Performance

| Operation Category           | C++ vs C# Performance            | Status | Key Findings                                      |
| :--------------------------- | :------------------------------- | :----: | :------------------------------------------------ |
| **Hash Table Operations**    | **1.20-3.37x faster**            |   ✅   | Strong C++ advantage on Linux                     |
| **Codebook Access (Direct)** | **7.73x faster** vs C# baseline  |   ✅   | Excellent Linux performance boost                 |
| **Codebook Access (API)**    | **330x slower** vs C# baseline   |   ⚠️   | Much better than Windows, still expected overhead |
| **String Hashing (Short)**   | **1.03-2.40x mixed**             |   ✅   | Mixed results, some algorithms competitive        |
| **String Hashing (Long)**    | **1.02x slower to 1.81x faster** |   ✅   | Generally better performance than Windows         |
| **GMOD Loading**             | **2.73x faster**                 |   ✅   | Excellent performance advantage                   |
| **GMOD Lookup**              | **1.31-2.56x faster**            |   ✅   | Excellent C++ advantage, especially GMOD API |
| **GMOD Traversal**           | **1.67x slower**                 |   ❌   | **Better than Windows, needs optimization**        |
| **Path Parsing**             | **3.18-7.03x slower**            |  ❌❌  | **Improvement but still optimization needed**                           |
| **Version Path Conversion**  | **8.95x slower**                  | ❌❌❌ | **Major improvement but still critical**        |

### Cross-Platform Summary

| Operation Category           | Linux vs Windows (C++) | Status | Platform Recommendation                  |
| :--------------------------- | :--------------------- | :----: | :--------------------------------------- |
| **Hash Table Operations**    | **1.13-11.9x faster**  |   ✅   | **Linux strongly preferred**             |
| **Codebook Access (Direct)** | **1.47-3.99x faster**  |   ✅   | **Linux significantly better**           |
| **Codebook Access (API)**    | **11.9x faster**       |   ✅   | **Linux dramatically better**            |
| **String Hashing (Short)**   | **1.04-2.31x mixed**   |   ⚠️   | **Algorithm-dependent performance**      |
| **String Hashing (Long)**    | **1.12-1.32x faster**  |   ✅   | **Linux slightly better**                |
| **GMOD Loading**             | **2.01x faster**       |   ✅   | **Linux preferred**                      |
| **GMOD Lookup**              | **1.45-7.91x mixed**   |   ⚠️   | **Mixed results, significant GMOD API improvement** |
| **GMOD Traversal**           | **1.03x slower**       |   ❌   | **Nearly identical cross-platform**              |
| **Path Parsing**             | **1.73-2.04x faster**  |   ✅   | **Linux significant improvement** |
| **Version Path Conversion**  | **2.23x faster**       |   ✅   | **Linux dramatically better** |

---

## Codebooks Lookup

### Cross-Platform Performance Comparison

Performance comparison between different C++ access methods for Vista SDK codebook operations:

| Operation             | Windows C++ | vs Baseline  | Linux C++ | vs Baseline  | Cross-Platform   | Notes |
| :-------------------- | :---------- | :----------: | :-------- | :----------: | :--------------- | :---- |
| **SDK Instance**      | 0.689 ns    |  **1.00x**   | 0.212 ns  |  **1.00x**   | **3.25x faster** |       |
| **SDK Reference**     | 0.855 ns    |  **1.24x**   | 0.210 ns  |  **0.99x**   | **4.07x faster** |       |
| **Array Lookup**      | 1.60 ns     |  **2.32x**   | 0.699 ns  |  **3.30x**   | **2.29x faster** |       |
| **Vector Lookup**     | 1.78 ns     |  **2.58x**   | 1.68 ns   |  **7.92x**   | **1.06x faster** |       |
| **Hash Table Lookup** | 2.17 ns     |  **3.15x**   | 2.79 ns   |  **13.16x**  | **1.29x slower** |       |
| **CHD Dictionary**    | 36.3 ns     |  **52.7x**   | 23.6 ns   |  **111x**    | **1.54x faster** |       |
| **Tree Lookup**       | 7.13 ns     |  **10.3x**   | 3.04 ns   |  **14.34x**  | **2.35x faster** |       |
| **SDK API Method**    | 859 ns      |  **1,247x**  | 76.8 ns   |   **362x**   | **11.2x faster** |       |
| **VIS Singleton**     | 122,457 ns  | **177,787x** | 92,820 ns | **437,830x** | **1.32x faster** |       |

#### STL Container Performance

| C++ Method        | Windows Time | Linux Time | Performance Ratio | Implementation                      | Performance vs Array | Status | Notes |
| :---------------- | :----------- | :--------- | :---------------- | :---------------------------------- | :------------------- | :----- | :---- |
| **Array**         | 1.60 ns      | 0.699 ns   | **2.29x faster**  | `std::array` linear search          | **Baseline**         | ✅     |       |
| **Vector**        | 1.78 ns      | 1.68 ns    | **1.06x faster**  | `std::vector` linear search         | **2.40x slower**     | ✅     |       |
| **UnorderedMap**  | 2.17 ns      | 2.79 ns    | **1.29x slower**  | `std::unordered_map::find()`        | **3.99x slower**     | ✅     |       |
| **ChdDictionary** | 36.3 ns      | 23.6 ns    | **1.54x faster**  | CHD perfect hash table              | **33.8x slower**     | ✅     |       |
| **Map**           | 7.13 ns      | 3.04 ns    | **2.35x faster**  | `std::map::find()` (red-black tree) | **4.35x slower**     | ⚠️     |       |

#### Vista SDK Access Performance

| C++ Method             | Windows Time   | Linux Time    | Performance Ratio | Implementation                     | Performance vs Array | Status | Notes |
| :--------------------- | :------------- | :------------ | :---------------- | :--------------------------------- | :------------------- | :----- | :---- |
| **CodebooksInstance**  | **0.689 ns**   | **0.212 ns**  | **3.25x faster**  | Owned instance + direct access     | **3.30x faster**     | ✅     |       |
| **CodebooksReference** | **0.855 ns**   | **0.210 ns**  | **4.07x faster**  | Reference wrapper + direct access  | **3.33x faster**     | ✅     |       |
| **CodebooksAPI**       | **859 ns**     | **76.8 ns**   | **11.2x faster**  | `codebook()` method call           | **110x slower**      | ⚠️     |       |
| **CodebooksVISCall**   | **122,457 ns** | **92,820 ns** | **1.32x faster**  | `VIS::instance().codebooks()` call | **132,755x slower**  | ⚠️     |       |

#### Detailed C++ Results

##### Windows Platform

| Benchmark                 | Time           | CPU            | Iterations |
| :------------------------ | :------------- | :------------- | :--------- |
| **BM_CodebooksInstance**  | **0.689 ns**   | **0.688 ns**   | 20.2B      |
| **BM_CodebooksReference** | **0.855 ns**   | **0.854 ns**   | 16.4B      |
| **BM_Array**              | **1.60 ns**    | **1.60 ns**    | 8.96B      |
| **BM_Vector**             | **1.78 ns**    | **1.78 ns**    | 7.93B      |
| **BM_UnorderedMap**       | **2.17 ns**    | **2.17 ns**    | 6.45B      |
| **BM_ChdDictionary**      | **35.9 ns**    | **35.9 ns**    | 388M       |
| **BM_Map**                | **7.13 ns**    | **7.12 ns**    | 1.94B      |
| **BM_CodebooksAPI**       | **859 ns**     | **859 ns**     | 16.3M      |
| **BM_CodebooksVISCall**   | **117,876 ns** | **117,658 ns** | 115K       |

##### Linux Platform

| Benchmark                 | Time          | CPU           | Iterations |
| :------------------------ | :------------ | :------------ | :--------- |
| **BM_CodebooksInstance**  | **0.212 ns**  | **0.212 ns**  | 66.3B      |
| **BM_CodebooksReference** | **0.210 ns**  | **0.210 ns**  | 66.9B      |
| **BM_Array**              | **0.699 ns**  | **0.699 ns**  | 20.0B      |
| **BM_Vector**             | **1.68 ns**   | **1.68 ns**   | 8.4B       |
| **BM_UnorderedMap**       | **2.79 ns**   | **2.79 ns**   | 5.0B       |
| **BM_ChdDictionary**      | **23.6 ns**   | **23.6 ns**   | 589M       |
| **BM_Map**                | **3.04 ns**   | **3.04 ns**   | 4.6B       |
| **BM_CodebooksAPI**       | **76.8 ns**   | **76.8 ns**   | 182M       |
| **BM_CodebooksVISCall**   | **92,820 ns** | **92,817 ns** | 145K       |

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

| Operation     | Windows C++ | vs Baseline | Linux C++ | vs Baseline | Cross-Platform   | Notes                  |
| :------------ | :---------- | :---------: | :-------- | :---------: | :--------------- | :--------------------- |
| **GMOD Load** | 30.3 ms     |  **1.00x**  | 11.1 ms   |  **1.00x**  | **2.73x faster** | Full GMOD construction |

### GMOD Load Performance Comparison

| C++ Method   | Windows Time | Linux Time | Performance Ratio | C# Method | C# Time (Win) | C# Time (Linux) | C++ vs C# (Windows) | C++ vs C# (Linux)   |
| :----------- | :----------- | :--------- | :---------------- | :-------- | :------------ | :-------------- | :------------------ | :------------------ |
| **gmodLoad** | 30.3 ms      | 11.1 ms    | **2.73x faster**  | **Load**  | 30.40 ms      | 29.68 ms        | ✅ **1.00x same**   | ✅ **2.68x faster** |

#### Detailed C++ Results

##### Windows Platform

| Benchmark    | Time    | CPU     | Iterations |
| :----------- | :------ | :------ | :--------- |
| **gmodLoad** | 30.3 ms | 30.3 ms | 385        |

##### Linux Platform

| Benchmark    | Time    | CPU     | Iterations |
| :----------- | :------ | :------ | :--------- |
| **gmodLoad** | 11.1 ms | 11.1 ms | 1,251      |

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
| **Frozen Dictionary** | 17.4 ns     |  **1.00x**  | 52.8 ns   |  **1.00x**  | **3.04x slower** | ⚡ Baseline lookup method        |
| **Hash Table Lookup** | 16.7 ns     |  **0.96x**  | 51.6 ns   |  **0.98x**  | **3.09x slower** | ⚡ Similar                       |
| **GMOD API Lookup**   | 9.71 ns     |  **0.56x**  | 6.67 ns   |  **0.13x**  | **1.46x faster** | ✅ Expected method call overhead |

### GMOD Lookup Performance Comparison

#### Cross-Platform C++ Results

| C++ Method     | Windows Time | Linux Time | Performance Ratio | C# Method (Win) | C# Time (Win) | C# Method (Linux) | C# Time (Linux) | C++ vs C# (Windows) | C++ vs C# (Linux)   |
| :------------- | :----------- | :--------- | :---------------- | :-------------- | :------------ | :---------------- | :-------------- | :------------------ | :------------------ |
| **dict**       | 16.7 ns      | 51.6 ns    | **3.09x slower**  | **Dict**        | 38.34 ns      | **Dict**          | 37.85 ns        | ✅ **2.30x faster** | ❌ **1.36x slower** |
| **frozenDict** | 17.4 ns      | 52.8 ns    | **3.03x slower**  | **FrozenDict**  | 15.21 ns      | **FrozenDict**    | 15.81 ns        | ❌ **1.14x slower** | ❌ **3.34x slower** |
| **gmod**       | 10.2 ns      | 6.67 ns    | **1.53x faster**  | **Gmod**        | 15.62 ns      | **Gmod**          | 17.07 ns        | ✅ **1.53x faster** | ✅ **2.56x faster** |

#### Detailed C++ Results

##### Windows Platform

| Benchmark      | Time    | CPU     | Iterations |
| :------------- | :------ | :------ | :--------- |
| **Dict**       | 16.7 ns | 16.6 ns | 861M       |
| **FrozenDict** | 17.4 ns | 17.4 ns | 815M       |
| **Gmod**       | 9.71 ns | 9.62 ns | 1.47B      |

##### Linux Platform

| Benchmark      | Time    | CPU     | Iterations |
| :------------- | :------ | :------ | :--------- |
| **Dict**       | 51.6 ns | 51.6 ns | 273M       |
| **FrozenDict** | 52.8 ns | 52.7 ns | 266M       |
| **Gmod**       | 6.67 ns | 6.67 ns | 2.12B      |

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
| **TryParse**                       | 19.5 μs     | 11.3 μs   | **1.73x slower**  |   ✅   |       |
| **TryParseFullPath**               | 8.67 μs     | 4.43 μs   | **1.96x slower**  |   ✅   |       |
| **TryParseIndividualized**         | 17.9 μs     | 9.17 μs   | **1.95x slower**  |   ✅   |       |
| **TryParseFullPathIndividualized** | 8.75 μs     | 4.29 μs   | **2.04x slower**  |   ✅   |       |

### GMOD Path Parsing Performance Comparison

#### Cross-Platform C++ Results

| C++ Method                         | Windows Time | Linux Time | Performance Ratio | C# Method (Win)                    | C# Time (Win) | C# Method (Linux)                  | C# Time (Linux) | C++ vs C# (Windows) | C++ vs C# (Linux)   |
| :--------------------------------- | :----------- | :--------- | :---------------- | :--------------------------------- | :------------ | :--------------------------------- | :-------------- | :------------------ | :------------------ |
| **tryParse**                       | 19.5 μs      | 11.3 μs    | **1.73x slower**  | **TryParse**                       | 3.77 μs       | **TryParse**                       | 3.55 μs         | ❌ **5.17x slower** | ❌ **3.18x slower** |
| **tryParseFullPath**               | 8.67 μs      | 4.43 μs    | **1.96x slower**  | **TryParseFullPath**               | 571 ns        | **TryParseFullPath**               | 630 ns          | ❌ **15.2x slower** | ❌ **7.03x slower** |
| **tryParseIndividualized**         | 17.9 μs      | 9.17 μs    | **1.95x slower**  | **TryParseIndividualized**         | 1.49 μs       | **TryParseIndividualized**         | 1.60 μs         | ❌ **12.0x slower** | ❌ **5.73x slower** |
| **tryParseFullPathIndividualized** | 8.75 μs      | 4.29 μs    | **2.04x slower**  | **TryParseFullPathIndividualized** | 694 ns        | **TryParseFullPathIndividualized** | 788 ns          | ❌ **12.6x slower** | ❌ **5.44x slower** |

#### Detailed C++ Results

##### Windows Platform

| Benchmark                          | Time    | CPU     | Iterations |
| :--------------------------------- | :------ | :------ | :--------- |
| **tryParse**                       | 19.5 μs | 19.4 μs | 711K       |
| **tryParseFullPath**               | 8.67 μs | 8.65 μs | 1.64M      |
| **tryParseIndividualized**         | 17.9 μs | 17.8 μs | 800K       |
| **tryParseFullPathIndividualized** | 8.75 μs | 8.69 μs | 1.60M      |

##### Linux Platform

| Benchmark                          | Time    | CPU     | Iterations |
| :--------------------------------- | :------ | :------ | :--------- |
| **tryParse**                       | 11.3 μs | 11.3 μs | 1.24M      |
| **tryParseFullPath**               | 4.43 μs | 4.43 μs | 3.17M      |
| **tryParseIndividualized**         | 9.17 μs | 9.16 μs | 1.53M      |
| **tryParseFullPathIndividualized** | 4.29 μs | 4.29 μs | 3.25M      |

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
| **Full Traversal** | 263 ms      | 270 ms    | **1.03x slower**  |   ✅   |       |

### GMOD Traversal Performance Comparison

#### Cross-Platform C++ Results

| C++ Method        | Windows Time | Linux Time | Performance Ratio | C# Method (Win)   | C# Time (Win) | C# Method (Linux) | C# Time (Linux) | C++ vs C# (Windows) | C++ vs C# (Linux)   |
| :---------------- | :----------- | :--------- | :---------------- | :---------------- | :------------ | :---------------- | :-------------- | :------------------ | :------------------ |
| **fullTraversal** | 263 ms       | 270 ms     | **1.03x slower**  | **FullTraversal** | 135.3 ms      | **FullTraversal** | 161.3 ms        | ❌ **1.94x slower** | ❌ **1.67x slower** |

#### Detailed C++ Results

##### Windows Platform

| Benchmark         | Time   | CPU    | Iterations |
| :---------------- | :----- | :----- | :--------- |
| **FullTraversal** | 263 ms | 245 ms | 62         |

##### Linux Platform

| Benchmark         | Time   | CPU    | Iterations |
| :---------------- | :----- | :----- | :--------- |
| **FullTraversal** | 270 ms | 270 ms | 52         |

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
| **Convert Path** | 29.2 μs     | 13.1 μs   | **2.23x slower**  |   ✅   |       |

### GMOD Versioning Path Conversion Performance Comparison

#### Cross-Platform C++ Results

| C++ Method      | Windows Time | Linux Time | Performance Ratio | C# Method (Win) | C# Time (Win) | C# Method (Linux) | C# Time (Linux) | C++ vs C# (Windows) | C++ vs C# (Linux)  |
| :-------------- | :----------- | :--------- | :---------------- | :-------------- | :------------ | :---------------- | :-------------- | :------------------ | :----------------- |
| **convertPath** | 29.2 μs      | 13.1 μs    | **2.23x slower**  | **ConvertPath** | 1.489 μs      | **ConvertPath**   | 1.464 μs        | ❌ **19.6x slower** | ❌ **8.95x slower** |

#### Detailed C++ Results

##### Windows Platform

| Benchmark       | Time    | CPU     | Iterations |
| :-------------- | :------ | :------ | :--------- |
| **convertPath** | 29.2 μs | 28.9 μs | 482K       |

##### Linux Platform

| Benchmark       | Time    | CPU     | Iterations |
| :-------------- | :------ | :------ | :--------- |
| **convertPath** | 13.1 μs | 13.1 μs | 1.07M      |

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

_Last updated: July 26, 2025_
