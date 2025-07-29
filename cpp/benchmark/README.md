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
| **Hash Table Operations**    | **1.10-2.28x faster**            |   ✅   |
| **Codebook Access (Direct)** | **1.04x slower** vs C# baseline  |   ⚠️   |
| **Codebook Access (API)**    | **1,287x slower** vs C# baseline |   ⚠️   |
| **String Hashing (Short)**   | **1.03-1.70x mixed**             |   ✅   |
| **String Hashing (Long)**    | **1.03x slower to 1.51x slower** |   ⚠️   |
| **GMOD Loading**             | **1.12x slower**                 |   ⚠️   |
| **GMOD Lookup**              | **1.10-2.28x faster**            |   ✅   |
| **GMOD Lookup (HashMap)**    | **1.28x slower**                 |   ❌   |
| **GMOD Traversal**           | **1.54x slower**                 |   ❌   |
| **Path Parsing**             | **5.81-17.0x slower**            |  ❌❌  |
| **Version Path Conversion**  | **5.55x slower**                 |  ❌❌  |

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
| **Codebook Access (Direct)** | **24.8x faster**        |   ✅   |
| **Codebook Access (API)**    | **13.4x faster**        |   ✅   |
| **GMOD Loading**             | **1.70x faster**        |   ✅   |
| **Path Parsing**             | **1.3-1.6x faster**     |   ✅   |
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
| **Hash Table Operations**    | **1.13-11.9x faster**  |   ✅   |
| **Codebook Access (Direct)** | **3.02x faster**       |   ✅   |
| **Codebook Access (API)**    | **10.4x faster**       |   ✅   |
| **String Hashing (Short)**   | **1.04-2.31x mixed**   |   ⚠️   |
| **String Hashing (Long)**    | **1.12-1.32x faster**  |   ✅   |
| **GMOD Loading**             | **2.59x faster**       |   ✅   |
| **GMOD Lookup**              | **1.45-7.91x mixed**   |   ⚠️   |
| **GMOD Lookup (HashMap)**    | **Same performance**   |   ✅   |
| **GMOD Traversal**           | **1.03x slower**       |   ❌   |
| **Path Parsing**             | **1.73-2.04x faster**  |   ✅   |
| **Version Path Conversion**  | **6.92x faster**       |   ✅   |

---

## Codebooks Lookup

### Cross-Platform Performance Comparison (MSVC vs Linux Clang)

**Note:** Windows results are from MSVC 17.14.10. See GCC section below for GCC 14.2.0 comparison.

Performance comparison between different C++ access methods for Vista SDK codebook operations:

| Operation             | Windows C++ (MSVC) | vs Baseline | Linux C++ (Clang) | vs Baseline | Cross-Platform   |
| :-------------------- | :----------------- | :---------: | :---------------- | :---------: | :--------------- |
| **SDK Instance**      | 0.669 ns           |  **1.00x**  | 0.212 ns          |  **1.00x**  | **3.16x faster** |
| **SDK Reference**     | 0.841 ns           |  **1.26x**  | 0.210 ns          |  **0.99x**  | **4.00x faster** |
| **Array Lookup**      | 1.56 ns            |  **2.33x**  | 0.699 ns          |  **3.30x**  | **2.23x faster** |
| **Vector Lookup**     | 1.80 ns            |  **2.69x**  | 1.68 ns           |  **7.92x**  | **1.07x faster** |
| **Hash Table Lookup** | 2.13 ns            |  **3.18x**  | 2.79 ns           | **13.16x**  | **1.31x slower** |
| **CHD Dictionary**    | 35.9 ns            |  **53.7x**  | 23.6 ns           |  **111x**   | **1.52x faster** |
| **Tree Lookup**       | 7.63 ns            |  **11.4x**  | 3.04 ns           | **14.34x**  | **2.51x faster** |
| **SDK API Method**    | 893 ns             |  **1335x**  | 76.8 ns           |  **362x**   | **11.6x faster** |
| **VIS Singleton**     | 117860 ns          | **176168x** | 92820 ns          | **437830x** | **1.27x faster** |

### Windows GCC vs MSVC Comparison

