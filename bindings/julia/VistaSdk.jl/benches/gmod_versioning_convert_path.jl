using VistaSdk
using BenchmarkTools

suite = BenchmarkGroup()

v = vis()
g = gmod(v, V3_4a)
locs = locations(v, V3_4a)

path_no_loc = from_short_path("411.1/C101.72/I101", g, locs)
path_with_loc = from_short_path("612.21-1/C701.13/S93", g, locs)

suite["ConvertPath"] = @benchmarkable convert_path($v, V3_4a, $path_no_loc, V3_5a)
suite["ConvertPathWithLocation"] =
    @benchmarkable convert_path($v, V3_4a, $path_with_loc, V3_5a)

results = run(suite; verbose = true)

println("\nResults (ns/op):")
for name in sort(collect(keys(results)))
    println("  ", name, ": ", round(median(results[name]).time; digits = 2))
end
