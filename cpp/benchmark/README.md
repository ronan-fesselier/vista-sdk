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

| Platform    | Benchmark Framework     | C++ Compiler  | C# Runtime               | OS Version                              |
| :---------- | :---------------------- | :------------ | :----------------------- | :-------------------------------------- |
| **Windows** | Google Benchmark v1.9.4 | MSVC 17.14.10 | .NET 8.0.16, RyuJIT AVX2 | Windows 10                              |
| **Windows** | Google Benchmark v1.9.4 | GCC 14.2.0    | .NET 8.0.16, RyuJIT AVX2 | Windows 10                              |
| **Linux**   | Google Benchmark v1.9.4 | Clang 16.0.6  | .NET 8.0.17, RyuJIT AVX2 | LMDE 6 (faye) / Debian 6.1.140-1 x86_64 |

**Note:** All C++ performance data below includes results from both MSVC 17.14.10 and GCC 14.2.0 (mingw-w64) compilers on Windows platform for comprehensive compiler comparison.

---

## Executive Summary

### Windows Platform Performance (MSVC 17.14.10)

**Note:** The following results are from MSVC 17.14.10 compiler. See GCC comparison section below for GCC 14.2.0 results.

| Operation Category           | C++ vs C# Performance            | Status |
| :--------------------------- | :------------------------------- | :----: |
| **Hash Table Operations**    | **1.18-2.91x faster**            |   ✅   |
| **Codebook Access (Direct)** | **1.09x slower** vs C# baseline  |   ⚠️   |
| **Codebook Access (API)**    | **1,179x slower** vs C# baseline |   ⚠️   |
| **String Hashing (Short)**   | **1.03-1.70x mixed**             |   ✅   |
| **String Hashing (Long)**    | **1.03x slower to 1.51x slower** |   ⚠️   |
| **GMOD Loading**             | **1.05x slower**                 |   ⚠️   |
| **GMOD Lookup**              | **1.18-2.91x faster**            |   ✅   |
| **GMOD Lookup (HashMap)**    | **1.20x slower**                 |   ❌   |
| **GMOD Traversal**           | **1.49x slower**                 |   ❌   |
| **Path Parsing**             | **5.12-20.3x slower**            |  ❌❌  |
| **Version Path Conversion**  | **5.42x slower**                 |  ❌❌  |

### Windows Platform Performance (GCC 14.2.0)

| Operation Category           | C++ vs C# Performance           | Status |
| :--------------------------- | :------------------------------ | :----: |
| **Hash Table Operations**    | **1.0-13.0x faster**            |   ✅   |
| **Codebook Access (Direct)** | **25.7x faster** vs C# baseline |   ✅   |
| **Codebook Access (API)**    | **96.3x slower** vs C# baseline |   ❌   |
| **String Hashing (Short)**   | **1.1-2.4x mixed**              |   ✅   |
| **String Hashing (Long)**    | **1.1-2.4x mixed**              |   ✅   |
| **GMOD Loading**             | **1.52x faster**                |   ✅   |
| **GMOD Lookup**              | **1.2-1.7x faster**             |   ✅   |
| **GMOD Lookup (HashMap)**    | **1.09x faster**                |   ✅   |
| **GMOD Traversal**           | **1.57x slower**                |   ❌   |
| **Path Parsing**             | **1.3-1.6x faster**             |   ✅   |
| **Version Path Conversion**  | **1.3x faster**                 |   ✅   |

### GCC vs MSVC Performance Comparison (Windows)

| Operation Category           | GCC vs MSVC Performance | Status |
| :--------------------------- | :---------------------- | :----: |
| **Codebook Access (Direct)** | **23.6x faster**        |   ✅   |
| **Codebook Access (API)**    | **12.2x faster**        |   ✅   |
| **GMOD Loading**             | **1.60x faster**        |   ✅   |
| **GMOD Lookup**              | **1.3-4.2x mixed**      |   ⚠️   |
| **GMOD Traversal**           | **1.62x slower**        |   ❌   |
| **Path Parsing**             | **1.3-3.7x faster**     |   ✅   |
| **Version Path Conversion**  | **1.3x faster**         |   ✅   |
| **STL Containers**           | **Mixed results**       |   ⚠️   |
| **Hash Algorithms**          | **Mixed results**       |   ⚠️   |

### Linux Platform Performance (Clang 16.0.6)

| Operation Category           | C++ vs C# Performance            | Status |
| :--------------------------- | :------------------------------- | :----: |
| **Hash Table Operations**    | **1.20-3.37x faster**            |   ✅   |
| **Codebook Access (Direct)** | **2.74x faster** vs C# baseline  |   ✅   |
| **Codebook Access (API)**    | **133x slower** vs C# baseline   |   ⚠️   |
| **String Hashing (Short)**   | **1.03-2.40x mixed**             |   ✅   |
| **String Hashing (Long)**    | **1.02x slower to 1.81x faster** |   ✅   |
| **GMOD Loading**             | **2.68x faster**                 |   ✅   |
| **GMOD Lookup**              | **1.47-2.45x faster**            |   ✅   |
| **GMOD Lookup (HashMap)**    | **1.91x slower**                 |   ❌   |
| **GMOD Traversal**           | **1.67x slower**                 |   ❌   |
| **Path Parsing**             | **3.18-7.03x slower**            |  ❌❌  |
| **Version Path Conversion**  | **2.88x slower**                 |  ❌❌  |

### Cross-Platform Summary (Linux Clang vs Windows MSVC)