| Operation             | MSVC 17.14.10 | GCC 14.2.0    | GCC vs MSVC      | GCC Performance Notes                 |
| :-------------------- | :------------ | :------------ | :--------------- | :------------------------------------ |
| **SDK Instance**      | 0.669 ns      | **0.027 ns**  | **24.8x faster** | **Exceptional optimization**          |
| **SDK Reference**     | 0.841 ns      | **0.027 ns**  | **31.1x faster** | **Exceptional optimization**          |
| **Array Lookup**      | 1.56 ns       | **0.412 ns**  | **3.8x faster**  | **Strong improvement**                |
| **Vector Lookup**     | **1.80 ns**   | 3.03 ns       | **1.7x slower**  | **GCC STL implementation difference** |
| **Hash Table Lookup** | **2.13 ns**   | 6.58 ns       | **3.1x slower**  | **GCC STL implementation difference** |
| **CHD Dictionary**    | 35.9 ns       | **7.28 ns**   | **4.9x faster**  | **Significant improvement**           |
| **Tree Lookup**       | 7.63 ns       | **2.37 ns**   | **3.2x faster**  | **Good improvement**                  |
| **SDK API Method**    | 893 ns        | **66.8 ns**   | **13.4x faster** | **Dramatic optimization**             |
| **VIS Singleton**     | 117860 ns     | **107337 ns** | **1.10x faster** | **Modest improvement**                |

#### STL Container Performance (MSVC vs Linux Clang)

| C++ Method        | Windows Time (MSVC) | Linux Time (Clang) | Performance Ratio | Implementation                      | Performance vs Array | Status |
| :---------------- | :------------------ | :----------------- | :---------------- | :---------------------------------- | :------------------- | :----- |
| **Array**         | 1.56 ns             | 0.699 ns           | **2.23x faster**  | `std::array` linear search          | **Baseline**         | ✅     |
| **Vector**        | 1.80 ns             | 1.68 ns            | **1.07x faster**  | `std::vector` linear search         | **1.15x slower**     | ✅     |
| **UnorderedMap**  | 2.13 ns             | 2.79 ns            | **1.31x slower**  | `std::unordered_map::find()`        | **1.37x slower**     | ✅     |
| **ChdDictionary** | 35.9 ns             | 23.6 ns            | **1.52x faster**  | CHD perfect hash table              | **23.0x slower**     | ⚠️     |
| **Map**           | 7.63 ns             | 3.04 ns            | **2.51x faster**  | `std::map::find()` (red-black tree) | **4.89x slower**     | ⚠️     |

#### STL Container Performance (Windows: GCC vs MSVC)

| C++ Method        | MSVC 17.14.10 | GCC 14.2.0   | GCC vs MSVC     | Implementation Notes                           | Status |
| :---------------- | :------------ | :----------- | :-------------- | :--------------------------------------------- | :----- |
| **Array**         | 1.56 ns       | **0.412 ns** | **3.8x faster** | GCC better optimization                        | ✅     |
| **Vector**        | **1.80 ns**   | 3.03 ns      | **1.7x slower** | **GCC STL vector implementation difference**   | ⚠️     |
| **UnorderedMap**  | **2.13 ns**   | 6.58 ns      | **3.1x slower** | **GCC STL hash map implementation difference** | ⚠️     |
| **ChdDictionary** | 35.9 ns       | **7.28 ns**  | **4.9x faster** | GCC excellent optimization                     | ✅     |
| **Map**           | 7.63 ns       | **2.37 ns**  | **3.2x faster** | GCC red-black tree optimization                | ✅     |

#### Vista SDK Access Performance (MSVC vs Linux Clang)

| C++ Method             | Windows Time (MSVC) | Linux Time (Clang) | Performance Ratio | Implementation                     | Performance vs Array | Status |
| :--------------------- | :------------------ | :----------------- | :---------------- | :--------------------------------- | :------------------- | :----- |
| **CodebooksInstance**  | **0.669 ns**        | **0.212 ns**       | **3.16x faster**  | Owned instance + direct access     | **2.33x faster**     | ✅     |
| **CodebooksReference** | **0.841 ns**        | **0.210 ns**       | **4.00x faster**  | Reference wrapper + direct access  | **1.85x faster**     | ✅     |
| **CodebooksAPI**       | **893 ns**          | **76.8 ns**        | **11.6x faster**  | `codebook()` method call           | **572x slower**      | ⚠️     |
| **CodebooksVISCall**   | **117860 ns**       | **92820 ns**       | **1.27x faster**  | `VIS::instance().codebooks()` call | **75551x slower**    | ❌     |

