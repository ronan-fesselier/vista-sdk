using VistaSdk
using BenchmarkTools

suite = BenchmarkGroup()

v = vis()
g = gmod(v, V3_7a)

map_ = Dict{String,GmodNodeRef}()
for node in g
    map_[code(node)] = node
end

suite["DictLookup"] = @benchmarkable begin
    haskey($map_, "VE") &&
        haskey($map_, "400a") &&
        haskey($map_, "400") &&
        haskey($map_, "H346.11112")
end

_try_get_node(g, c) =
    try
        get_node(g, c)
        true
    catch
        false
    end

suite["Gmod"] = @benchmarkable begin
    _try_get_node($g, "VE") &&
        _try_get_node($g, "400a") &&
        _try_get_node($g, "400") &&
        _try_get_node($g, "H346.11112")
end

results = run(suite; verbose = true)

println("\nResults (ns/op):")
for name in sort(collect(keys(results)))
    println("  ", name, ": ", round(median(results[name]).time; digits = 2))
end
