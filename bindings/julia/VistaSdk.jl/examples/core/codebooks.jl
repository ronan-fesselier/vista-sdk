using VistaSdk

println("=== VistaSdk.jl Codebooks Sample ===\n")

let
    println("1. VIS Singleton: version management")
    println("-------------------------------------------------------")

    v = vis()
    all_versions = versions(v)

    println("Latest VIS version : $(latest(v))")
    println("Available versions ($(length(all_versions))):")
    for ver in all_versions
        println("  - $ver")
    end
    println()
end

let
    println("2. Codebooks: accessing codebooks for the latest version")
    println("-------------------------------------------------------")

    v = vis()
    cbs = codebooks(v, latest(v))

    println("Codebooks for version: $(version(cbs))")
    println(
        "Quantity has $(lpad(length(standard_values(cbs[VistaSdk.Quantity])), 3)) standard values",
    )
    println(
        "Position has $(lpad(length(standard_values(cbs[VistaSdk.Position])), 3)) standard values",
    )
    println(
        "State    has $(lpad(length(standard_values(cbs[VistaSdk.State])),    3)) standard values",
    )
    println()
end

let
    println("3. Codebook: standard values and groups")
    println("-------------------------------------------------------")

    v = vis()
    cbs = codebooks(v, latest(v))
    qty = cbs[VistaSdk.Quantity]

    println(
        "has_standard_value(qty, \"temperature\") = $(has_standard_value(qty, "temperature"))",
    )
    println(
        "has_standard_value(qty, \"pressure\")    = $(has_standard_value(qty, "pressure"))",
    )
    println(
        "has_standard_value(qty, \"invalid_qty\") = $(has_standard_value(qty, "invalid_qty"))",
    )

    qty_groups = groups(qty)
    println("\nGroups in Quantity ($(length(qty_groups))):")
    for g in qty_groups
        println("  - $g")
    end
    println()
end

let
    println("4. Position codebook: numeric value handling")
    println("-------------------------------------------------------")

    v = vis()
    cbs = codebooks(v, latest(v))
    pos = cbs[VistaSdk.Position]

    for v_str in ["1", "03", "42", "1234", "centre", "port", "starboard", "invalid"]
        println("  has_standard_value(pos, \"$v_str\") = $(has_standard_value(pos, v_str))")
    end
    println()
end

let
    println("5. MetadataTag: creating tags from standard values")
    println("-------------------------------------------------------")

    v = vis()
    cbs = codebooks(v, latest(v))
    qty = cbs[VistaSdk.Quantity]
    tag = create_tag(qty, "temperature")

    if tag !== nothing
        println("Temperature tag:")
        println("  name:      $(name(tag))")
        println("  value:     $(value(tag))")
        println("  is_custom: $(is_custom(tag))")
        println("  string:    $tag")
    end
    println()
end

let
    println("6. MetadataTag: custom tags")
    println("-------------------------------------------------------")

    v = vis()
    cbs = codebooks(v, latest(v))
    qty = cbs[VistaSdk.Quantity]
    custom_tag = create_tag(qty, "custom_measurement")

    if custom_tag !== nothing
        println("Custom tag:")
        println("  name:      $(name(custom_tag))")
        println("  value:     $(value(custom_tag))")
        println("  is_custom: $(is_custom(custom_tag))")
        println("  string:    $custom_tag")
    end
    println()
end

let
    println("7. Position validation")
    println("-------------------------------------------------------")

    v = vis()
    cbs = codebooks(v, latest(v))
    pos = cbs[VistaSdk.Position]

    test_positions = [
        "centre",
        "port",
        "1",
        "42",
        "centre-starboard",
        "port-1",
        "centre-starboard-2",
        "invalid position!",
        "starboard-centre",
    ]

    for p in test_positions
        result = validate_position(pos, p)
        label = if result == VistaSdk.Valid
            "Valid"
        elseif result == VistaSdk.Custom
            "Custom"
        elseif result == VistaSdk.Invalid
            "Invalid"
        elseif result == VistaSdk.InvalidOrder
            "Invalid (wrong order)"
        else
            "Invalid (duplicate groups)"
        end
        println("  $(rpad(repr(p), 24)): $label")
    end
    println()
end

let
    println("8. CodebookName: string conversions")
    println("-------------------------------------------------------")

    for nm in [
        VistaSdk.Quantity,
        VistaSdk.Position,
        VistaSdk.State,
        VistaSdk.Command,
        VistaSdk.Detail,
    ]
        println("  $(rpad(string(nm), 24)) -> $(codebook_name_to_prefix(nm))")
    end
    println()
    for prefix in ["qty", "pos", "cnt"]
        result = codebook_name_from_prefix(prefix)
        println("  from_prefix(\"$prefix\") = $result")
    end
    println()
end

let
    println("9. Invalid operations")
    println("-------------------------------------------------------")

    v = vis()
    cbs = codebooks(v, latest(v))
    qty = cbs[VistaSdk.Quantity]

    println("create_tag(qty, \"\")    = $(create_tag(qty, ""))")
    println("create_tag(qty, \"   \") = $(create_tag(qty, "   "))")
    println("from_prefix(\"invalid\") = $(codebook_name_from_prefix("invalid"))")
    println()
end

let
    println("10. Building a complete metadata path")
    println("-------------------------------------------------------")

    v = vis()
    cbs = codebooks(v, latest(v))

    tags = [
        create_tag(cbs[VistaSdk.Quantity], "temperature"),
        create_tag(cbs[VistaSdk.Content], "oil"),
        create_tag(cbs[VistaSdk.Position], "1"),
        create_tag(cbs[VistaSdk.State], "running"),
    ]

    if all(!isnothing, tags)
        println("Metadata path segments:")
        for t in tags
            println("  $t")
        end
    end
    println()
end