#### Vista SDK Access Performance (Windows: GCC vs MSVC)

| C++ Method             | MSVC 17.14.10 | GCC 14.2.0    | GCC vs MSVC      | Performance Notes                       | Status |
| :--------------------- | :------------ | :------------ | :--------------- | :-------------------------------------- | :----- |
| **CodebooksInstance**  | 0.669 ns      | **0.027 ns**  | **24.8x faster** | **Near-complete optimization**          | ✅     |
| **CodebooksReference** | 0.841 ns      | **0.027 ns**  | **31.1x faster** | **Near-complete optimization**          | ✅     |
| **CodebooksAPI**       | 893 ns        | **66.8 ns**   | **13.4x faster** | **Dramatic method call optimization**   | ✅     |
| **CodebooksVISCall**   | 117860 ns     | **107337 ns** | **1.10x faster** | **Modest singleton access improvement** | ✅     |

#### Detailed C++ Results

##### Windows Platform (MSVC 17.14.10)

| Benchmark                 | Time          | CPU           | Iterations |
| :------------------------ | :------------ | :------------ | :--------- |
| **BM_CodebooksInstance**  | **0.669 ns**  | **0.666 ns**  | 21.54B     |
| **BM_CodebooksReference** | **0.841 ns**  | **0.838 ns**  | 16.69B     |
| **BM_Array**              | **1.56 ns**   | **1.54 ns**   | 9.24B      |
| **BM_Vector**             | **1.80 ns**   | **1.79 ns**   | 7.53B      |
| **BM_UnorderedMap**       | **2.13 ns**   | **2.12 ns**   | 6.69B      |
| **BM_ChdDictionary**      | **35.9 ns**   | **35.7 ns**   | 395M       |
| **BM_Map**                | **7.63 ns**   | **7.57 ns**   | 1.85B      |
| **BM_CodebooksAPI**       | **893 ns**    | **888 ns**    | 15.86M     |
| **BM_CodebooksVISCall**   | **117860 ns** | **116664 ns** | 119K       |

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

| Operation     | Windows C++ (MSVC) | vs Baseline | Linux C++ (Clang) | vs Baseline | Cross-Platform   |
| :------------ | :----------------- | :---------: | :---------------- | :---------: | :--------------- |
| **GMOD Load** | 34.0 ms            |  **1.00x**  | 11.1 ms           |  **1.00x**  | **3.06x faster** |

### Windows GCC vs MSVC Comparison

| Operation     | MSVC 17.14.10 | GCC 14.2.0  | GCC vs MSVC      | Performance Notes           |
| :------------ | :------------ | :---------- | :--------------- | :-------------------------- |
| **GMOD Load** | 34.0 ms       | **20.0 ms** | **1.70x faster** | **Significant improvement** |

### GMOD Load Performance Comparison

| C++ Method   | Windows Time (MSVC) | Windows Time (GCC) | Linux Time (Clang) | C# Method | C# Time (Win) | C# Time (Linux) | MSVC vs C# (Windows) | GCC vs C# (Windows) | C++ vs C# (Linux)   |
| :----------- | :------------------ | :----------------- | :----------------- | :-------- | :------------ | :-------------- | :------------------- | :------------------ | :------------------ |
| **gmodLoad** | 34.0 ms             | **20.0 ms**        | 11.1 ms            | **Load**  | 30.40 ms      | 29.68 ms        | ❌ **1.12x slower**  | ✅ **1.52x faster** | ✅ **2.68x faster** |

#### Detailed C++ Results

##### Windows Platform (MSVC 17.14.10)

| Benchmark    | Time    | CPU     | Iterations |
| :----------- | :------ | :------ | :--------- |
| **gmodLoad** | 34.0 ms | 33.8 ms | 365        |

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
| **StringMap (Dict)**   | 16.8 ns            |  **0.99x**  | 55.6 ns           |  **1.01x**  | **3.31x slower** | ✅ Similar to frozen           |
| **StringMap (Frozen)** | 16.9 ns            |  **1.00x**  | 54.8 ns           |  **1.00x**  | **3.24x slower** | ✅ Legacy baseline method      |
| **HashMap (Robin)**    | 16.8 ns            |  **0.99x**  | 30.2 ns           |  **0.55x**  | **1.80x slower** | ✅ **1.82x faster** Robin Hood |
| **GMOD API Lookup**    | 17.6 ns            |  **1.04x**  | 6.97 ns           |  **0.13x**  | **2.52x faster** | ✅ Direct GMOD API access      |