**Note:** This comparison is between Linux Clang 16.0.6 and Windows MSVC 17.14.10. For GCC vs MSVC comparison, see section above.

| Operation Category           | Linux vs Windows (C++) | Status |
| :--------------------------- | :--------------------- | :----: |
| **Hash Table Operations**    | **1.14-12.0x faster**  |   ✅   |
| **Codebook Access (Direct)** | **3.14x faster**       |   ✅   |
| **Codebook Access (API)**    | **10.8x faster**       |   ✅   |
| **String Hashing (Short)**   | **1.04-2.31x mixed**   |   ⚠️   |
| **String Hashing (Long)**    | **1.12-1.32x faster**  |   ✅   |
| **GMOD Loading**             | **3.18x faster**       |   ✅   |
| **GMOD Lookup**              | **1.65-3.54x mixed**   |   ⚠️   |
| **GMOD Lookup (HashMap)**    | **1.65x slower**       |   ❌   |
| **GMOD Traversal**           | **1.34x slower**       |   ❌   |
| **Path Parsing**             | **1.72-5.29x faster**  |   ✅   |
| **Version Path Conversion**  | **6.92x faster**       |   ✅   |

---

## Codebooks Lookup

### Cross-Platform Performance Comparison (MSVC vs Linux Clang)

**Note:** Windows results are from MSVC 17.14.10. See GCC section below for GCC 14.2.0 comparison.

Performance comparison between different C++ access methods for Vista SDK codebook operations:

| Operation             | Windows C++ (MSVC) | vs Baseline | Linux C++ (Clang) | vs Baseline | Cross-Platform   |
| :-------------------- | :----------------- | :---------: | :---------------- | :---------: | :--------------- |
| **SDK Instance**      | 0.637 ns           |  **1.00x**  | 0.212 ns          |  **1.00x**  | **3.01x faster** |
| **SDK Reference**     | 0.791 ns           |  **1.24x**  | 0.210 ns          |  **0.99x**  | **3.77x faster** |
| **Array Lookup**      | 1.49 ns            |  **2.34x**  | 0.699 ns          |  **3.30x**  | **2.13x faster** |
| **Vector Lookup**     | 1.73 ns            |  **2.72x**  | 1.68 ns           |  **7.92x**  | **1.03x faster** |
| **Hash Table Lookup** | 2.03 ns            |  **3.19x**  | 2.79 ns           | **13.16x**  | **1.37x slower** |
| **CHD Dictionary**    | 33.5 ns            |  **52.6x**  | 23.6 ns           |  **111x**   | **1.42x faster** |
| **Tree Lookup**       | 3.31 ns            |  **5.20x**  | 3.04 ns           | **14.34x**  | **1.09x faster** |
| **SDK API Method**    | 818 ns             |  **1284x**  | 76.8 ns           |  **362x**   | **10.6x faster** |
| **VIS Singleton**     | 108351 ns          | **170063x** | 92820 ns          | **437830x** | **1.17x faster** |

### Windows MSVC vs GCCC Comparison

| Operation             | MSVC 17.14.10 | GCC 14.2.0    | GCC vs MSVC      |
| :-------------------- | :------------ | :------------ | :--------------- |
| **SDK Instance**      | 0.637 ns      | **0.027 ns**  | **23.6x faster** |
| **SDK Reference**     | 0.791 ns      | **0.027 ns**  | **29.3x faster** |
| **Array Lookup**      | 1.49 ns       | **0.412 ns**  | **3.6x faster**  |
| **Vector Lookup**     | **1.73 ns**   | 3.03 ns       | **1.8x slower**  |
| **Hash Table Lookup** | **2.03 ns**   | 6.58 ns       | **3.2x slower**  |
| **CHD Dictionary**    | 33.5 ns       | **7.28 ns**   | **4.6x faster**  |
| **Tree Lookup**       | 3.31 ns       | **2.37 ns**   | **1.4x faster**  |
| **SDK API Method**    | 818 ns        | **66.8 ns**   | **12.2x faster** |
| **VIS Singleton**     | 108351 ns     | **107337 ns** | **1.01x faster** |

#### STL Container Performance (MSVC vs Linux Clang)

| C++ Method        | Windows Time (MSVC) | Linux Time (Clang) | Performance Ratio | Implementation                      | Performance vs Array | Status |
| :---------------- | :------------------ | :----------------- | :---------------- | :---------------------------------- | :------------------- | :----- |
| **Array**         | 1.47 ns             | 0.699 ns           | **2.10x faster**  | `std::array` linear search          | **Baseline**         | ✅     |
| **Vector**        | 1.77 ns             | 1.68 ns            | **1.05x faster**  | `std::vector` linear search         | **1.20x slower**     | ✅     |
| **UnorderedMap**  | 2.11 ns             | 2.79 ns            | **1.32x slower**  | `std::unordered_map::find()`        | **1.44x slower**     | ✅     |
| **ChdDictionary** | 34.2 ns             | 23.6 ns            | **1.45x faster**  | CHD perfect hash table              | **23.3x slower**     | ⚠️     |
| **Map**           | 7.12 ns             | 3.04 ns            | **2.34x faster**  | `std::map::find()` (red-black tree) | **4.84x slower**     | ⚠️     |

#### STL Container Performance (Windows: MSVC vs GCC)

