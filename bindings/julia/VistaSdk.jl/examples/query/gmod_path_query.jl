using VistaSdk

println("=== VistaSdk.jl GmodPathQuery Sample ===\n")

let
    println("1. Path builder matches itself")
    println("--------------------------------")

    v = vis()
    g = gmod(v, V3_4a)
    l = locations(v, V3_4a)

    path = from_short_path("411.1-1/C101", g, l)
    q = build(from_path(GmodPathQueryBuilder, path))
    println("  Path: $(string(path))")
    println("  Matches itself: $(is_match(q, path))")
    println()
end

let
    println("2. Path builder: per-node location overrides")
    println("----------------------------------------------")

    v = vis()
    g = gmod(v, V3_4a)
    l = locations(v, V3_4a)

    path = from_short_path("433.1-P/C322.31-2/C173", g, l)
    println("  Base path: $(string(path))")

    q_exact = build(from_path(GmodPathQueryBuilder, path))
    q_any_loc = build(
        path_with_node_all_locations(
            path_with_node_all_locations(
                from_path(GmodPathQueryBuilder, path),
                "433.1",
                true,
            ),
            "C322.31",
            true,
        ),
    )

    cases = [
        "433.1-P/C322.31-2/C173",
        "433.1-S/C322.31-2/C173",
        "433.1-P/C322.31/C173",
        "433.1/C322.31/C173",
    ]
    println("  exact query vs any-location query:")
    for s in cases
        p = from_short_path(s, g, l)
        if p !== nothing
            println(
                "    $(rpad(s, 30))  exact=$(is_match(q_exact, p))  any_loc=$(is_match(q_any_loc, p))",
            )
        end
    end
    println()
end

let
    println("3. Nodes builder: match by node code regardless of path prefix")
    println("---------------------------------------------------------------")

    v = vis()
    g = gmod(v, V3_7a)
    l = locations(v, V3_7a)

    node = get_node(g, "C101.61")
    q = build(with_node_all_locations(create(GmodPathQueryBuilder), node, true))

    cases = [
        "411.1/C101.61-1/S203.3/S110.2/C101",
        "511.11/C101.61-1/S203.3/S110.2/C101",
        "221.11/C1141.421/C1051.7/C101.61-2/S203",
        "411.1/C101.31/S203",
    ]
    for s in cases
        p = from_short_path(s, g, l)
        if p !== nothing
            println("  $(is_match(q, p) ? "match" : "no match"): $s")
        end
    end
    println()
end

let
    println("4. with_any_node_before: ignore parent nodes")
    println("----------------------------------------------")

    v = vis()
    g = gmod(v, V3_9a)
    l = locations(v, V3_9a)

    base = from_short_path("411.1/C101.31", g, l)
    q = build(
        without_locations(
            with_any_node_before(from_path(GmodPathQueryBuilder, base), "C101"),
        ),
    )

    cases = [
        ("411.1/C101.31", true),
        ("511.11/C101.31", true),
        ("411.1/C101.31-2", true),
        ("411.1/C102.31", false),
    ]
    println("  Base: $(string(base))  (any parent before C101, no locations)")
    for (s, expected) in cases
        p = from_short_path(s, g, l)
        if p !== nothing
            result = is_match(q, p)
            status = result == expected ? "OK" : "FAILED"
            println("  [$status] $(result ? "match" : "no match"): $s")
        end
    end
    println()
end

let
    println("5. with_any_node_after: ignore suffix nodes")
    println("---------------------------------------------")

    v = vis()
    g = gmod(v, V3_4a)
    l = locations(v, V3_4a)

    base = from_short_path("411.1/C101.31", g, l)
    q = build(
        with_any_node_after(
            without_locations(from_path(GmodPathQueryBuilder, base)),
            "411.1",
        ),
    )

    cases = [
        ("411.1/C101.31-2", true),
        ("411.1/C101.63/S206", true),
        ("411.1-1/C101.61/S203", true),
        ("511.11/C101.63/S206", false),
        ("652.31/S90.3/S61", false),
    ]
    println("  Base: $(string(base))  (any suffix after 411.1, no locations)")
    for (s, expected) in cases
        p = from_short_path(s, g, l)
        if p !== nothing
            result = is_match(q, p)
            status = result == expected ? "OK" : "FAILED"
            println("  [$status] $(result ? "match" : "no match"): $s")
        end
    end
    println()
end

let
    println("6. without_locations: ignore all location individualizations")
    println("--------------------------------------------------------------")

    v = vis()
    g = gmod(v, V3_4a)
    l = locations(v, V3_4a)

    base = from_short_path("411.1-1/C101.31", g, l)
    q = build(without_locations(from_path(GmodPathQueryBuilder, base)))

    for s in ["411.1-1/C101.31", "411.1-2/C101.31", "411.1/C101.31"]
        p = from_short_path(s, g, l)
        if p !== nothing
            println("  $(is_match(q, p) ? "match" : "no match"): $s")
        end
    end
    println()
end

let
    println("7. with_node_locations: match node with specific locations only")
    println("-------------------------------------------------------------------")

    v = vis()
    g = gmod(v, V3_7a)
    l = locations(v, V3_7a)

    node = get_node(g, "C101.61")
    loc1 = Base.parse(Location, l, "1")
    loc2 = Base.parse(Location, l, "2")
    q = build(with_node_locations(create(GmodPathQueryBuilder), node, [loc1, loc2]))

    cases = [
        ("411.1/C101.61-1/S203.3/S110.2/C101", true),
        ("511.11/C101.61-2/S203.3/S110.2/C101", true),
        ("221.11/C1141.421/C1051.7/C101.61-3/S203", false),
        ("411.1/C101.61/S203.3/S110.2/C101", false),
    ]
    println("  Node C101.61 restricted to locations 1 and 2:")
    for (s, expected) in cases
        p = from_short_path(s, g, l)
        if p !== nothing
            result = is_match(q, p)
            status = result == expected ? "OK" : "FAILED"
            println("  [$status] $(result ? "match" : "no match"): $s")
        end
    end
    println()
end

let
    println("8. path-only vs nodes-only method enforcement")
    println("-----------------------------------------------")

    v = vis()
    g = gmod(v, V3_4a)
    l = locations(v, V3_4a)

    node = get_node(g, "411.1")
    nb = with_node_all_locations(create(GmodPathQueryBuilder), node, true)

    try
        path_with_node_all_locations(nb, "411.1", true)
        println("  FAILED: path_with_node_all_locations on nodes builder should throw")
    catch
        println("  OK: path_with_node_all_locations on nodes builder throws")
    end

    path = from_short_path("411.1/C101.31", g, l)
    pb = from_path(GmodPathQueryBuilder, path)

    try
        with_node_all_locations(pb, node, true)
        println("  FAILED: with_node_all_locations on path builder should throw")
    catch
        println("  OK: with_node_all_locations on path builder throws")
    end
    println()
end