### Windows GCC vs MSVC Comparison

| Operation              | MSVC 17.14.10 | GCC 14.2.0  | GCC vs MSVC     | Performance Notes               |
| :--------------------- | :------------ | :---------- | :-------------- | :------------------------------ |
| **StringMap (Dict)**   | **16.8 ns**   | 65.5 ns     | **3.9x slower** | **GCC STL hash map slower**     |
| **StringMap (Frozen)** | **16.9 ns**   | 65.3 ns     | **3.9x slower** | **GCC STL hash map slower**     |
| **HashMap (Robin)**    | 16.8 ns       | **13.9 ns** | **1.2x faster** | **GCC Robin Hood optimization** |
| **GMOD API Lookup**    | 17.6 ns       | **10.1 ns** | **1.7x faster** | **GCC API optimization**        |

### GMOD Lookup Performance Comparison

#### Cross-Platform C++ Results (MSVC vs Linux Clang)

| C++ Method           | Windows Time (MSVC) | Linux Time (Clang) | Performance Ratio | C# Method (Win) | C# Time (Win) | C# Method (Linux) | C# Time (Linux) | MSVC vs C# (Windows) | C++ vs C# (Linux)   |
| :------------------- | :------------------ | :----------------- | :---------------- | :-------------- | :------------ | :---------------- | :-------------- | :------------------- | :------------------ |
| **StringMap (Dict)** | 16.3 ns             | 55.6 ns            | **3.41x slower**  | **Dict**        | 38.34 ns      | **Dict**          | 37.85 ns        | ✅ **2.35x faster**  | ❌ **1.47x slower** |
| **frozenDict**       | 16.4 ns             | 54.8 ns            | **3.34x slower**  | **FrozenDict**  | 15.21 ns      | **FrozenDict**    | 15.81 ns        | ❌ **1.08x slower**  | ❌ **3.47x slower** |
| **HashMap (Robin)**  | 19.4 ns             | 30.2 ns            | **1.56x slower**  | **FrozenDict**  | 15.21 ns      | **FrozenDict**    | 15.81 ns        | ❌ **1.28x slower**  | ❌ **1.91x slower** |
| **gmod**             | 15.2 ns             | 6.97 ns            | **2.18x faster**  | **Gmod**        | 15.62 ns      | **Gmod**          | 17.07 ns        | ✅ **1.03x faster**  | ✅ **2.45x faster** |

#### Windows GCC vs C# Comparison

| C++ Method (GCC)     | GCC Time    | C# Method      | C# Time (Win) | GCC vs C# (Windows) | Performance Notes              |
| :------------------- | :---------- | :------------- | :------------ | :------------------ | :----------------------------- |
| **StringMap (Dict)** | 65.5 ns     | **Dict**       | 38.34 ns      | ❌ **1.7x slower**  | **C# Dictionary faster**       |
| **frozenDict**       | 65.3 ns     | **FrozenDict** | 15.21 ns      | ❌ **4.3x slower**  | **C# FrozenDict much faster**  |
| **HashMap (Robin)**  | **13.9 ns** | **FrozenDict** | 15.21 ns      | ✅ **1.1x faster**  | **GCC Robin Hood competitive** |
| **gmod**             | **10.1 ns** | **Gmod**       | 15.62 ns      | ✅ **1.5x faster**  | **GCC GMOD API excellent**     |

#### Detailed C++ Results

##### Windows Platform (MSVC 17.14.10)

| Benchmark      | Time    | CPU     | Iterations |
| :------------- | :------ | :------ | :--------- |
| **Dict**       | 16.3 ns | 16.2 ns | 887M       |
| **FrozenDict** | 16.4 ns | 16.3 ns | 862M       |
| **HashMap**    | 19.4 ns | 19.3 ns | 734M       |
| **Gmod**       | 15.2 ns | 15.1 ns | 924M       |

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
| **TryParse**                       | 21.9 μs            | 11.3 μs           | **1.94x slower**  |   ✅   |
| **TryParseFullPath**               | 9.68 μs            | 4.43 μs           | **2.19x slower**  |   ✅   |
| **TryParseIndividualized**         | 19.9 μs            | 9.17 μs           | **2.17x slower**  |   ✅   |
| **TryParseFullPathIndividualized** | 9.56 μs            | 4.29 μs           | **2.23x slower**  |   ✅   |