| C++ Method        | MSVC 17.14.10 | GCC 14.2.0   | GCC vs MSVC     | Implementation Notes                           | Status |
| :---------------- | :------------ | :----------- | :-------------- | :--------------------------------------------- | :----- |
| **Array**         | 1.49 ns       | **0.412 ns** | **3.6x faster** | GCC better optimization                        | ✅     |
| **Vector**        | **1.73 ns**   | 3.03 ns      | **1.8x slower** | **GCC STL vector implementation difference**   | ⚠️     |
| **UnorderedMap**  | **2.03 ns**   | 6.58 ns      | **3.2x slower** | **GCC STL hash map implementation difference** | ⚠️     |
| **ChdDictionary** | 33.5 ns       | **7.28 ns**  | **4.6x faster** | GCC excellent optimization                     | ✅     |
| **Map**           | 3.31 ns       | **2.37 ns**  | **1.4x faster** | GCC red-black tree optimization                | ✅     |

#### Vista SDK Access Performance (MSVC vs Linux Clang)

| C++ Method             | Windows Time (MSVC) | Linux Time (Clang) | Performance Ratio | Implementation                     | Performance vs Array | Status |
| :--------------------- | :------------------ | :----------------- | :---------------- | :--------------------------------- | :------------------- | :----- |
| **CodebooksInstance**  | **0.637 ns**        | **0.212 ns**       | **3.01x faster**  | Owned instance + direct access     | **2.34x faster**     | ✅     |
| **CodebooksReference** | **0.791 ns**        | **0.210 ns**       | **3.77x faster**  | Reference wrapper + direct access  | **1.88x faster**     | ✅     |
| **CodebooksAPI**       | **818 ns**          | **76.8 ns**        | **10.6x faster**  | `codebook()` method call           | **549x slower**      | ⚠️     |
| **CodebooksVISCall**   | **108351 ns**       | **92820 ns**       | **1.17x faster**  | `VIS::instance().codebooks()` call | **72751x slower**    | ❌     |

#### Vista SDK Access Performance (Windows: GCC vs MSVC)

| C++ Method             | MSVC 17.14.10 | GCC 14.2.0    | GCC vs MSVC      | Performance Notes                       | Status |
| :--------------------- | :------------ | :------------ | :--------------- | :-------------------------------------- | :----- |
| **CodebooksInstance**  | 0.637 ns      | **0.027 ns**  | **23.6x faster** | **Near-complete optimization**          | ✅     |
| **CodebooksReference** | 0.791 ns      | **0.027 ns**  | **29.3x faster** | **Near-complete optimization**          | ✅     |
| **CodebooksAPI**       | 818 ns        | **66.8 ns**   | **12.2x faster** | **Dramatic method call optimization**   | ✅     |
| **CodebooksVISCall**   | 108351 ns     | **107337 ns** | **1.01x faster** | **Modest singleton access improvement** | ✅     |

#### Detailed C++ Results

##### Windows Platform (MSVC 17.14.10)

| Benchmark                 | Time          | CPU           | Iterations |
| :------------------------ | :------------ | :------------ | :--------- |
| **BM_CodebooksInstance**  | **0.637 ns**  | **0.637 ns**  | 22.07B     |
| **BM_CodebooksReference** | **0.791 ns**  | **0.790 ns**  | 17.67B     |
| **BM_Array**              | **1.49 ns**   | **1.49 ns**   | 8.96B      |
| **BM_Vector**             | **1.73 ns**   | **1.72 ns**   | 8.07B      |
| **BM_UnorderedMap**       | **2.03 ns**   | **2.03 ns**   | 6.89B      |
| **BM_ChdDictionary**      | **33.5 ns**   | **33.4 ns**   | 417M       |
| **BM_Map**                | **3.31 ns**   | **3.30 ns**   | 4.27B      |
| **BM_CodebooksAPI**       | **818 ns**    | **817 ns**    | 17.53M     |
| **BM_CodebooksVISCall**   | **108351 ns** | **108154 ns** | 128K       |

##### Windows Platform (GCC 14.2.0)

| Benchmark                 | Time          | CPU           | Iterations   |
| :------------------------ | :------------ | :------------ | :----------- |
| **BM_CodebooksInstance**  | **0.027 ns**  | **0.026 ns**  | 523976608187 |
| **BM_CodebooksReference** | **0.027 ns**  | **0.027 ns**  | 527058823529 |
| **BM_Array**              | **0.412 ns**  | **0.408 ns**  | 34329501916  |
| **BM_Vector**             | **3.03 ns**   | **3.01 ns**   | 4666666667   |
| **BM_UnorderedMap**       | **6.58 ns**   | **6.52 ns**   | 2148681055   |
| **BM_ChdDictionary**      | **7.28 ns**   | **7.20 ns**   | 1973568282   |
| **BM_Map**                | **2.37 ns**   | **2.34 ns**   | 5933774834   |
| **BM_CodebooksAPI**       | **66.8 ns**   | **66.4 ns**   | 207407407    |
| **BM_CodebooksVISCall**   | **107337 ns** | **106812 ns** | 128000       |

##### Linux Platform (Clang 16.0.6)

| Benchmark                 | Time         | CPU          | Iterations |
| :------------------------ | :----------- | :----------- | :--------- |
| **BM_CodebooksInstance**  | **0.212 ns** | **0.212 ns** | 66.3B      |
| **BM_CodebooksReference** | **0.210 ns** | **0.210 ns** | 66.9B      |
| **BM_Array**              | **0.699 ns** | **0.699 ns** | 20.0B      |
| **BM_Vector**             | **1.68 ns**  | **1.68 ns**  | 8.4B       |
| **BM_UnorderedMap**       | **2.79 ns**  | **2.79 ns**  | 5.0B       |
| **BM_ChdDictionary**      | **23.6 ns**  | **23.6 ns**  | 589M       |
| **BM_Map**                | **3.04 ns**  | **3.04 ns**  | 4.6B       |
| **BM_CodebooksAPI**       | **76.8 ns**  | **76.8 ns**  | 182M       |
| **BM_CodebooksVISCall**   | **92820 ns** | **92817 ns** | 145K       |

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

