using VistaSdk
using BenchmarkTools

suite = BenchmarkGroup()

v = vis()
cbs = codebooks(v, V3_11a)

quantity = cbs[VistaSdk.Quantity]
state = cbs[VistaSdk.State]
position = cbs[VistaSdk.Position]

suite["Quantity_hasStandardValue_hit"] =
    @benchmarkable has_standard_value($quantity, "temperature")
suite["Quantity_hasStandardValue_miss"] =
    @benchmarkable has_standard_value($quantity, "not_a_real_quantity")
suite["Quantity_createTag_standard"] = @benchmarkable create_tag($quantity, "temperature")
suite["Quantity_createTag_custom"] =
    @benchmarkable create_tag($quantity, "custom_measurement")
suite["State_hasGroup_hit"] = @benchmarkable has_group($state, "Running")
suite["State_hasGroup_miss"] = @benchmarkable has_group($state, "NotARealGroup")
suite["Position_validatePosition_simple"] =
    @benchmarkable validate_position($position, "centre")
suite["Position_validatePosition_composite"] =
    @benchmarkable validate_position($position, "centre-starboard-2")

results = run(suite; verbose = true)

println("\nResults (ns/op):")
for name in sort(collect(keys(results)))
    println("  ", name, ": ", round(median(results[name]).time; digits = 2))
end