### Windows GCC vs MSVC Comparison

| Operation                          | MSVC 17.14.10 | GCC 14.2.0  | GCC vs MSVC     | Performance Notes           |
| :--------------------------------- | :------------ | :---------- | :-------------- | :-------------------------- |
| **TryParse**                       | 21.9 μs       | **14.7 μs** | **1.5x faster** | **Significant improvement** |
| **TryParseFullPath**               | 9.68 μs       | **6.29 μs** | **1.5x faster** | **Good improvement**        |
| **TryParseIndividualized**         | 19.9 μs       | **12.8 μs** | **1.6x faster** | **Notable improvement**     |
| **TryParseFullPathIndividualized** | 9.56 μs       | **6.20 μs** | **1.5x faster** | **Consistent improvement**  |

### GMOD Path Parsing Performance Comparison

#### Cross-Platform C++ Results

| C++ Method                         | Windows Time | Linux Time | Performance Ratio | C# Method (Win)                    | C# Time (Win) | C# Method (Linux)                  | C# Time (Linux) | C++ vs C# (Windows) | C++ vs C# (Linux)   |
| :--------------------------------- | :----------- | :--------- | :---------------- | :--------------------------------- | :------------ | :--------------------------------- | :-------------- | :------------------ | :------------------ |
| **tryParse**                       | 20.0 μs      | 11.3 μs    | **1.77x slower**  | **TryParse**                       | 3.77 μs       | **TryParse**                       | 3.55 μs         | ❌ **5.31x slower** | ❌ **3.18x slower** |
| **tryParseFullPath**               | 9.21 μs      | 4.43 μs    | **2.08x slower**  | **TryParseFullPath**               | 571 ns        | **TryParseFullPath**               | 630 ns          | ❌ **16.1x slower** | ❌ **7.03x slower** |
| **tryParseIndividualized**         | 18.8 μs      | 9.17 μs    | **2.05x slower**  | **TryParseIndividualized**         | 1.49 μs       | **TryParseIndividualized**         | 1.60 μs         | ❌ **12.6x slower** | ❌ **5.73x slower** |
| **tryParseFullPathIndividualized** | 9.11 μs      | 4.29 μs    | **2.12x slower**  | **TryParseFullPathIndividualized** | 694 ns        | **TryParseFullPathIndividualized** | 788 ns          | ❌ **13.1x slower** | ❌ **5.44x slower** |

#### Detailed C++ Results

##### Windows Platform

| Benchmark                          | Time    | CPU     | Iterations |
| :--------------------------------- | :------ | :------ | :--------- |
| **tryParse**                       | 20.0 μs | 19.9 μs | 700K       |
| **tryParseFullPath**               | 8.60 μs | 8.55 μs | 1.64M      |
| **tryParseIndividualized**         | 18.0 μs | 17.9 μs | 779K       |
| **tryParseFullPathIndividualized** | 8.56 μs | 8.51 μs | 1.64M      |

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
| **Full Traversal** | 209 ms             | 270 ms            |

### Windows GCC vs MSVC Comparison

| Operation          | MSVC 17.14.10 | GCC 14.2.0 |
| :----------------- | :------------ | :--------- |
| **Full Traversal** | 209 ms        | 326 ms     |

### GMOD Traversal Performance Comparison

#### Cross-Platform C++ Results (MSVC vs Linux Clang)

| C++ Method        | Windows Time (MSVC) | Linux Time (Clang) | Performance Ratio | C# Method (Win)   | C# Time (Win) | C# Method (Linux) | C# Time (Linux) | MSVC vs C# (Windows) | C++ vs C# (Linux)   |
| :---------------- | :------------------ | :----------------- | :---------------- | :---------------- | :------------ | :---------------- | :-------------- | :------------------- | :------------------ |
| **fullTraversal** | 209 ms              | 270 ms             | **1.29x slower**  | **FullTraversal** | 135.3 ms      | **FullTraversal** | 161.3 ms        | ❌ **1.54x slower**  | ❌ **1.67x slower** |