### Cross-Platform Performance Comparison (MSVC vs Linux Clang)

| Operation     | Windows C++ (MSVC) | Linux C++ (Clang) | Cross-Platform   |
| :------------ | :----------------- | :---------------- | :--------------- |
| **GMOD Load** | 32.0 ms            | 11.1 ms           | **2.88x faster** |

### Windows GCC vs MSVC Comparison

| Operation     | MSVC 17.14.10 | GCC 14.2.0  | GCC vs MSVC      |
| :------------ | :------------ | :---------- | :--------------- |
| **GMOD Load** | 32.0 ms       | **20.0 ms** | **1.60x faster** |

### GMOD Load Performance Comparison

| C++ Method   | Windows Time (MSVC) | Windows Time (GCC) | Linux Time (Clang) | C# Method | C# Time (Win) | C# Time (Linux) | MSVC vs C# (Windows) | GCC vs C# (Windows) | C++ vs C# (Linux)   |
| :----------- | :------------------ | :----------------- | :----------------- | :-------- | :------------ | :-------------- | :------------------- | :------------------ | :------------------ |
| **gmodLoad** | 32.0 ms             | **20.0 ms**        | 11.1 ms            | **Load**  | 30.40 ms      | 29.68 ms        | ❌ **1.05x slower**  | ✅ **1.52x faster** | ✅ **2.68x faster** |

#### Detailed C++ Results

##### Windows Platform (MSVC 17.14.10)

| Benchmark    | Time    | CPU     | Iterations |
| :----------- | :------ | :------ | :--------- |
| **gmodLoad** | 32.0 ms | 32.0 ms | 361        |

##### Windows Platform (GCC 14.2.0)

| Benchmark    | Time    | CPU     | Iterations |
| :----------- | :------ | :------ | :--------- |
| **gmodLoad** | 20.0 ms | 19.6 ms | 560        |

##### Linux Platform (Clang 16.0.6)

| Benchmark    | Time    | CPU     | Iterations |
| :----------- | :------ | :------ | :--------- |
| **gmodLoad** | 11.1 ms | 11.1 ms | 1251       |

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

### Cross-Platform Performance Comparison (MSVC vs Linux Clang)

| Operation              | Windows C++ (MSVC) | vs Baseline | Linux C++ (Clang) | vs Baseline | Cross-Platform   | Notes                          |
| :--------------------- | :----------------- | :---------: | :---------------- | :---------: | :--------------- | :----------------------------- |
| **StringMap (Dict)**   | 15.7 ns            |  **0.94x**  | 55.6 ns           |  **1.01x**  | **3.54x slower** | ✅ Similar to frozen           |
| **StringMap (Frozen)** | 16.7 ns            |  **1.00x**  | 54.8 ns           |  **1.00x**  | **3.28x slower** | ✅ Legacy baseline method      |
| **HashMap (Robin)**    | 18.3 ns            |  **1.10x**  | 30.2 ns           |  **0.55x**  | **1.65x slower** | ✅ **1.82x faster** Robin Hood |
| **GMOD API Lookup**    | 14.3 ns            |  **0.86x**  | 6.97 ns           |  **0.13x**  | **2.05x faster** | ✅ Direct GMOD API access      |

### Windows GCC vs MSVC Comparison

| Operation              | MSVC 17.14.10 | GCC 14.2.0  | GCC vs MSVC     |
| :--------------------- | :------------ | :---------- | :-------------- |
| **StringMap (Dict)**   | **15.7 ns**   | 65.5 ns     | **4.2x slower** |
| **StringMap (Frozen)** | **16.7 ns**   | 65.3 ns     | **3.9x slower** |
| **HashMap (Robin)**    | 18.3 ns       | **13.9 ns** | **1.3x faster** |
| **GMOD API Lookup**    | 14.3 ns       | **10.1 ns** | **1.4x faster** |

### GMOD Lookup Performance Comparison

#### Cross-Platform C++ Results (MSVC vs Linux Clang)

| C++ Method           | Windows Time (MSVC) | Linux Time (Clang) | Performance Ratio | C# Method (Win) | C# Time (Win) | C# Method (Linux) | C# Time (Linux) | MSVC vs C# (Windows) | C++ vs C# (Linux)   |
| :------------------- | :------------------ | :----------------- | :---------------- | :-------------- | :------------ | :---------------- | :-------------- | :------------------- | :------------------ |
| **StringMap (Dict)** | 15.7 ns             | 55.6 ns            | **3.54x slower**  | **Dict**        | 38.34 ns      | **Dict**          | 37.85 ns        | ✅ **2.44x faster**  | ❌ **1.47x slower** |
| **frozenDict**       | 16.7 ns             | 54.8 ns            | **3.28x slower**  | **FrozenDict**  | 15.21 ns      | **FrozenDict**    | 15.81 ns        | ❌ **1.10x slower**  | ❌ **3.47x slower** |
| **HashMap (Robin)**  | 18.3 ns             | 30.2 ns            | **1.65x slower**  | **FrozenDict**  | 15.21 ns      | **FrozenDict**    | 15.81 ns        | ❌ **1.20x slower**  | ❌ **1.91x slower** |
| **gmod**             | 14.3 ns             | 6.97 ns            | **2.05x faster**  | **Gmod**        | 15.62 ns      | **Gmod**          | 17.07 ns        | ✅ **1.09x faster**  | ✅ **2.45x faster** |

