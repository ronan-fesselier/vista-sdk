using VistaSdk

println("=== VistaSdk.jl GmodPath Sample ===\n")

v = vis()

println("1. GmodPath: parsing short paths")
println("-----------------------------------")

let g = gmod(v, latest(v)), locs = locations(v, latest(v))
    p1 = from_short_path("411.1", g, locs)
    if p1 !== nothing
        println("Parsed: \"411.1\"")
        println("  Node     : $(code(node(p1)))")
        println("  Full path: $(to_full_path_string(p1))")
    end

    p2 = from_short_path("411.1-1P", g, locs)
    if p2 !== nothing
        println("\nParsed: \"411.1-1P\"")
        println("  Node     : $(code(node(p2)))")
        loc = location(node(p2))
        loc !== nothing && println("  Location : $loc")
        println("  Full path: $(to_full_path_string(p2))")
    end

    p3 = from_short_path("612.21-1/C701.13/S93", g, locs)
    if p3 !== nothing
        println("\nParsed: \"612.21-1/C701.13/S93\"")
        println("  Final node : $(code(node(p3)))")
        println("  Path length: $(length(p3)) nodes")
        println("  Full path  : $(to_full_path_string(p3))")
    end

    p4, errors4 = from_short_path_with_errors("C101.63-2P/S206.22", g, locs)
    if p4 !== nothing
        println("\nParsed: \"C101.63-2P/S206.22\"")
        println("  Final node : $(code(node(p4)))")
        println("  Path length: $(length(p4)) nodes")
        println("  Full path  : $(to_full_path_string(p4))")
    elseif has_errors(errors4)
        println("\nFailed to parse \"C101.63-2P/S206.22\":")
        for e in errors4
            println("  - $(e.message)")
        end
    end
    println()
end

println("2. GmodPath: parsing full paths")
println("----------------------------------")

let g = gmod(v, latest(v)), locs = locations(v, latest(v))
    p1 = from_full_path("VE/400a/410/411/411i/411.1-1P", g, locs)
    if p1 !== nothing
        println("Parsed full path: \"VE/400a/410/411/411i/411.1-1P\"")
        println("  Final node : $(code(node(p1)))")
        println("  Path length: $(length(p1)) nodes")
        loc = location(node(p1))
        loc !== nothing && println("  Location   : $loc")
    end

    full2 = "VE/600a/610/612/612.2/612.2i-1/612.21-1/CS10/C701/C701.1/C701.13/S93"
    p2 = from_full_path(full2, g, locs)
    if p2 !== nothing
        println("\nParsed: \"$full2\"")
        println("  Final node : $(code(node(p2)))")
        println("  Path length: $(length(p2)) nodes")
        println("  Full path  : $(to_full_path_string(p2))")
    end
    println()
end

println("3. GmodPath: invalid paths with error handling")
println("-------------------------------------------------")

let g = gmod(v, latest(v)), locs = locations(v, latest(v))
    invalid_paths = ["", "INVALID", "411.1-XYZ", "VE/INVALID/411.1"]
    for s in invalid_paths
        p, errors = from_short_path_with_errors(s, g, locs)
        println("Path: \"$s\"")
        if p !== nothing
            println("  Valid (unexpected!)")
        else
            println("  Invalid - Errors:")
            for e in errors
                println("    - $(e.message)")
            end
        end
        println()
    end
end

println("4. GmodPath: navigating path nodes")
println("-------------------------------------")

let g = gmod(v, latest(v)), locs = locations(v, latest(v))
    full = "VE/600a/610/612/612.2/612.2i-1/612.21-1/CS10/C701/C701.1/C701.13/S93"
    p = from_full_path(full, g, locs)
    if p !== nothing
        println("Path: $(to_full_path_string(p))")
        println("\nAll nodes in path:")
        ps = parents(p)
        for (i, n) in enumerate(ps)
            loc = location(n)
            node_str = loc !== nothing ? "$(code(n))-$loc" : code(n)
            println("  [$(lpad(i-1, 2))] $(rpad(node_str, 12)) ($(name(metadata(n))))")
        end
        n = node(p)
        loc = location(n)
        node_str = loc !== nothing ? "$(code(n))-$loc" : code(n)
        println("  [$(lpad(length(ps), 2))] $(rpad(node_str, 12)) ($(name(metadata(n))))")

        println("\nFinal node:")
        println("  Code    : $(code(node(p)))")
        println("  Name    : $(name(metadata(node(p))))")
        println("  Category: $(category(metadata(node(p))))")
    end
    println()
end

println("5. GmodPath: individualization")
println("--------------------------------")

let g = gmod(v, latest(v)), locs = locations(v, latest(v))
    p = from_short_path("411.1/C101.62/S205", g, locs)
    if p !== nothing
        println("Path: $p")
        println("Is individualizable: $(is_individualizable(p))")

        n_sets = individualizable_set_count(p)
        println("Individualizable sets: $n_sets")

        for i = 1:n_sets
            s = individualizable_set_at(p, i)
            s === nothing && continue
            println("\nSet $i:")
            println("  Nodes  : $(sprint(show, s))")
            print("  Indices: ")
            for j = 1:index_count(s)
                idx = index_at(s, j)
                idx !== nothing && print("$idx ")
            end
            println()
        end

        ind = from_short_path("411.1-1/C101.62/S205", g, locs)
        if ind !== nothing
            println("\nIndividualized: $ind")
            println("Full path     : $(to_full_path_string(ind))")
        end
    end
    println()
end

println("6. GmodPath: conversion between formats")
println("------------------------------------------")

let g = gmod(v, latest(v)), locs = locations(v, latest(v))
    p1 = from_short_path("411.1-1P", g, locs)
    if p1 !== nothing
        println("Short path: $p1")
        println("Full path : $(to_full_path_string(p1))")
    end

    p2 = from_full_path("VE/400a/410/411/411i/411.1-1P", g, locs)
    if p2 !== nothing
        println("\nFull path : $(to_full_path_string(p2))")
        println("Short path: $p2")
    end
    println()
end

println("7. GmodPath: comparison and equality")
println("---------------------------------------")

let g = gmod(v, latest(v)), locs = locations(v, latest(v))
    p1 = from_short_path("411.1-1P", g, locs)
    p2 = from_full_path("VE/400a/410/411/411i/411.1-1P", g, locs)
    p3 = from_short_path("411.1-2P", g, locs)

    if p1 !== nothing && p2 !== nothing && p3 !== nothing
        println("Path 1: $p1")
        println("Path 2: $p2")
        println("Path 3: $p3\n")
        println("Path 1 == Path 2? $(p1 == p2)")
        println("Path 1 == Path 3? $(p1 == p3)")
        println("Path 1 != Path 3? $(p1 != p3)")
    end
    println()
end

println("8. GmodPath: working with product types/selections")
println("-----------------------------------------------------")

let g = gmod(v, latest(v)), locs = locations(v, latest(v))
    p1 = from_short_path("411.1-1P", g, locs)
    if p1 !== nothing
        n = node(p1)
        println("Path                 : $p1")
        println("Node code            : $(code(n))")
        println("Is product selection : $(is_product_selection(n))")
        ps = product_selection(n)
        ps !== nothing && println("Has product selection: $(code(ps))")
    end

    p2 = from_short_path("612.21-1/C701.13/S93", g, locs)
    if p2 !== nothing
        n = node(p2)
        println("\nPath      : $p2")
        println("Node code : $(code(n)) ($(name(metadata(n))))")
        println("Category  : $(category(metadata(n)))")
        println("Is product: $(category(metadata(n)) == "PRODUCT")")
    end
    println()
end
