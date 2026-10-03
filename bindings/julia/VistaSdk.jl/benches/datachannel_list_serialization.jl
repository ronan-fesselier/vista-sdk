using VistaSdk
using BenchmarkTools

suite = BenchmarkGroup()

const DCL_JSON = read(
    joinpath(
        @__DIR__,
        "..",
        "..",
        "..",
        "..",
        "cpp",
        "tests",
        "transport",
        "_files",
        "DataChannelList.json",
    ),
    String,
)

package = dcl_from_json(DCL_JSON)

suite["Serialize"] = @benchmarkable dcl_to_json($package; pretty = false)
suite["Deserialize"] = @benchmarkable dcl_from_json(DCL_JSON)

results = run(suite; verbose = true)

println("\nResults (ns/op):")
for name in sort(collect(keys(results)))
    println("  ", name, ": ", round(median(results[name]).time; digits = 2))
end