#### Windows GCC vs C# Comparison

| C++ Method (GCC)     | GCC Time    | C# Method      | C# Time (Win) | GCC vs C# (Windows) |
| :------------------- | :---------- | :------------- | :------------ | :------------------ |
| **StringMap (Dict)** | 65.5 ns     | **Dict**       | 38.34 ns      | ❌ **1.7x slower**  |
| **frozenDict**       | 65.3 ns     | **FrozenDict** | 15.21 ns      | ❌ **4.3x slower**  |
| **HashMap (Robin)**  | **13.9 ns** | **FrozenDict** | 15.21 ns      | ✅ **1.1x faster**  |
| **gmod**             | **10.1 ns** | **Gmod**       | 15.62 ns      | ✅ **1.5x faster**  |

#### Detailed C++ Results

##### Windows Platform (MSVC 17.14.10)

| Benchmark      | Time    | CPU     | Iterations |
| :------------- | :------ | :------ | :--------- |
| **Dict**       | 15.7 ns | 15.6 ns | 896M       |
| **FrozenDict** | 16.7 ns | 16.7 ns | 853M       |
| **HashMap**    | 18.3 ns | 18.1 ns | 772M       |
| **Gmod**       | 14.3 ns | 14.3 ns | 973M       |

##### Windows Platform (GCC 14.2.0)

| Benchmark      | Time    | CPU     | Iterations |
| :------------- | :------ | :------ | :--------- |
| **Dict**       | 65.5 ns | 61.5 ns | 237665782  |
| **FrozenDict** | 65.3 ns | 65.0 ns | 213842482  |
| **HashMap**    | 13.9 ns | 13.1 ns | 1006741573 |
| **Gmod**       | 10.1 ns | 9.12 ns | 1483443709 |

##### Linux Platform (Clang 16.0.6)

| Benchmark      | Time    | CPU     | Iterations |
| :------------- | :------ | :------ | :--------- |
| **Dict**       | 55.6 ns | 55.6 ns | 256M       |
| **FrozenDict** | 54.8 ns | 54.7 ns | 258M       |
| **HashMap**    | 30.2 ns | 30.1 ns | 476M       |
| **Gmod**       | 6.97 ns | 6.96 ns | 1.99B      |

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

### Cross-Platform Performance Comparison (MSVC vs Linux Clang)

| Operation                          | Windows C++ (MSVC) | Linux C++ (Clang) | Performance Ratio | Status |
| :--------------------------------- | :----------------- | :---------------- | :---------------- | :----: |
| **TryParse**                       | 19.3 μs            | 11.3 μs           | **1.71x slower**  |   ✅   |
| **TryParseFullPath**               | 9.22 μs            | 4.43 μs           | **2.08x slower**  |   ✅   |
| **TryParseIndividualized**         | 18.3 μs            | 9.17 μs           | **2.00x slower**  |   ✅   |
| **TryParseFullPathIndividualized** | 12.8 μs            | 4.29 μs           | **2.98x slower**  |   ✅   |

### Windows GCC vs MSVC Comparison

| Operation                          | MSVC 17.14.10 | GCC 14.2.0  | GCC vs MSVC     |
| :--------------------------------- | :------------ | :---------- | :-------------- |
| **TryParse**                       | 19.3 μs       | **14.7 μs** | **1.3x faster** |
| **TryParseFullPath**               | 9.22 μs       | **6.29 μs** | **1.5x faster** |
| **TryParseIndividualized**         | 18.3 μs       | **12.8 μs** | **1.4x faster** |
| **TryParseFullPathIndividualized** | 12.8 μs       | **6.20 μs** | **2.1x faster** |

### GMOD Path Parsing Performance Comparison

#### Cross-Platform C++ Results

| C++ Method                         | Windows Time | Linux Time | Performance Ratio | C# Method (Win)                    | C# Time (Win) | C# Method (Linux)                  | C# Time (Linux) | C++ vs C# (Windows) | C++ vs C# (Linux)   |
| :--------------------------------- | :----------- | :--------- | :---------------- | :--------------------------------- | :------------ | :--------------------------------- | :-------------- | :------------------ | :------------------ |
| **tryParse**                       | 19.3 μs      | 11.3 μs    | **1.71x slower**  | **TryParse**                       | 3.77 μs       | **TryParse**                       | 3.55 μs         | ❌ **5.12x slower** | ❌ **3.18x slower** |
| **tryParseFullPath**               | 9.22 μs      | 4.43 μs    | **2.08x slower**  | **TryParseFullPath**               | 571 ns        | **TryParseFullPath**               | 630 ns          | ❌ **16.1x slower** | ❌ **7.03x slower** |
| **tryParseIndividualized**         | 18.3 μs      | 9.17 μs    | **2.00x slower**  | **TryParseIndividualized**         | 1.49 μs       | **TryParseIndividualized**         | 1.60 μs         | ❌ **12.3x slower** | ❌ **5.73x slower** |
| **tryParseFullPathIndividualized** | 12.8 μs      | 4.29 μs    | **2.98x slower**  | **TryParseFullPathIndividualized** | 694 ns        | **TryParseFullPathIndividualized** | 788 ns          | ❌ **18.4x slower** | ❌ **5.44x slower** |

#### Detailed C++ Results

##### Windows Platform

