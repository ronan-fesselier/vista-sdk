``` ini

BenchmarkDotNet=v0.13.1, OS=Windows 10.0.19044.2728 (21H2)
11th Gen Intel Core i9-11950H 2.60GHz, 1 CPU, 16 logical and 8 physical cores
.NET SDK=6.0.406
  [Host]     : .NET 6.0.14 (6.0.1423.7309), X64 RyuJIT
  DefaultJob : .NET 6.0.14 (6.0.1423.7309), X64 RyuJIT


```
|                         Method |    Categories |     Mean |     Error |    StdDev |   Median |  Gen 0 |  Gen 1 | Allocated |
|------------------------------- |-------------- |---------:|----------:|----------:|---------:|-------:|-------:|----------:|
|               TryParseFullPath |   No location | 1.253 μs | 0.0353 μs | 0.0972 μs | 1.235 μs | 0.1431 |      - |      2 KB |
|                       TryParse |   No location | 4.563 μs | 0.1376 μs | 0.3813 μs | 4.528 μs | 0.3052 |      - |      4 KB |
|                                |               |          |           |           |          |        |        |           |
| TryParseFullPathIndividualized | With location | 1.364 μs | 0.0426 μs | 0.1159 μs | 1.330 μs | 0.1545 |      - |      2 KB |
|         TryParseIndividualized | With location | 1.796 μs | 0.0393 μs | 0.1076 μs | 1.757 μs | 0.3433 | 0.0019 |      4 KB |
