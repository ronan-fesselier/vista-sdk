using VistaSdk
using BenchmarkTools

suite = BenchmarkGroup()

v = vis()
g = gmod(v, V3_7a)

suite["FullTraversal/iterate"] = @benchmarkable begin
    count = 0
    for _ in $g
        count += 1
    end
    count
end

suite["FullTraversal/traverse"] = @benchmarkable begin
    count = 0
    traverse($g, (parents, node) -> begin
        count += 1
        TraversalContinue
    end)
    count
end

results = run(suite; verbose = true)

println("\nResults (ns/op):")
for name in sort(collect(keys(results)))
    println("  ", name, ": ", round(median(results[name]).time; digits = 2))
end