#### Windows GCC vs C# Comparison

| C++ Method (GCC)  | GCC Time | C# Method         | C# Time (Win) | GCC vs C# (Windows) | Performance Notes                 |
| :---------------- | :------- | :---------------- | :------------ | :------------------ | :-------------------------------- |
| **fullTraversal** | 326 ms   | **FullTraversal** | 135.3 ms      | ❌ **2.4x slower**  | **C# faster than both compilers** |

#### Detailed C++ Results

##### Windows Platform

| Benchmark         | Time   | CPU    | Iterations |
| :---------------- | :----- | :----- | :--------- |
| **FullTraversal** | 209 ms | 211 ms | 66         |

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

| Operation        | Windows C++ (MSVC) | Linux C++ (Clang) | Performance Ratio | Status |
| :--------------- | :----------------- | :---------------- | :---------------- | :----: |
| **Convert Path** | 8.48 μs            | 4.22 μs           | **2.01x slower**  |   ✅   |

### Windows GCC vs MSVC Comparison

| Operation        | MSVC 17.14.10 | GCC 14.2.0  | GCC vs MSVC     |
| :--------------- | :------------ | :---------- | :-------------- |
| **Convert Path** | 8.48 μs       | **6.57 μs** | **1.3x faster** |

### GMOD Versioning Path Conversion Performance Comparison

#### Cross-Platform C++ Results (MSVC vs Linux Clang)

| C++ Method      | Windows Time (MSVC) | Linux Time (Clang) | Performance Ratio | C# Method (Win) | C# Time (Win) | C# Method (Linux) | C# Time (Linux) | MSVC vs C# (Windows) | C++ vs C# (Linux)   |
| :-------------- | :------------------ | :----------------- | :---------------- | :-------------- | :------------ | :---------------- | :-------------- | :------------------- | :------------------ |
| **convertPath** | 8.45 μs             | 4.22 μs            | **2.00x slower**  | **ConvertPath** | 1.489 μs      | **ConvertPath**   | 1.464 μs        | ❌ **5.68x slower**  | ❌ **2.88x slower** |

#### Windows GCC vs C# Comparison

| C++ Method (GCC) | GCC Time | C# Method       | C# Time (Win) | GCC vs C# (Windows) |
| :--------------- | :------- | :-------------- | :------------ | :------------------ |
| **convertPath**  | 6.57 μs  | **ConvertPath** | 1.489 μs      | ❌ **4.4x slower**  |

#### Detailed C++ Results

##### Windows Platform

| Benchmark       | Time    | CPU     | Iterations |
| :-------------- | :------ | :------ | :--------- |
| **convertPath** | 8.45 μs | 8.41 μs | 1.67M      |

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

| Operation         | Windows C++ (MSVC) | Linux C++ (Clang) | Performance Ratio | Status | Notes                      |
| :---------------- | :----------------- | :---------------- | :---------------- | :----: | :------------------------- |
| **bcl (400)**     | 1.23 ns            | 2.72 ns           | **2.21x slower**  |   ✅   | Linux significantly slower |
| **bcl (H346)**    | 2.68 ns            | 2.52 ns           | **1.06x faster**  |   ✅   | Similar performance        |
| **bclOrd (400)**  | 1.87 ns            | 0.931 ns          | **2.01x faster**  |   ✅   | Linux advantage            |
| **bclOrd (H346)** | 3.19 ns            | 2.02 ns           | **1.58x faster**  |   ✅   | Strong Linux performance   |
| **Larson (400)**  | 1.84 ns            | 1.60 ns           | **1.15x faster**  |   ✅   | Slight Windows advantage   |
| **Larson (H346)** | 4.94 ns            | 3.43 ns           | **1.44x faster**  |   ✅   | Linux improvement          |
| **crc32 (400)**   | 1.51 ns            | 1.38 ns           | **1.09x faster**  |   ✅   | Slight Linux advantage     |
| **crc32 (H346)**  | 4.66 ns            | 4.22 ns           | **1.10x faster**  |   ✅   | Marginal improvement       |
| **fnv (400)**     | 1.52 ns            | 1.17 ns           | **1.30x faster**  |   ✅   | Linux faster               |
| **fnv (H346)**    | 3.43 ns            | 2.61 ns           | **1.31x faster**  |   ✅   | Good Linux performance     |

