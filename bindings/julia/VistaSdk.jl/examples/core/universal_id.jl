using VistaSdk

println("=== VistaSdk.jl UniversalId Sample ===\n")

let
    println("1. UniversalIdBuilder: Building a simple UniversalId")
    println("------------------------------------------------------")

    v = vis()
    ver = latest(v)
    g = gmod(v, ver)
    locs = locations(v, ver)
    cbs = codebooks(v, ver)

    imo = ImoNumber(1234567)
    primary = from_short_path("411.1/C101.31-2", g, locs)
    qty_tag = create_tag(cbs[VistaSdk.Quantity], "temperature")

    if primary !== nothing && qty_tag !== nothing
        lb = with_metadata_tag(
            with_primary_item(create(LocalIdBuilder, ver), primary),
            qty_tag,
        )
        uid =
            build(with_local_id(with_imo_number(create(UniversalIdBuilder, ver), imo), lb))
        println("Built UniversalId: $(string(uid))")
        println("  IMO Number: $(string(imo_number(uid)))")
        println("  LocalId   : $(string(local_id(uid)))")
    end
    println()
end

let
    println("2. UniversalIdBuilder: Building with full LocalId")
    println("---------------------------------------------------")

    v = vis()
    ver = latest(v)
    g = gmod(v, ver)
    locs = locations(v, ver)
    cbs = codebooks(v, ver)

    primary = from_short_path("621.21/S90", g, locs)
    secondary = from_short_path("411.1/C101", g, locs)
    qty_tag = create_tag(cbs[VistaSdk.Quantity], "mass")
    cnt_tag = create_tag(cbs[VistaSdk.Content], "fuel.oil")
    pos_tag = create_tag(cbs[VistaSdk.Position], "inlet")

    if primary !== nothing &&
       secondary !== nothing &&
       qty_tag !== nothing &&
       cnt_tag !== nothing &&
       pos_tag !== nothing
        lb = with_metadata_tag(
            with_metadata_tag(
                with_metadata_tag(
                    with_secondary_item(
                        with_primary_item(create(LocalIdBuilder, ver), primary),
                        secondary,
                    ),
                    qty_tag,
                ),
                cnt_tag,
            ),
            pos_tag,
        )
        imo = parse(ImoNumber, "IMO9074729")
        uid =
            build(with_local_id(with_imo_number(create(UniversalIdBuilder, ver), imo), lb))
        println("Built UniversalId: $(string(uid))")
        println("  Format       : {naming-entity}/{imo-number}{local-id}")
        println("  Naming entity: $(naming_entity(UniversalId))")
        println("  IMO Number   : $(string(imo_number(uid)))")
    end
    println()
end

let
    println("3. UniversalId: Parsing from string")
    println("-------------------------------------")

    s = "data.dnv.com/IMO1234567/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature"
    uid = from_string(UniversalIdBuilder, s)
    if uid !== nothing
        println("Parsed successfully: $(string(uid))")
        println("  IMO Number : $(string(imo_number(uid)))")
        lid = local_id(uid)
        println("  LocalId    : $(string(lid))")
        println("  VIS Version: $(string(version(lid)))")
    else
        println("Parse failed")
    end
    println()
end

let
    println("4. UniversalId: Parsing with error handling")
    println("----------------------------------------------")

    test_strings = [
        "data.dnv.com/IMO1234567/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature",
        "data.dnv.com/IMO9074729/dnv-v2/vis-3-4a/621.21/S90/sec/411.1/C101/meta/qty-mass/cnt-fuel.oil/pos-inlet",
        "data.dnv.com/IMO9785811/dnv-v2/vis-3-7a/612.21/C701.23/C633/meta/calc~accumulate",
        "",
        "data.dnv.com/INVALID/dnv-v2/vis-3-4a/411.1/meta/qty-temperature",
        "wrong.entity/IMO1234567/dnv-v2/vis-3-4a/411.1/meta/qty-temperature",
        "data.dnv.com/IMO1234567/dnv-v2/vis-3-4a/INVALID/meta/qty-temperature",
    ]

    for s in test_strings
        uid, errors = from_string_with_errors(UniversalIdBuilder, s)
        display = isempty(s) ? "(empty)" : length(s) > 60 ? s[1:60] * "..." : s
        println("Parsing: \"$display\"")
        if uid !== nothing
            println("  Success")
            println("    IMO: $(string(imo_number(uid)))")
        else
            println("  Failed:")
            if has_errors(errors)
                for e in errors
                    println("    - $(e)")
                end
            end
        end
        println()
    end
