using VistaSdk
using BenchmarkTools

suite = BenchmarkGroup()

const TSD_JSON = read(
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
        "TimeSeriesData.json",
    ),
    String,
)

package = tsd_from_json(TSD_JSON)

suite["Serialize"] = @benchmarkable tsd_to_json($package, false)
suite["Deserialize"] = @benchmarkable tsd_from_json(TSD_JSON)

results = run(suite; verbose = true)

println("\nResults (ns/op):")
for name in sort(collect(keys(results)))
    println("  ", name, ": ", round(median(results[name]).time; digits = 2))
end
