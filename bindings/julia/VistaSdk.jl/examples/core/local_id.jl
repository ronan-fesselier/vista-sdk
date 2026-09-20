using VistaSdk

println("=== VistaSdk.jl LocalId Sample ===\n")

let
    println("1. LocalIdBuilder: Building a simple LocalId")
    println("-----------------------------------------------")

    v = vis()
    g = gmod(v, latest(v))
    locs = locations(v, latest(v))
    cbs = codebooks(v, latest(v))

    primary = from_short_path("411.1/C101.31-2", g, locs)
    qty = create_tag(cbs[VistaSdk.Quantity], "temperature")

    if primary !== nothing && qty !== nothing
        id = build(
            with_metadata_tag(
                with_primary_item(create(LocalIdBuilder, latest(v)), primary),
                qty,
            ),
        )
        println("Built LocalId : $id")
        println("  VIS version : $(version(id))")
        println("  Primary item: $(primary_item(id))")
        println("  Quantity    : $(value(quantity(id)))")
    end
    println()
end

let
    println("2. LocalIdBuilder: Multiple metadata tags")
    println("-------------------------------------------")

    v = vis()
    g = gmod(v, latest(v))
    locs = locations(v, latest(v))
    cbs = codebooks(v, latest(v))

    primary = from_short_path("621.21/S90", g, locs)
    qty = create_tag(cbs[VistaSdk.Quantity], "mass")
    cnt = create_tag(cbs[VistaSdk.Content], "fuel.oil")
    pos = create_tag(cbs[VistaSdk.Position], "inlet")

    if primary !== nothing && qty !== nothing && cnt !== nothing && pos !== nothing
        id = build(
            with_metadata_tag(
                with_metadata_tag(
                    with_metadata_tag(
                        with_primary_item(create(LocalIdBuilder, latest(v)), primary),
                        qty,
                    ),
                    cnt,
                ),
                pos,
            ),
        )
        println("Built LocalId: $id")
        println("  Metadata tags:")
        println("    Quantity: $(value(quantity(id)))")
        println("    Content : $(value(content(id)))")
        println("    Position: $(value(VistaSdk.position(id)))")
    end
    println()
end

let
    println("3. LocalIdBuilder: With secondary item")
    println("----------------------------------------")

    v = vis()
    g = gmod(v, latest(v))
    locs = locations(v, latest(v))
    cbs = codebooks(v, latest(v))

    primary = from_short_path("621.21/S90", g, locs)
    secondary = from_short_path("411.1/C101", g, locs)
    qty = create_tag(cbs[VistaSdk.Quantity], "mass")
    cnt = create_tag(cbs[VistaSdk.Content], "fuel.oil")
    pos = create_tag(cbs[VistaSdk.Position], "inlet")

    if primary !== nothing &&
       secondary !== nothing &&
       qty !== nothing &&
       cnt !== nothing &&
       pos !== nothing
        id = build(
            with_metadata_tag(
                with_metadata_tag(
                    with_metadata_tag(
                        with_secondary_item(
                            with_primary_item(create(LocalIdBuilder, latest(v)), primary),
                            secondary,
                        ),
                        qty,
                    ),
                    cnt,
                ),
                pos,
            ),
        )
        println("Built LocalId    : $id")
        println("  Primary item   : $(primary_item(id))")
        println("  Secondary item : $(secondary_item(id))")
        println("  Has secondary  : $(secondary_item(id) !== nothing)")
    end
    println()
end

let
    println("4. LocalId: Parsing from string")
    println("---------------------------------")

    s = "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature"
    id = from_string(LocalId, s)

    if id !== nothing
        println("Parsed successfully: $id")
        println("  VIS version : $(version(id))")
        println("  Primary item: $(primary_item(id))")
        println("  Quantity    : $(value(quantity(id)))")
    else
        println("Parse failed")
    end
    println()
end

