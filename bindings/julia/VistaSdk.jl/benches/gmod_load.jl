using VistaSdk
using BenchmarkTools

suite = BenchmarkGroup()

v = vis()

gmod(v, V3_4a)
gmod(v, V3_7a)

suite["GmodCacheAccess_v3_4a"] = @benchmarkable gmod($v, V3_4a)
suite["GmodCacheAccess_v3_7a"] = @benchmarkable gmod($v, V3_7a)

results = run(suite; verbose = true)

println("\nResults (ns/op):")
for name in sort(collect(keys(results)))
    println("  ", name, ": ", round(median(results[name]).time; digits = 2))
end