| Benchmark                          | Time    | CPU     | Iterations |
| :--------------------------------- | :------ | :------ | :--------- |
| **tryParse**                       | 19.3 μs | 19.2 μs | 728K       |
| **tryParseFullPath**               | 9.22 μs | 9.18 μs | 1.53M      |
| **tryParseIndividualized**         | 18.3 μs | 18.2 μs | 779K       |
| **tryParseFullPathIndividualized** | 12.8 μs | 12.7 μs | 1.11M      |

##### Linux Platform

| Benchmark                          | Time    | CPU     | Iterations |
| :--------------------------------- | :------ | :------ | :--------- |
| **tryParse**                       | 11.3 μs | 11.3 μs | 1.24M      |
| **tryParseFullPath**               | 4.43 μs | 4.43 μs | 3.17M      |
| **tryParseIndividualized**         | 9.17 μs | 9.16 μs | 1.53M      |
| **tryParseFullPathIndividualized** | 4.29 μs | 4.29 μs | 3.25M      |

#### Detailed C# Results (Windows)

| Method                             | Mean      | Error    | StdDev   | Categories    | Gen0   | Allocated |
| :--------------------------------- | :-------- | :------- | :------- | :------------ | :----- | :-------- |
| **TryParse**                       | 3360.8 ns | 41.66 ns | 38.97 ns | No location   | 0.2213 | 2792 B    |
| **TryParseFullPath**               | 571.2 ns  | 7.45 ns  | 5.82 ns  | No location   | 0.0601 | 760 B     |
| **TryParseIndividualized**         | 1492.2 ns | 28.77 ns | 51.14 ns | With location | 0.2251 | 2,832 B   |
| **TryParseFullPathIndividualized** | 694.1 ns  | 4.33 ns  | 3.84 ns  | With location | 0.0935 | 1176 B    |

#### Detailed C# Results (Linux)

| Method                             | Categories    |      Mean |    Error |   StdDev |   Gen0 | Allocated |
| ---------------------------------- | ------------- | --------: | -------: | -------: | -----: | --------: |
| **TryParseFullPath**               | No location   |  630.4 ns |  2.53 ns |  2.11 ns | 0.0601 |     760 B |
| **TryParse**                       | No location   | 3548.7 ns | 14.72 ns | 11.49 ns | 0.2213 |    2792 B |
| **TryParseFullPathIndividualized** | With location |  787.9 ns |  2.30 ns |  1.92 ns | 0.0935 |    1176 B |
| **TryParseIndividualized**         | With location | 1598.6 ns | 13.64 ns | 12.76 ns | 0.2251 |    2832 B |

---

## GMOD Traversal

### Cross-Platform Performance Comparison (MSVC vs Linux Clang)

| Operation          | Windows C++ (MSVC) | Linux C++ (Clang) |
| :----------------- | :----------------- | :---------------- |
| **Full Traversal** | 201 ms             | 270 ms            |

### Windows GCC vs MSVC Comparison

| Operation          | MSVC 17.14.10 | GCC 14.2.0 |
| :----------------- | :------------ | :--------- |
| **Full Traversal** | 201 ms        | 326 ms     |

### GMOD Traversal Performance Comparison

#### Cross-Platform C++ Results (MSVC vs Linux Clang)

| C++ Method        | Windows Time (MSVC) | Linux Time (Clang) | Performance Ratio | C# Method (Win)   | C# Time (Win) | C# Method (Linux) | C# Time (Linux) | MSVC vs C# (Windows) | C++ vs C# (Linux)   |
| :---------------- | :------------------ | :----------------- | :---------------- | :---------------- | :------------ | :---------------- | :-------------- | :------------------- | :------------------ |
| **fullTraversal** | 201 ms              | 270 ms             | **1.34x slower**  | **FullTraversal** | 135.3 ms      | **FullTraversal** | 161.3 ms        | ❌ **1.49x slower**  | ❌ **1.67x slower** |

#### Windows GCC vs C# Comparison

| C++ Method (GCC)  | GCC Time | C# Method         | C# Time (Win) | GCC vs C# (Windows) | Performance Notes                 |
| :---------------- | :------- | :---------------- | :------------ | :------------------ | :-------------------------------- |
| **fullTraversal** | 326 ms   | **FullTraversal** | 135.3 ms      | ❌ **2.4x slower**  | **C# faster than both compilers** |

#### Detailed C++ Results

##### Windows Platform

| Benchmark         | Time   | CPU    | Iterations |
| :---------------- | :----- | :----- | :--------- |
| **FullTraversal** | 201 ms | 200 ms | 67         |

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

### Cross-Platform Performance Comparison (MSVC vs Linux Clang)

| Operation        | Windows C++ (MSVC) | Linux C++ (Clang) | Status |
| :--------------- | :----------------- | :---------------- | :----: |
| **Convert Path** | 8.07 μs            | 4.22 μs           |   ✅   |

### Windows GCC vs MSVC Comparison

| Operation        | MSVC 17.14.10 | GCC 14.2.0  | GCC vs MSVC     |
| :--------------- | :------------ | :---------- | :-------------- |
| **Convert Path** | 8.07 μs       | **6.57 μs** | **1.2x faster** |

### GMOD Versioning Path Conversion Performance Comparison

#### Cross-Platform C++ Results (MSVC vs Linux Clang)

| C++ Method      | Windows Time (MSVC) | Linux Time (Clang) | Performance Ratio | C# Method (Win) | C# Time (Win) | C# Method (Linux) | C# Time (Linux) | MSVC vs C# (Windows) | C++ vs C# (Linux)   |
| :-------------- | :------------------ | :----------------- | :---------------- | :-------------- | :------------ | :---------------- | :-------------- | :------------------- | :------------------ |
| **convertPath** | 8.07 μs             | 4.22 μs            | **1.91x slower**  | **ConvertPath** | 1.489 μs      | **ConvertPath**   | 1.464 μs        | ❌ **5.42x slower**  | ❌ **2.88x slower** |