let
    println("5. LocalId: Parsing with error handling")
    println("-----------------------------------------")

    for s in [
        "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature",
        "/dnv-v2/vis-3-4a/621.21/S90/sec/411.1/C101/meta/qty-mass/cnt-fuel.oil/pos-inlet",
        "/dnv-v2/vis-3-7a/612.21/C701.23/C633/meta/calc~accumulate",
        "",
        "/dnv-v2/INVALID/411.1/meta/qty-temperature",
        "/dnv-v2/vis-3-4a/INVALID/meta/qty-temperature",
        "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-invalid_quantity",
        "//vis-3-4a/not-a-valid-path/meta/qty-temperature",
    ]
        id, errors = from_string_with_errors(LocalId, s)
        label = isempty(s) ? "(empty)" : s
        println("Parsing: \"$label\"")
        if id !== nothing
            println("  Success: $id")
        else
            println("  Failed:")
            if has_errors(errors)
                for e in errors
                    println("  [$(e.type)] $(e.message)")
                end
            end
        end
        println()
    end
end

let
    println("6. LocalIdBuilder: Modifying an existing LocalId")
    println("--------------------------------------------------")

    v = vis()
    cbs = codebooks(v, latest(v))

    original = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature")
    if original !== nothing
        println("Original: $original")

        cnt = create_tag(cbs[VistaSdk.Content], "water")
        if cnt !== nothing
            modified = build(with_metadata_tag(builder(original), cnt))
            println("Modified (added content)          : $modified")
        end

        pos = create_tag(cbs[VistaSdk.Position], "outlet")
        if pos !== nothing
            modified2 = build(
                with_metadata_tag(
                    without_metadata_tag(builder(original), VistaSdk.Quantity),
                    pos,
                ),
            )
            println("Modified (removed qty, added pos) : $modified2")
        end
    end
    println()
end

let
    println("7. LocalIdBuilder: Verbose mode")
    println("---------------------------------")

    v = vis()
    g = gmod(v, latest(v))
    locs = locations(v, latest(v))
    cbs = codebooks(v, latest(v))

    primary = from_short_path("411.1/C101.31-2", g, locs)
    qty = create_tag(cbs[VistaSdk.Quantity], "temperature")

    if primary !== nothing && qty !== nothing
        b = with_metadata_tag(
            with_primary_item(create(LocalIdBuilder, latest(v)), primary),
            qty,
        )
        normal = build(b)
        verbose = build(with_verbose_mode(b, true))
        println("Normal mode : $normal")
        println("Verbose mode: $verbose")
        println("  Includes human-readable common names for better understanding")
    end
    println()
end

let
    println("8. LocalId: Accessing components")
    println("----------------------------------")

    id = from_string(
        LocalId,
        "/dnv-v2/vis-3-4a/621.21/S90/sec/411.1/C101/meta/qty-mass/cnt-fuel.oil/pos-inlet",
    )
    if id !== nothing
        println("LocalId: $id\n")
        println("Components:")
        println("  VIS version    : $(version(id))")
        println("  Primary item   : $(primary_item(id))")
        sec = secondary_item(id)
        sec !== nothing && println("  Secondary item : $sec")
        println("\nMetadata tags:")
        for (label, tag) in [
            ("Quantity   ", quantity(id)),
            ("Content    ", content(id)),
            ("Calculation", calculation(id)),
            ("State      ", state(id)),
            ("Command    ", command(id)),
            ("Type       ", tag_type(id)),
            ("Position   ", VistaSdk.position(id)),
            ("Detail     ", detail(id)),
        ]
            tag !== nothing && println("  $label: $(value(tag))")
        end
        tags = metadata_tags(id)
        println("\nAll metadata tags ($(length(tags))):")
        for t in tags
            prefix = codebook_name_to_prefix(name(t))
            custom = is_custom(t) ? " (custom)" : ""
            println("  $prefix-$(value(t))$custom")
        end
    end
    println()
end

let
    println("9. LocalId: Equality")
    println("----------------------")

    id1 = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature")
    id2 = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature")
    id3 = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-pressure")

    if id1 !== nothing && id2 !== nothing && id3 !== nothing
        println("id1: $id1")
        println("id2: $id2")
        println("id3: $id3\n")
        println("id1 == id2 ? $(id1 == id2)")
        println("id1 == id3 ? $(id1 == id3)")
        println("id1 != id3 ? $(id1 != id3)")
    end
    println()
end

