using VistaSdk

println("=== vista-sdk ImoNumber Sample ===\n")

let
    println("1. ImoNumber: Creating from integer")
    println("----------------------------------------")

    imo1 = ImoNumber(9074729)
    imo2 = ImoNumber(9785811)
    imo3 = ImoNumber(1234567)

    println("Created IMO numbers:")
    println("  ", imo1)
    println("  ", imo2)
    println("  ", imo3)

    println("\nAttempting to create invalid IMO number (1234507):")
    try
        ImoNumber(1234507)
        println("  ERROR: Invalid number accepted (unexpected)")
    catch e
        println("  Correctly rejected: ", e)
    end

    println()
end

let
    println("2. ImoNumber: Creating from string")
    println("---------------------------------------")

    imo1 = parse(ImoNumber, "IMO9074729")
    println("From 'IMO9074729': ", imo1)

    imo2 = parse(ImoNumber, "9785811")
    println("From '9785811'   : ", imo2)

    println("Are they equal? ", imo1 == parse(ImoNumber, "IMO9074729"))
    println("Are they equal? ", imo2 == parse(ImoNumber, "IMO9785811"))

    println("\nAttempting to create from invalid string ('IM9074729'):")
    try
        parse(ImoNumber, "IM9074729")
        println("  ERROR: Invalid string accepted (unexpected)")
    catch e
        println("  Correctly rejected: ", e)
    end

    println()
end

let
    println("3. is_valid: Validating integers")
    println("---------------------------------------------------")

    test_numbers = [9074729, 9785811, 1234567, 1234507, 123456, 12345678, 0, -1]

    println("Validation results:")
    for num in test_numbers
        valid = is_valid(num)
        println("  ", lpad(num, 10), ": ", valid)
    end

    println()
end

let
    println("4. ImoNumber: Safe parsing with parse()")
    println("------------------------------------------")

    test_strings = ["9074729", "IMO9785811", "1234567", "1234507", "IM9074729", "", "abc"]

    println("Parsing results:")
    for s in test_strings
        padding = max(10 - length(s), 0) + 1
        print("  '", s, "'", " "^padding, "-> ")
        try
            println(parse(ImoNumber, s))
        catch
            println("Invalid")
        end
    end

    println()
end

let
    println("5. ImoNumber: Parse patterns")
    println("------------------------------")

    input = "IMO9074729"

    try
        imo = parse(ImoNumber, input)
        println("Parsed successfully: ", imo)
    catch
        println("Parse failed")
    end

    try
        imo = parse(ImoNumber, "9785811")
        println("Parsed successfully: ", imo)
    catch
    end

    println()
end

let
    println("6. ImoNumber: Comparison operators")
    println("-------------------------------------")

    imo1 = ImoNumber(9074729)
    imo2 = parse(ImoNumber, "IMO9074729")
    imo3 = ImoNumber(9785811)

    println("Equality:")
    println("  9074729 == IMO9074729? ", imo1 == imo2)
    println("  9074729 == 9785811   ? ", imo1 == imo3)

    println("\nInequality:")
    println("  9074729 != 9785811   ? ", imo1 != imo3)
    println("  9074729 != IMO9074729? ", imo1 != imo2)

    println()
end

let
    println("7. ImoNumber: string() formatting")
    println("----------------------------------------")

    imo = ImoNumber(9074729)

    println("String representation:")
    println("  string(): ", string(imo))
    println("  Format  : Always includes \"IMO\" prefix")
    println("  Format  : Always 7 digits after \"IMO\"")

    println("\nConsistent output:")
    println("  From int 9074729 : ", ImoNumber(9074729))
    println("  From '9074729'   : ", parse(ImoNumber, "9074729"))
    println("  From 'IMO9074729': ", parse(ImoNumber, "IMO9074729"))

    println()
end

let
    println("8. ImoNumber: Real-world examples")
    println("--------------------------------------")

    ships = [
        ("Queen Mary 2", 9241061),
        ("Peter Faber", 8027781),
        ("Oasis of the Seas", 9383936),
        ("Symphony of the Seas", 9744001),
    ]

    println("Famous ships:")
    for (name, imo_number) in ships
        if is_valid(imo_number)
            imo = ImoNumber(imo_number)
            println("  ", rpad(name, 20), ": ", imo)
        end
    end

    println()
end

let
    println("9. ImoNumber: Error handling patterns")
    println("-----------------------------------------")

    inputs = ["9074729", "1234507"]

    for user_input in inputs
        println("\nTesting with '", user_input, "':")

        println("  Pattern 1: parse() (throws on error):")
        try
            imo = parse(ImoNumber, user_input)
            println("    Success: ", imo)
        catch e
            println("    Error: ", e)
        end

        println("  Pattern 2: try/catch (propagatable):")
        try
            imo = parse(ImoNumber, user_input)
            println("    Success: ", imo)
        catch e
            println("    Error: ", e)
        end

        println("  Pattern 3: try/catch returning nothing (no diagnostic needed):")
        imo = try
            parse(ImoNumber, user_input)
        catch
            nothing
        end
        if imo !== nothing
            println("    Success: ", imo)
        else
            println("    Invalid IMO number")
        end
    end

    println()
end
