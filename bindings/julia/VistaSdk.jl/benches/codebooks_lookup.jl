using VistaSdk
using BenchmarkTools

suite = BenchmarkGroup()

v = vis()
cbs = codebooks(v, V3_11a)

entries = Dict(
    "Quantity" => 0,
    "Content" => 1,
    "Position" => 2,
    "State" => 3,
    "Command" => 4,
    "Detail" => 5,
    "Calculation" => 6,
    "Type" => 7,
)
keys_to_lookup = ["Quantity", "Position", "State", "Detail"]

suite["DictLookup"] = @benchmarkable all(k -> haskey($entries, k), $keys_to_lookup)

suite["Codebooks"] = @benchmarkable begin
    $cbs[VistaSdk.Quantity]
    $cbs[VistaSdk.Position]
    $cbs[VistaSdk.State]
    $cbs[VistaSdk.Detail]
end

results = run(suite; verbose = true)

println("\nResults (ns/op):")
for name in sort(collect(keys(results)))
    println("  ", name, ": ", round(median(results[name]).time; digits = 2))
end
