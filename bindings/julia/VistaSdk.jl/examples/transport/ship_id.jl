using VistaSdk

println("=== VistaSdk.jl ShipId Sample ===\n")

let
    println("1. ShipId: Constructing from an IMO number")
    println("----------------------------------------------")

    imo = parse(ImoNumber, "9074729")
    s = from_imo_number(imo)

    println("  is_imo_number: $(is_imo_number(s))")
    println("  is_other_id  : $(is_other_id(s))")
    println("  string       : $(string(s))")
    println()
end

let
    println("2. ShipId: Constructing from an alternative identifier")
    println("-----------------------------------------------------------")

    s = from_other_id("VESSEL-XYZ-789")

    println("  is_imo_number: $(is_imo_number(s))")
    println("  is_other_id  : $(is_other_id(s))")
    println("  string       : $(string(s))")
    println()
end

let
    println("3. ShipId::from_string: Parsing header ShipID values")
    println("--------------------------------------------------------")

    test_values = ["IMO9074729", "imo9074729", "IMO1234568", "VESSEL-ABC-123", ""]

    for v in test_values
        label = isempty(v) ? "(empty)" : "'$v'"
        println("  $label")
        s = from_string(ShipId, v)
        if s === nothing
            println("    -> (invalid, empty input)")
        elseif is_imo_number(s)
            println("    -> IMO number: $(string(s))")
        else
            println("    -> Alternative id: $(string(s))")
        end
    end
    println()
end

let
    println("4. ShipId: Dispatching on the identifier kind")
    println("----------------------------------------------")

    ship_ids =
        ShipId[from_imo_number(parse(ImoNumber, "9785811")), from_other_id("BARGE-042")]

    for s in ship_ids
        desc = if s isa ShipIdImo
            "IMO number  : $(string(s))"
        else
            "Alternative : $(other_id(s))"
        end
        println("  $desc")
    end
    println()
end

let
    println("5. ShipId: Equality comparison")
    println("----------------------------------")

    a = from_imo_number(parse(ImoNumber, "9074729"))
    b = from_imo_number(parse(ImoNumber, "9074729"))
    c = from_imo_number(parse(ImoNumber, "9785811"))

    @assert a == b
    @assert a != c
    println("  IMO9074729 == IMO9074729 : $(a == b)")
    println("  IMO9074729 == IMO9785811 : $(a == c)")
    println()
end

let
    println("6. ShipId: Empty other_id throws")
    println("----------------------------------")

    try
        from_other_id("")
        println("  ERROR: accepted unexpectedly")
    catch e
        println("  Correctly rejected: $e")
    end
    println()
end