end

let
    println("5. UniversalIdBuilder: Modifying existing UniversalIds")
    println("--------------------------------------------------------")

    original = from_string(
        UniversalIdBuilder,
        "data.dnv.com/IMO1234567/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature",
    )

    if original !== nothing
        println("Original                    : $(string(original))")

        v = vis()
        ver = latest(v)
        cbs = codebooks(v, ver)

        new_imo = ImoNumber(9074729)
        modified = build(with_imo_number(builder(original), new_imo))
        println("Modified (new IMO)          : $(string(modified))")

        cnt_tag = create_tag(cbs[VistaSdk.Content], "water")
        if cnt_tag !== nothing
            new_lb = with_metadata_tag(builder(local_id(original)), cnt_tag)
            modified2 = build(with_local_id(builder(original), new_lb))
            println("Modified (added content tag): $(string(modified2))")
        end
    end
    println()
end

let
    println("6. UniversalId: Accessing components")
    println("--------------------------------------")

    uid = from_string(
        UniversalIdBuilder,
        "data.dnv.com/IMO9074729/dnv-v2/vis-3-4a/621.21/S90/sec/411.1/C101/meta/qty-mass/cnt-fuel.oil/pos-inlet",
    )

    if uid !== nothing
        println("UniversalId: $(string(uid))\n")
        println("IMO Number:")
        println("  Value: $(string(imo_number(uid)))")

        println("\nLocalId components:")
        lid = local_id(uid)
        println("  VIS Version   : $(string(version(lid)))")
        println("  Primary item  : $(string(primary_item(lid)))")
        sec = secondary_item(lid)
        sec !== nothing && println("  Secondary item: $(string(sec))")

        println("\nMetadata tags:")
        qty = quantity(lid)
        qty !== nothing && println("  Quantity: $(value(qty))")
        cnt = content(lid)
        cnt !== nothing && println("  Content : $(value(cnt))")
        pos = VistaSdk.position(lid)
        pos !== nothing && println("  Position: $(value(pos))")
    end
    println()
end

let
    println("7. UniversalId: Equality and comparison")
    println("-----------------------------------------")

    uid1 = from_string(
        UniversalIdBuilder,
        "data.dnv.com/IMO1234567/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature",
    )
    uid2 = from_string(
        UniversalIdBuilder,
        "data.dnv.com/IMO1234567/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature",
    )
    uid3 = from_string(
        UniversalIdBuilder,
        "data.dnv.com/IMO9074729/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature",
    )
    uid4 = from_string(
        UniversalIdBuilder,
        "data.dnv.com/IMO1234567/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-pressure",
    )

    if uid1 !== nothing && uid2 !== nothing && uid3 !== nothing && uid4 !== nothing
        println("UniversalId1: Same IMO, same LocalId")
        println("UniversalId2: Same IMO, same LocalId")
        println("UniversalId3: Different IMO, same LocalId")
        println("UniversalId4: Same IMO, different LocalId\n")
        println("uid1 == uid2? $(uid1 == uid2) (same IMO & LocalId)")
        println("uid1 == uid3? $(uid1 == uid3) (different IMO)")
        println("uid1 == uid4? $(uid1 == uid4) (different LocalId)")
        println("uid1 != uid3? $(uid1 != uid3)")
    end
    println()