let
    println("10. LocalIdBuilder: Custom metadata tags")
    println("------------------------------------------")

    v = vis()
    g = gmod(v, latest(v))
    locs = locations(v, latest(v))
    cbs = codebooks(v, latest(v))

    primary = from_short_path("411.1/C101.31-2", g, locs)
    custom_qty = create_tag(cbs[VistaSdk.Quantity], "my_custom_measurement")

    if primary !== nothing && custom_qty !== nothing
        id = build(
            with_metadata_tag(
                with_primary_item(create(LocalIdBuilder, latest(v)), primary),
                custom_qty,
            ),
        )
        println("LocalId with custom tag: $id")
        println("  has_custom_tag : $(has_custom_tag(id))")
        println("  quantity value : $(value(quantity(id)))")
        println("  is_custom      : $(is_custom(quantity(id)))")
    end
    println()
end

let
    println("11. LocalIdBuilder: Validation")
    println("--------------------------------")

    v = vis()
    g = gmod(v, latest(v))
    locs = locations(v, latest(v))
    cbs = codebooks(v, latest(v))

    primary = from_short_path("411.1/C101.31-2", g, locs)
    qty = create_tag(cbs[VistaSdk.Quantity], "temperature")

    if primary !== nothing && qty !== nothing
        valid_b = with_metadata_tag(
            with_primary_item(create(LocalIdBuilder, latest(v)), primary),
            qty,
        )
        println("Valid builder:")
        println("  is_valid: $(is_valid(valid_b))")
        println("  is_empty: $(is_empty(valid_b))")

        empty_b = create(LocalIdBuilder, latest(v))
        println("\nEmpty builder:")
        println("  is_valid: $(is_valid(empty_b))")
        println("  is_empty: $(is_empty(empty_b))")

        incomplete_b = with_primary_item(create(LocalIdBuilder, latest(v)), primary)
        println("\nBuilder with primary item, no metadata:")
        println("  is_valid: $(is_valid(incomplete_b))")
        println("  is_empty: $(is_empty(incomplete_b))")

        println("\nAttempting to build invalid LocalId:")
        try
            build(incomplete_b)
            println("  ERROR: build succeeded unexpectedly")
        catch e
            println("  Correctly rejected: $e")
        end
    end
    println()
end