#### Windows GCC vs C# Comparison

| C++ Method (GCC) | GCC Time | C# Method       | C# Time (Win) | GCC vs C# (Windows) |
| :--------------- | :------- | :-------------- | :------------ | :------------------ |
| **convertPath**  | 6.57 μs  | **ConvertPath** | 1.489 μs      | ❌ **4.4x slower**  |

#### Detailed C++ Results

##### Windows Platform

| Benchmark       | Time    | CPU     | Iterations |
| :-------------- | :------ | :------ | :--------- |
| **convertPath** | 8.07 μs | 8.07 μs | 1.72M      |

##### Linux Platform

| Benchmark       | Time    | CPU     | Iterations |
| :-------------- | :------ | :------ | :--------- |
| **convertPath** | 4.22 μs | 4.21 μs | 3.45M      |

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

### Cross-Platform Performance Comparison (MSVC vs Linux Clang)

| Operation                 | Windows C++ (MSVC) | Linux C++ (Clang) | Performance Ratio | Status |
| :------------------------ | :----------------- | :---------------- | :---------------- | :----: |
| **bcl (400)**             | 1.23 ns            | 2.72 ns           | **2.21x slower**  |   ✅   |
| **bcl (H346)**            | 2.68 ns            | 2.52 ns           | **1.06x faster**  |   ✅   |
| **bclOrd (400)**          | 1.87 ns            | 0.931 ns          | **2.01x faster**  |   ✅   |
| **bclOrd (H346)**         | 3.19 ns            | 2.02 ns           | **1.58x faster**  |   ✅   |
| **Larson (400)**          | 1.84 ns            | 1.60 ns           | **1.15x faster**  |   ✅   |
| **Larson (H346)**         | 4.94 ns            | 3.43 ns           | **1.44x faster**  |   ✅   |
| **crc32 (400)**           | 1.51 ns            | 1.38 ns           | **1.09x faster**  |   ✅   |
| **crc32 (H346)**          | 4.66 ns            | 4.22 ns           | **1.10x faster**  |   ✅   |
| **fnv (400)**             | 1.52 ns            | 1.17 ns           | **1.30x faster**  |   ✅   |
| **fnv (H346)**            | 3.43 ns            | 2.61 ns           | **1.31x faster**  |   ✅   |
| **HashCode (400)**        | 1.54 ns            | -                 | -                 |   ✅   |
| **HashCode (H346_11112)** | 3.32 ns            | -                 | -                 |   ✅   |

### Windows GCC vs MSVC Comparison

| Operation                  | MSVC 17.14.10 | GCC 14.2.0   | GCC vs MSVC     | Status |
| :------------------------- | :------------ | :----------- | :-------------- | :----: |
| **bcl (400)**              | **1.23 ns**   | 2.90 ns      | **2.4x slower** |   ❌   |
| **bcl (H346)**             | **2.68 ns**   | 2.95 ns      | **1.1x slower** |   ⚠️   |
| **bclOrd (400)**           | 1.87 ns       | **0.877 ns** | **2.1x faster** |   ✅   |
| **bclOrd (H346)**          | 3.19 ns       | **2.32 ns**  | **1.4x faster** |   ✅   |
| **Larson (400)**           | 1.84 ns       | **1.44 ns**  | **1.3x faster** |   ✅   |
| **Larson (H346)**          | **4.94 ns**   | 6.04 ns      | **1.2x slower** |   ❌   |
| **crc32 (400)**            | 1.51 ns       | **1.36 ns**  | **1.1x faster** |   ✅   |
| **crc32 (H346)**           | 4.66 ns       | **4.35 ns**  | **1.1x faster** |   ✅   |
| **fnv (400)**              | 1.52 ns       | **1.18 ns**  | **1.3x faster** |   ✅   |
| **fnv (H346)**             | 3.43 ns       | **2.53 ns**  | **1.4x faster** |   ✅   |
| **BM_HashCode_400**        | 1.54 ns       |              |                 |        |
| **BM_HashCode_H346_11112** | 3.32 ns       |              |                 |        |

### Short String Hashing Performance Comparison

#### Cross-Platform C++ Results

