using VistaSdk
using BenchmarkTools

suite = BenchmarkGroup()

const SIMPLE = "/dnv-v2/vis-3-4a/751/I101/meta/state-common.alarm"
const COMPLEX = "/dnv-v2/vis-3-4a/1036.11/S90.3/S61/sec/1036.13i-1/C662.1/C661/meta/state-auto.control/detail-blow.off"

suite["Simple"] = @benchmarkable from_string(LocalId, SIMPLE)
suite["Complex"] = @benchmarkable from_string(LocalId, COMPLEX)

results = run(suite; verbose = true)

println("\nResults (ns/op):")
for name in sort(collect(keys(results)))
    println("  ", name, ": ", round(median(results[name]).time; digits = 2))
end