let
    println("12. MqttLocalId: MQTT-compatible formatting")
    println("---------------------------------------------")

    for (label, s) in [
        ("qty only", "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature"),
        (
            "qty + cnt + pos",
            "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
        ),
        (
            "with secondary item",
            "/dnv-v2/vis-3-4a/621.21/S90/sec/411.1/C101/meta/qty-mass/cnt-fuel.oil/pos-inlet",
        ),
    ]
        id = from_string(LocalId, s)
        id === nothing && continue
        mqtt = create(MqttLocalId, builder(id))
        mqtt === nothing && continue
        println("$label:")
        println("  Standard : $id")
        println("  MQTT     : $mqtt\n")
    end

    v = vis()
    g = gmod(v, latest(v))
    locs = locations(v, latest(v))
    cbs = codebooks(v, latest(v))

    primary = from_short_path("411.1/C101.31-2", g, locs)
    secondary = from_short_path("411.1/C101.31-5", g, locs)
    qty = create_tag(cbs[VistaSdk.Quantity], "temperature")
    cnt = create_tag(cbs[VistaSdk.Content], "exhaust.gas")
    calc = create_tag(cbs[VistaSdk.Calculation], "average")
    st = create_tag(cbs[VistaSdk.State], "high")
    cmd = create_tag(cbs[VistaSdk.Command], "start")
    typ = create_tag(cbs[VistaSdk.Type], "instantaneous")
    pos = create_tag(cbs[VistaSdk.Position], "inlet")
    det = create_tag(cbs[VistaSdk.Detail], "my_sensor_42")

    if all(!isnothing, [primary, secondary, qty, cnt, calc, st, cmd, typ, pos, det])
        b = with_metadata_tag(
            with_metadata_tag(
                with_metadata_tag(
                    with_metadata_tag(
                        with_metadata_tag(
                            with_metadata_tag(
                                with_metadata_tag(
                                    with_metadata_tag(
                                        with_secondary_item(
                                            with_primary_item(
                                                create(LocalIdBuilder, latest(v)),
                                                primary,
                                            ),
                                            secondary,
                                        ),
                                        qty,
                                    ),
                                    cnt,
                                ),
                                calc,
                            ),
                            st,
                        ),
                        cmd,
                    ),
                    typ,
                ),
                pos,
            ),
            det,
        )
        id = build(b)
        mqtt = create(MqttLocalId, b)
        if mqtt !== nothing
            println("all 8 slots + secondary + free-form detail:")
            println("  Standard : $id")
            println("  MQTT     : $mqtt")
            println("  Slots    : qty/cnt/calc/state/cmd/type/pos/detail")
            d = detail(id)
            d !== nothing &&
                println("  Detail   : '$(value(d))' (is_custom: $(is_custom(d)))\n")
        end
    end

    primary2 = from_short_path("411.1/C101.31-2", g, locs)
    secondary2 = from_short_path("411.1/C101.31-5", g, locs)
    qty2 = create_tag(cbs[VistaSdk.Quantity], "temperature")
    cnt2 = create_tag(cbs[VistaSdk.Content], "exhaust.gas")

    if primary2 !== nothing &&
       secondary2 !== nothing &&
       qty2 !== nothing &&
       cnt2 !== nothing
        b2 = with_metadata_tag(
            with_metadata_tag(
                with_secondary_item(
                    with_primary_item(create(LocalIdBuilder, V3_4a), primary2),
                    secondary2,
                ),
                qty2,
            ),
            cnt2,
        )
        mqtt2 = create(MqttLocalId, b2)
        if mqtt2 !== nothing
            println("reading MqttLocalId components:")
            println("  version        : $(version(mqtt2))")
            println("  primary_item   : $(primary_item(mqtt2))")
            println("  secondary_item : $(secondary_item(mqtt2))")
            println("  quantity       : $(value(quantity(mqtt2)))")
            println("  content        : $(value(content(mqtt2)))")
            calc2 = calculation(mqtt2)
            println("  calculation    : $(calc2 !== nothing ? value(calc2) : "(none)")\n")
        end
    end

    primary3 = from_short_path("411.1/C101.63/S206", g, locs)
    qty3 = create_tag(cbs[VistaSdk.Quantity], "temperature")

    if primary3 !== nothing && qty3 !== nothing
        b3 = with_verbose_mode(
            with_metadata_tag(
                with_primary_item(create(LocalIdBuilder, V3_4a), primary3),
                qty3,
            ),
            true,
        )
        mqtt3 = create(MqttLocalId, b3)
        if mqtt3 !== nothing
            println("builder-level state (accessible via builder()):")
            println(
                "  is_verbose_mode and has_custom_tag are not duplicated on MqttLocalId.",
            )
            println("  verbose mode has no effect on the MQTT format.")
            println("  builder().is_verbose_mode() : $(is_verbose_mode(builder(mqtt3)))")
            println("  builder().has_custom_tag()  : $(has_custom_tag(builder(mqtt3)))")
            println("  MQTT (no '~' despite verbose=true): $mqtt3\n")
        end
    end

    primary4 = from_short_path("411.1/C101.31-2", g, locs)
    qty4 = create_tag(cbs[VistaSdk.Quantity], "temperature")
    cnt4 = create_tag(cbs[VistaSdk.Content], "exhaust.gas")

    if primary4 !== nothing && qty4 !== nothing && cnt4 !== nothing
        b4 = with_metadata_tag(
            with_primary_item(create(LocalIdBuilder, V3_4a), primary4),
            qty4,
        )
        a = create(MqttLocalId, b4)
        b4b = create(MqttLocalId, b4)
        b4_with_cnt = with_metadata_tag(b4, cnt4)
        c = create(MqttLocalId, b4_with_cnt)
        if a !== nothing && b4b !== nothing && c !== nothing
            println("equality:")
            println("  a == b (same builder) : $(a == b4b)")
            println("  a == c (extra tag)    : $(a == c)\n")
        end
    end

    println("MQTT format differences vs standard:")
    println("  - No leading '/'")
    println("  - Underscores instead of slashes in paths")
    println("  - No 'meta/' section")
    println("  - '_' placeholder for absent metadata slots")
    println("  - 8 fixed slots: qty/cnt/calc/state/cmd/type/pos/detail")
end
