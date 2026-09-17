using VistaSdk

println("=== VistaSdk.jl Locations Sample ===\n")

let
    println("1. Locations: accessing location data for a VIS version")
    println("-------------------------------------------------------")

    v = vis()
    ver = latest(v)
    locs = locations(v, ver)

    println("Locations for version   : $ver")
    println("Total relative locations: $(length(locs))")
    println()
end

let
    println("2. Location Parsing: valid location strings")
    println("-------------------------------------------------------")

    v = vis()
    locs = locations(v, latest(v))

    valid_strs = ["1", "5", "42", "1P", "2CF", "3SU", "10FI", "CFOU"]
    for s in valid_strs
        loc = parse(Location, locs, s)
        result = loc !== nothing ? "Valid: '$(string(loc))'" : "Invalid (unexpected)"
        println("  $(rpad("'$s'", 9))-> $result")
    end
    println()
end

let
    println("3. Location Parsing: invalid strings with errors")
    println("-------------------------------------------------------")

    v = vis()
    locs = locations(v, latest(v))

    invalid_strs = [
        "",
        "   ",
        "X",
        "1X",
        "ZPS",
        "PC",
        "1PS",
        "1UL",
        "2IO",
        "SP1",
        "1SPA",
        "10PSFI",
        "ACFIMOPSU",
    ]
    for s in invalid_strs
        loc, errors = parse_with_errors(locs, s)
        println("  \"$s\" -> Invalid")
        if loc === nothing && has_errors(errors)
            println("    Errors:")
            for e in errors
                println("      - $(e.message)")
            end
        end
    end
    println()
end

let
    println("4. RelativeLocations: exploring location metadata")
    println("-------------------------------------------------------")

    v = vis()
    locs = locations(v, latest(v))

    println("Location codes and their meanings:")
    for rl in locs
        def = definition(rl)
        suffix = def !== nothing ? " ($def)" : ""
        println("  $(code(rl)) - $(name(rl))$suffix")
    end
    println()
end

let
    println("5. Location Groups: grouped location codes")
    println("-------------------------------------------------------")

    v = vis()
    locs = locations(v, latest(v))

    println("Location groups:")
    for (grp, label) in [
        (VistaSdk.Side, "Side:"),
        (VistaSdk.Vertical, "Vertical:"),
        (VistaSdk.Transverse, "Transverse:"),
        (VistaSdk.Longitudinal, "Longitudinal:"),
    ]
        entries = group(locs, grp)
        isempty(entries) && continue
        codes = join([string(code(rl)) for rl in entries], ", ")
        println("  $(rpad(label, 14))$codes")
    end
    println()
end

let
    println("6. LocationBuilder: creating locations with fluent API")
    println("-------------------------------------------------------")

    v = vis()
    locs = locations(v, latest(v))

    b = with_number(LocationBuilder(locs), 1)
    println("Building location step by step:")
    println("  After with_number(1)        : $b")
    b = with_side(b, 'P')
    println("  After with_side('P')        : $b")
    b = with_vertical(b, 'U')
    println("  After with_vertical('U')    : $b")
    b = with_transverse(b, 'I')
    println("  After with_transverse('I')  : $b")
    b = with_longitudinal(b, 'F')
    println("  After with_longitudinal('F'): $b")
    loc = build(b)
    println("  Final location              : $(string(loc))\n")
end

let
    println("7. LocationBuilder: method chaining")
    println("-------------------------------------------------------")

    v = vis()
    locs = locations(v, latest(v))

    loc = build(
        with_longitudinal(
            with_vertical(with_side(with_number(LocationBuilder(locs), 5), 'S'), 'M'),
            'A',
        ),
    )
    println("Location built with chaining: $(string(loc))\n")
end

let
    println("8. LocationBuilder: parsing existing locations")
    println("-------------------------------------------------------")

    v = vis()
    locs = locations(v, latest(v))

    existing = parse(Location, locs, "2CF")
    @assert existing !== nothing
    b = with_location(LocationBuilder(locs), existing)
    println("Original location: $(string(existing))")
    println("Builder state after parsing:")
    println("  Number      : $(something(number(b), "none"))")
    println("  Side        : $(something(side(b), "none"))")
    println("  Vertical    : $(something(vertical(b), "none"))")
    println("  Transverse  : $(something(transverse(b), "none"))")
    println("  Longitudinal: $(something(longitudinal(b), "none"))\n")
end

let
    println("9. LocationBuilder: modifying locations")
    println("-------------------------------------------------------")

    v = vis()
    locs = locations(v, latest(v))

    original = parse(Location, locs, "1P")
    @assert original !== nothing
    println("Original: $(string(original))")

    modified = build(
        with_vertical(without_side(with_location(LocationBuilder(locs), original)), 'U'),
    )
    println("Modified (removed side, added vertical)   : $(string(modified))")

    variation = build(
        with_vertical(with_number(with_location(LocationBuilder(locs), original), 5), 'L'),
    )
    println("Variation (changed number, added vertical): $(string(variation))\n")
end

let
    println("10. LocationBuilder: with_code() and with_number()")
    println("-------------------------------------------------------")

    v = vis()
    locs = locations(v, latest(v))

    loc = build(
        with_code(
            with_code(
                with_code(with_code(with_number(LocationBuilder(locs), 3), 'C'), 'M'),
                'O',
            ),
            'F',
        ),
    )
    println("Location built with with_code(): $(string(loc))")
    println("Components automatically routed to correct groups\n")
end

let
    println("11. LocationBuilder: removing components")
    println("-------------------------------------------------------")

    v = vis()
    locs = locations(v, latest(v))

    b = with_longitudinal(
        with_transverse(
            with_vertical(with_side(with_number(LocationBuilder(locs), 1), 'P'), 'U'),
            'I',
        ),
        'F',
    )
    println("Full location                : $b")
    b = without_number(b)
    println("After without_number()       : $b")
    b = without_transverse(b)
    println("After without_transverse()   : $b")
    b = without_value(b, VistaSdk.Vertical)
    println("After without_value(Vertical): $b\n")
end

let
    println("12. LocationBuilder: validation errors")
    println("-------------------------------------------------------")

    v = vis()
    locs = locations(v, latest(v))

    println("Testing invalid operations:")
    try
        with_number(LocationBuilder(locs), 0)
        println("  with_number(0): Accepted (unexpected)")
    catch e
        println("  with_number(0): Rejected - $e")
    end

    try
        with_side(LocationBuilder(locs), 'X')
        println("  with_side('X'): Accepted (unexpected)")
    catch e
        println("  with_side('X'): Rejected - $e")
    end

    try
        with_code(LocationBuilder(locs), 'Z')
        println("  with_code('Z'): Accepted (unexpected)")
    catch e
        println("  with_code('Z'): Rejected - $e")
    end
    println()
end

let
    println("13. Complete workflow: parse, modify, validate")
    println("-------------------------------------------------------")

    v = vis()
    locs = locations(v, latest(v))

    user_input = "3SU"
    parsed = parse(Location, locs, user_input)
    if parsed !== nothing
        println("User input: \"$user_input\" -> Valid")
        modified = build(
            with_longitudinal(
                with_vertical(
                    with_number(with_location(LocationBuilder(locs), parsed), 10),
                    'U',
                ),
                'F',
            ),
        )
        println("Modified location: $(string(modified))")
        revalidated = parse(Location, locs, string(modified))
        if revalidated !== nothing
            println("Modified location is valid: $(string(revalidated))")
        end
    else
        println("User input: \"$user_input\" -> Invalid")
    end
    println()
end