end

let
    println("8. UniversalIdBuilder: Validation")
    println("-----------------------------------")

    v = vis()
    ver = latest(v)
    g = gmod(v, ver)
    locs = locations(v, ver)
    cbs = codebooks(v, ver)

    imo = ImoNumber(1234567)
    primary = from_short_path("411.1/C101.31-2", g, locs)
    qty_tag = create_tag(cbs[VistaSdk.Quantity], "temperature")

    if primary !== nothing && qty_tag !== nothing
        lb = with_metadata_tag(
            with_primary_item(create(LocalIdBuilder, ver), primary),
            qty_tag,
        )

        valid_b = with_local_id(with_imo_number(create(UniversalIdBuilder, ver), imo), lb)
        println("Valid builder:")
        println("  Is valid: $(is_valid(valid_b))")

        missing_imo = with_local_id(create(UniversalIdBuilder, ver), lb)
        println("\nBuilder missing IMO number:")
        println("  Is valid: $(is_valid(missing_imo))")

        missing_lid = with_imo_number(create(UniversalIdBuilder, ver), imo)
        println("\nBuilder missing LocalId:")
        println("  Is valid: $(is_valid(missing_lid))")

        println("\nAttempting to build without IMO number:")
        try
            build(missing_imo)
            println("  ERROR: Invalid build succeeded (unexpected)")
        catch e
            println("  Correctly rejected: $e")
        end
    end
    println()
end

let
    println("9. UniversalId: Real-world vessel examples")
    println("---------------------------------------------")

    v = vis()
    ver = latest(v)
    g = gmod(v, ver)
    locs = locations(v, ver)
    cbs = codebooks(v, ver)

    sensors = [
        ("MV Queen Mary 2", 9241061, "411.1/C101.31-2", "temperature"),
        ("MS Oasis of the Seas", 9383936, "621.21/S90", "mass"),
        ("MS Symphony of the Seas", 9744001, "612.21/C701.23", "pressure"),
    ]

    for (vessel_name, imo_val, sensor_path, qty_val) in sensors
        imo = ImoNumber(imo_val)
        path = from_short_path(sensor_path, g, locs)
        tag = create_tag(cbs[VistaSdk.Quantity], qty_val)
        if path !== nothing && tag !== nothing
            lb =
                with_metadata_tag(with_primary_item(create(LocalIdBuilder, ver), path), tag)
            uid = build(
                with_local_id(with_imo_number(create(UniversalIdBuilder, ver), imo), lb),
            )
            println("$vessel_name:")
            println("  $(string(uid))\n")
        end
    end
end

let
    println("10. UniversalIdBuilder: Remove and add operations")
    println("---------------------------------------------------")

    original = from_string(
        UniversalIdBuilder,
        "data.dnv.com/IMO1234567/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature",
    )

    if original !== nothing
        println("Original: $(string(original))")

        wo_imo = without_imo_number(builder(original))
        println("\nAfter without_imo_number():")
        println("  Is valid: $(is_valid(wo_imo))")

        wo_lid = without_local_id(builder(original))
        println("\nAfter without_local_id():")
        println("  Is valid: $(is_valid(wo_lid))")

        v = vis()
        ver = latest(v)
        g = gmod(v, ver)
        locs = locations(v, ver)
        cbs = codebooks(v, ver)

        new_imo = ImoNumber(9074729)
        path = from_short_path("621.21/S90", g, locs)
        tag = create_tag(cbs[VistaSdk.Quantity], "mass")
        if path !== nothing && tag !== nothing
            new_lb =
                with_metadata_tag(with_primary_item(create(LocalIdBuilder, ver), path), tag)
            rebuilt = build(with_local_id(with_imo_number(wo_imo, new_imo), new_lb))
            println("\nRebuilt with new components: $(string(rebuilt))")
        end
    end
    println()
end