### Windows GCC vs MSVC Comparison

| Operation         | MSVC 17.14.10 | GCC 14.2.0   | GCC vs MSVC     | Status | Notes                     |
| :---------------- | :------------ | :----------- | :-------------- | :----: | :------------------------ |
| **bcl (400)**     | **1.23 ns**   | 2.90 ns      | **2.4x slower** |   ❌   | **GCC BCL slower**        |
| **bcl (H346)**    | **2.68 ns**   | 2.95 ns      | **1.1x slower** |   ⚠️   | **Slight regression**     |
| **bclOrd (400)**  | 1.87 ns       | **0.877 ns** | **2.1x faster** |   ✅   | **GCC BclOrd excellent**  |
| **bclOrd (H346)** | 3.19 ns       | **2.32 ns**  | **1.4x faster** |   ✅   | **Good improvement**      |
| **Larson (400)**  | 1.84 ns       | **1.44 ns**  | **1.3x faster** |   ✅   | **Moderate improvement**  |
| **Larson (H346)** | **4.94 ns**   | 6.04 ns      | **1.2x slower** |   ❌   | **GCC Larson regression** |
| **crc32 (400)**   | 1.51 ns       | **1.36 ns**  | **1.1x faster** |   ✅   | **Slight improvement**    |
| **crc32 (H346)**  | 4.66 ns       | **4.35 ns**  | **1.1x faster** |   ✅   | **Minor improvement**     |
| **fnv (400)**     | 1.52 ns       | **1.18 ns**  | **1.3x faster** |   ✅   | **Good improvement**      |
| **fnv (H346)**    | 3.43 ns       | **2.53 ns**  | **1.4x faster** |   ✅   | **Notable improvement**   |

### Short String Hashing Performance Comparison

#### Cross-Platform C++ Results

| Algorithm         | Windows Time | Linux Time | Performance Ratio | C# Baseline (Win) | C# Baseline (Linux) | C++ vs C# (Windows) | C++ vs C# (Linux)   |
| :---------------- | :----------- | :--------- | :---------------- | :---------------- | :------------------ | :------------------ | :------------------ |
| **bcl (400)**     | 1.21 ns      | 2.72 ns    | **2.25x slower**  | 1.135 ns          | 1.214 ns            | ❌ **1.07x slower** | ❌ **2.24x slower** |
| **bcl (H346)**    | 2.63 ns      | 2.52 ns    | **1.04x faster**  | 4.551 ns          | 4.560 ns            | ✅ **1.73x faster** | ✅ **1.81x faster** |
| **bclOrd (400)**  | 1.83 ns      | 0.931 ns   | **1.96x faster**  | 1.514 ns          | 1.368 ns            | ❌ **1.21x slower** | ✅ **1.47x faster** |
| **bclOrd (H346)** | 3.13 ns      | 2.02 ns    | **1.55x faster**  | 3.127 ns          | 2.340 ns            | ❌ **1.00x equal**  | ✅ **1.16x faster** |
| **Larson (400)**  | 1.81 ns      | 1.60 ns    | **1.13x faster**  | 1.219 ns          | 1.244 ns            | ❌ **1.48x slower** | ❌ **1.29x slower** |
| **Larson (H346)** | 4.85 ns      | 3.43 ns    | **1.41x faster**  | 3.349 ns          | 3.494 ns            | ❌ **1.45x slower** | ✅ **1.02x faster** |
| **crc32 (400)**   | 1.46 ns      | 1.38 ns    | **1.06x faster**  | 1.215 ns          | 1.235 ns            | ❌ **1.20x slower** | ❌ **1.12x slower** |
| **crc32 (H346)**  | 4.56 ns      | 4.22 ns    | **1.08x faster**  | 3.259 ns          | 3.441 ns            | ❌ **1.40x slower** | ❌ **1.23x slower** |
| **fnv (400)**     | 1.48 ns      | 1.17 ns    | **1.26x faster**  | 1.205 ns          | 1.291 ns            | ❌ **1.23x slower** | ✅ **1.10x faster** |
| **fnv (H346)**    | 3.36 ns      | 2.61 ns    | **1.29x faster**  | 3.337 ns          | 3.741 ns            | ❌ **1.01x equal**  | ✅ **1.43x faster** |

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

_Last updated: July 29, 2025_