| Algorithm           | Windows Time | Linux Time | Performance Ratio | C# Baseline (Win) | C# Baseline (Linux) | C++ vs C# (Windows) | C++ vs C# (Linux)   |
| :------------------ | :----------- | :--------- | :---------------- | :---------------- | :------------------ | :------------------ | :------------------ |
| **bcl (400)**       | 1.21 ns      | 2.72 ns    | **2.25x slower**  | 1.135 ns          | 1.214 ns            | ❌ **1.07x slower** | ❌ **2.24x slower** |
| **bcl (H346)**      | 2.63 ns      | 2.52 ns    | **1.04x faster**  | 4.551 ns          | 4.560 ns            | ✅ **1.73x faster** | ✅ **1.81x faster** |
| **bclOrd (400)**    | 1.83 ns      | 0.931 ns   | **1.96x faster**  | 1.514 ns          | 1.368 ns            | ❌ **1.21x slower** | ✅ **1.47x faster** |
| **bclOrd (H346)**   | 3.13 ns      | 2.02 ns    | **1.55x faster**  | 3.127 ns          | 2.340 ns            | ❌ **1.00x equal**  | ✅ **1.16x faster** |
| **Larson (400)**    | 1.81 ns      | 1.60 ns    | **1.13x faster**  | 1.219 ns          | 1.244 ns            | ❌ **1.48x slower** | ❌ **1.29x slower** |
| **Larson (H346)**   | 4.85 ns      | 3.43 ns    | **1.41x faster**  | 3.349 ns          | 3.494 ns            | ❌ **1.45x slower** | ✅ **1.02x faster** |
| **crc32 (400)**     | 1.46 ns      | 1.38 ns    | **1.06x faster**  | 1.215 ns          | 1.235 ns            | ❌ **1.20x slower** | ❌ **1.12x slower** |
| **crc32 (H346)**    | 4.56 ns      | 4.22 ns    | **1.08x faster**  | 3.259 ns          | 3.441 ns            | ❌ **1.40x slower** | ❌ **1.23x slower** |
| **fnv (400)**       | 1.48 ns      | 1.17 ns    | **1.26x faster**  | 1.205 ns          | 1.291 ns            | ❌ **1.23x slower** | ✅ **1.10x faster** |
| **fnv (H346)**      | 3.36 ns      | 2.61 ns    | **1.29x faster**  | 3.337 ns          | 3.741 ns            | ❌ **1.01x equal**  | ✅ **1.43x faster** |
| **HashCode (400)**  | 1.54 ns      | -          | -                 | 1.135 ns          | 1.214 ns            | ❌ **4.24x slower** | ❌ **3.96x slower** |
| **HashCode (H346)** | 3.32 ns      | -          | -                 | 4.551 ns          | 4.560 ns            | ❌ **3.71x slower** | ❌ **3.71x slower** |

#### Detailed C++ Results

##### Windows Platform

| Benchmark               | Input      | Time    | CPU     | Iterations |
| :---------------------- | :--------- | :------ | :------ | :--------- |
| **bcl_400**             | 400        | 1.21 ns | 1.20 ns | 11.5B      |
| **bcl_H346_11112**      | H346.11112 | 2.63 ns | 2.60 ns | 5.24B      |
| **bclOrd_400**          | 400        | 1.83 ns | 1.82 ns | 7.79B      |
| **bclOrd_H346_11112**   | H346.11112 | 3.13 ns | 3.09 ns | 4.53B      |
| **Larson_400**          | 400        | 1.81 ns | 1.80 ns | 7.86B      |
| **Larson_H346_11112**   | H346.11112 | 4.85 ns | 4.79 ns | 2.98B      |
| **crc32Intrinsic_400**  | 400        | 1.46 ns | 1.45 ns | 9.53B      |
| **crc32Intrinsic_H346** | H346.11112 | 4.56 ns | 4.51 ns | 3.05B      |
| **fnv_400**             | 400        | 1.48 ns | 1.47 ns | 9.33B      |
| **fnv_H346_11112**      | H346.11112 | 3.36 ns | 3.34 ns | 4.17B      |
| **HashCode_400**        | 400        | 1.54 ns | 4.77 ns | 2.94B      |
| **HashCode_H346_11112** | H346.11112 | 3.32 ns | 16.6 ns | 853M       |

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

#### Detailed C# Results (Windows)

| Method             | Input      |     Mean |     Error |    StdDev |        Ratio | RatioSD | Rank | Allocated | Alloc Ratio |
| :----------------- | :--------- | -------: | --------: | --------: | -----------: | ------: | ---: | --------: | ----------: |
| **HashCode**       | 400        | 4.316 ns | 0.0369 ns | 0.0345 ns | 3.58x slower |   0.06x |    6 |         - |          NA |
| **BclOrd**         | 400        | 1.596 ns | 0.0185 ns | 0.0164 ns | 1.32x slower |   0.02x |    5 |         - |          NA |
| **Fnv**            | 400        | 1.285 ns | 0.0263 ns | 0.0219 ns | 1.07x slower |   0.03x |    4 |         - |          NA |
| **Larsson**        | 400        | 1.265 ns | 0.0195 ns | 0.0152 ns | 1.05x slower |   0.02x |    3 |         - |          NA |
| **Crc32Intrinsic** | 400        | 1.221 ns | 0.0084 ns | 0.0079 ns | 1.01x slower |   0.02x |    2 |         - |          NA |
| **Bcl**            | 400        | 1.205 ns | 0.0201 ns | 0.0178 ns |     baseline |         |    1 |         - |          NA |
|                    |            |          |           |           |              |         |      |           |             |
| **HashCode**       | H346.11112 | 7.438 ns | 0.0717 ns | 0.0598 ns | 1.62x slower |   0.02x |    6 |         - |          NA |
| **Bcl**            | H346.11112 | 4.604 ns | 0.0232 ns | 0.0205 ns |     baseline |         |    5 |         - |          NA |
| **Larsson**        | H346.11112 | 3.451 ns | 0.0233 ns | 0.0218 ns | 1.33x faster |   0.01x |    4 |         - |          NA |
| **Fnv**            | H346.11112 | 3.381 ns | 0.0138 ns | 0.0122 ns | 1.36x faster |   0.01x |    3 |         - |          NA |
| **Crc32Intrinsic** | H346.11112 | 3.323 ns | 0.0598 ns | 0.0499 ns | 1.39x faster |   0.02x |    2 |         - |          NA |
| **BclOrd**         | H346.11112 | 3.203 ns | 0.0197 ns | 0.0165 ns | 1.44x faster |   0.01x |    1 |         - |          NA |

---

_Last updated: August 3, 2025_
