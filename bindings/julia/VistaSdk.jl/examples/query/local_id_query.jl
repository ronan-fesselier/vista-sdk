using VistaSdk

println("=== VistaSdk.jl LocalIdQuery Sample ===\n")

let
    println("1. Empty query matches any LocalId")
    println("------------------------------------")

    q = build(create(LocalIdQueryBuilder))

    ids = [
        "/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power",
        "/dnv-v2/vis-3-4a/652.31/S90.3/S61/sec/652.1i-1P/meta/cnt-sea.water/state-opened",
        "/dnv-v2/vis-3-7a/511.11/C101/meta/qty-pressure/cnt-lubricating.oil",
    ]
    for s in ids
        lid = from_string(LocalId, s)
        println("  $(is_match(q, lid) ? "match" : "no match"): $s")
    end
    println()
end

let
    println("2. Exact LocalId match via from_local_id")
    println("------------------------------------------")

    s = "/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power"
    lid = from_string(LocalId, s)
    q = build(from_local_id(LocalIdQueryBuilder, lid))

    cases = [
        "/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power",
        "/dnv-v2/vis-3-4a/411.1-1/C101.31/meta/qty-power",
        "/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-temperature",
    ]
    for c in cases
        l2 = from_string(LocalId, c)
        println("  $(is_match(q, l2) ? "match" : "no match"): $c")
    end
    println()
end

let
    println("3. Primary item query: ignore location individualizations")
    println("----------------------------------------------------------")

    v = vis()
    g = gmod(v, V3_4a)
    l = locations(v, V3_4a)

    path = from_short_path("411.1/C101.31", g, l)
    primary_q = build(without_locations(from_path(GmodPathQueryBuilder, path)))
    q = build(with_primary_item_query(create(LocalIdQueryBuilder), primary_q))

    cases = [
        ("/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power", true),
        ("/dnv-v2/vis-3-4a/411.1-1/C101.31/meta/qty-power", true),
        ("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-power", true),
        ("/dnv-v2/vis-3-4a/411.1/C102.31/meta/qty-power", false),
    ]
    println("  Primary path: $(string(path)) (without_locations)")
    for (s, expected) in cases
        lid = from_string(LocalId, s)
        result = is_match(q, lid)
        status = result == expected ? "OK" : "FAILED"
        println("  [$status] $(result ? "match" : "no match"): $s")
    end
    println()
end

let
    println("4. Secondary item query: match by path ignoring locations")
    println("----------------------------------------------------------")

    v = vis()
    g = gmod(v, V3_4a)
    l = locations(v, V3_4a)

    path = from_short_path("411.1/C101.31", g, l)
    secondary_q = build(without_locations(from_path(GmodPathQueryBuilder, path)))
    q = build(with_secondary_item_query(create(LocalIdQueryBuilder), secondary_q))

    cases = [
        "/dnv-v2/vis-3-4a/411.1/C101.63/S206/sec/411.1/C101.31-5/~propulsion.engine/~cooling.system/~for.propulsion.engine/~cylinder.5/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
        "/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power",
    ]
    for s in cases
        lid = from_string(LocalId, s)
        println("  $(is_match(q, lid) ? "match" : "no match"): $s")
    end
    println()
end

let
    println("5. Metadata tags filter: match by content tag")
    println("-----------------------------------------------")

    tags_q =
        build(with_tag(create(MetadataTagsQueryBuilder), VistaSdk.Content, "sea.water"))
    q = build(with_tags(create(LocalIdQueryBuilder), tags_q))

    cases = [
        "/dnv-v2/vis-3-4a/652.31/S90.3/S61/sec/652.1i-1P/meta/cnt-sea.water/state-opened",
        "/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power",
    ]
    for s in cases
        lid = from_string(LocalId, s)
        println("  $(is_match(q, lid) ? "match" : "no match"): $s")
    end
    println()
end

let
    println("6. without_secondary_item vs with_any_secondary_item")
    println("------------------------------------------------------")

    q_none = build(without_secondary_item(create(LocalIdQueryBuilder)))
    q_any = build(with_any_secondary_item(create(LocalIdQueryBuilder)))

    cases = [
        "/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power",
        "/dnv-v2/vis-3-4a/411.1/C101.31/sec/652.1i-1P/meta/qty-power",
    ]
    println("  $(rpad("LocalId", 70))  no_sec  any_sec")
    for s in cases
        lid = from_string(LocalId, s)
        println(
            "  $(rpad(s, 70))  $(is_match(q_none, lid) ? "match " : "no    ")  $(is_match(q_any, lid) ? "match" : "no")",
        )
    end
    println()
end

let
    println("7. is_match_str: match directly from string")
    println("---------------------------------------------")

    s = "/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power"
    q = build(from_string(LocalIdQueryBuilder, s))

    cases = [
        "/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power",
        "/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-temperature",
    ]
    for c in cases
        println("  $(is_match_str(q, c) ? "match" : "no match"): $c")
    end
    println()
end

let
    println("8. Combining primary path query, secondary constraint, and tags")
    println("----------------------------------------------------------------")

    v = vis()
    g = gmod(v, V3_4a)
    l = locations(v, V3_4a)

    path = from_short_path("652.31/S90.3/S61", g, l)
    primary_q = build(without_locations(from_path(GmodPathQueryBuilder, path)))
    tags_q =
        build(with_tag(create(MetadataTagsQueryBuilder), VistaSdk.Content, "sea.water"))

    q = build(
        with_tags(with_primary_item_query(create(LocalIdQueryBuilder), primary_q), tags_q),
    )

    cases = [
        "/dnv-v2/vis-3-4a/652.31/S90.3/S61/sec/652.1i-1P/meta/cnt-sea.water/state-opened",
        "/dnv-v2/vis-3-4a/652.31/S90.3/S61/meta/cnt-fuel.oil",
        "/dnv-v2/vis-3-4a/411.1/C101.31/meta/cnt-sea.water",
    ]
    for s in cases
        lid = from_string(LocalId, s)
        println("  $(is_match(q, lid) ? "match" : "no match"): $s")
    end
    println()
end
