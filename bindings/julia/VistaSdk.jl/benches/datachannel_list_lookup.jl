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
list = data_channel_list(package)

ch1 = list[1]
short_id1 = short_id(channel_id(ch1))
local_id_str1 = local_id_string(channel_id(ch1))
local_id1 = from_string(LocalId, local_id_str1)

suite["ByShortId"] = @benchmarkable from_short_id($list, $short_id1)
suite["ByLocalId"] = @benchmarkable from_local_id($list, $local_id1)

results = run(suite; verbose = true)

println("\nResults (ns/op):")
for name in sort(collect(keys(results)))
    println("  ", name, ": ", round(median(results[name]).time; digits = 2))
end
