using VistaSdk

println("=== VistaSdk.jl MetadataTagsQuery Sample ===\n")

let
    println("1. Empty query matches any LocalId")
    println("------------------------------------")

    q = build(create(MetadataTagsQueryBuilder))

    lids = [
        "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature",
        "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas",
        "/dnv-v2/vis-3-4a/652.31/S90.3/S61/sec/652.1i-1P/meta/cnt-sea.water/state-opened",
    ]
    for s in lids
        lid = from_string(LocalId, s)
        if lid !== nothing
            println("  $(is_match(q, lid) ? "match" : "no match"): $s")
        end
    end
    println()
end

let
    println("2. Single tag filter (subset mode: allow other tags)")
    println("------------------------------------------------------")

    q = build(
        with_allow_other_tags(
            with_tag(create(MetadataTagsQueryBuilder), VistaSdk.Content, "sea.water"),
            true,
        ),
    )

    cases = [
        (
            "/dnv-v2/vis-3-4a/652.31/S90.3/S61/sec/652.1i-1P/meta/cnt-sea.water/state-opened",
            true,
        ),
        ("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas", false),
        ("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature", false),
    ]
    for (s, expected) in cases
        lid = from_string(LocalId, s)
        if lid !== nothing
            result = is_match(q, lid)
            status = result == expected ? "OK" : "FAILED"
            println("  [$status] $(result ? "match" : "no match"): $s")
        end
    end
    println()
end

let
    println("3. Single tag filter (exact mode: no other tags allowed)")
    println("----------------------------------------------------------")

    q = build(
        with_allow_other_tags(
            with_tag(create(MetadataTagsQueryBuilder), VistaSdk.Content, "exhaust.gas"),
            false,
        ),
    )

    cases = [
        ("/dnv-v2/vis-3-4a/411.1/meta/cnt-exhaust.gas", true),
        ("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas", false),
    ]
    for (s, expected) in cases
        lid = from_string(LocalId, s)
        if lid !== nothing
            result = is_match(q, lid)
            status = result == expected ? "OK" : "FAILED"
            println("  [$status] $(result ? "match" : "no match"): $s")
        end
    end
    println()
end

let
    println("4. Multiple tags (exact mode)")
    println("------------------------------")

    q = build(
        with_allow_other_tags(
            with_tag(
                with_tag(
                    create(MetadataTagsQueryBuilder),
                    VistaSdk.Quantity,
                    "temperature",
                ),
                VistaSdk.Content,
                "exhaust.gas",
            ),
            false,
        ),
    )

    cases = [
        ("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas", true),
        (
            "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
            false,
        ),
    ]
    for (s, expected) in cases
        lid = from_string(LocalId, s)
        if lid !== nothing
            result = is_match(q, lid)
            status = result == expected ? "OK" : "FAILED"
            println("  [$status] $(result ? "match" : "no match"): $s")
        end
    end
    println()
end

let
    println("5. Builder from LocalId (pre-populate all tags)")
    println("------------------------------------------------")

    source =
        from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas")
    if source !== nothing
        println("  Source: $(string(source))")

        q_subset = build(from_local_id(MetadataTagsQueryBuilder, source, true))
        q_exact = build(from_local_id(MetadataTagsQueryBuilder, source, false))

        with_extra = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
        )
        if with_extra !== nothing
            println("  With extra tag: $(string(with_extra))")
            println(
                "    subset mode: $(is_match(q_subset, with_extra) ? "match" : "no match")",
            )
            println(
                "    exact  mode: $(is_match(q_exact, with_extra) ? "match" : "no match")",
            )
        end
    end
    println()
end

let
    println("6. Builder accessor on built query")
    println("------------------------------------")

    q = build(with_tag(create(MetadataTagsQueryBuilder), VistaSdk.Content, "sea.water"))
    b = builder(q)
    println(
        "  builder() returned a MetadataTagsQueryBuilderRef: $(b isa MetadataTagsQueryBuilderRef)",
    )
    println()
end
