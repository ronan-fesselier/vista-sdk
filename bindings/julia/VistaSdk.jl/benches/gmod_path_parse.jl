using VistaSdk
using BenchmarkTools

suite = BenchmarkGroup()

const SHORT_PATHS = [
    "411.1/C101.72/I101",
    "511/C101.63/S90",
    "411.1/C101.63/S90",
    "621.21/C101.72/I101",
    "511/C101.72/I101",
    "411.1/C101.31/I101",
]

const FULL_PATHS = [
    "VE/400a/410/411/411i/411.1/CS1/C101/C101.7/C101.72/I101",
    "VE/500a/510/511/CS1/C101/C101.6/C101.63/S90",
    "VE/400a/410/411/411i/411.1/CS1/C101/C101.6/C101.63/S90",
    "VE/600a/620/621/621.2/621.2i/621.21/CS1/C101/C101.7/C101.72/I101",
    "VE/500a/510/511/CS1/C101/C101.7/C101.72/I101",
    "VE/400a/410/411/411i/411.1/CS1/C101/C101.3/C101.31/I101",
]

const SHORT_PATHS_INDIVIDUALIZED = [
    "612.21-1/C701.13/S93",
    "612.21-2/C701.13/S93",
    "612.21-1/C701.13/S90",
    "612.21-2/C701.13/S90",
]

const FULL_PATHS_INDIVIDUALIZED = [
    "VE/600a/610/612/612.2/612.2i/612.21-1/CS10/C701/C701.1/C701.13/S93",
    "VE/600a/610/612/612.2/612.2i/612.21-2/CS10/C701/C701.1/C701.13/S93",
    "VE/600a/610/612/612.2/612.2i/612.21-1/CS10/C701/C701.1/C701.13/S90",
    "VE/600a/610/612/612.2/612.2i/612.21-2/CS10/C701/C701.1/C701.13/S90",
]

v = vis()
g = gmod(v, V3_4a)
locs = locations(v, V3_4a)

mutable struct _Cycler
    i::Int
    items::Vector{String}
end
_Cycler(items::Vector{String}) = _Cycler(1, items)

function _next!(c::_Cycler)
    s = c.items[c.i]
    c.i = c.i % length(c.items) + 1
    s
end

short_cycler = _Cycler(SHORT_PATHS)
full_cycler = _Cycler(FULL_PATHS)
short_indiv_cycler = _Cycler(SHORT_PATHS_INDIVIDUALIZED)
full_indiv_cycler = _Cycler(FULL_PATHS_INDIVIDUALIZED)

suite["FromShortPath"] = @benchmarkable from_short_path(_next!($short_cycler), $g, $locs)
suite["FromFullPath"] = @benchmarkable from_full_path(_next!($full_cycler), $g, $locs)
suite["FromShortPathIndividualized"] =
    @benchmarkable from_short_path(_next!($short_indiv_cycler), $g, $locs)
suite["FromFullPathIndividualized"] =
    @benchmarkable from_full_path(_next!($full_indiv_cycler), $g, $locs)

results = run(suite; verbose = true)

println("\nResults (ns/op):")
for name in sort(collect(keys(results)))
    println("  ", name, ": ", round(median(results[name]).time; digits = 2))
end
