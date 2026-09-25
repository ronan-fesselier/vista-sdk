using VistaSdk

println("=== VistaSdk ISO 19848 Sample ===\n")

let
    println("1. ISO19848: Singleton and version management")
    println("----------------------------------------------")

    iso = Iso19848()
    l = latest(iso)
    vs = versions(iso)
    println("Latest ISO 19848 version: ", l)
    println("Available versions (", length(vs), "):")
    for v in vs
        println("  - ", v)
    end
    println()
end

let
    println("2. DataChannelTypeNames: Accessing type names")
    println("-----------------------------------------------")

    iso = Iso19848()
    names = data_channel_type_names(iso, V2024)
    println("Data channel type names for v2024 (", length(names), "):")
    for entry in names
        println("  ", rpad(entry.type_, 20), " ", entry.description)
    end
    println()
end

let
    println("3. DataChannelTypeNames: Parsing and validation")
    println("-------------------------------------------------")

    iso = Iso19848()
    names = data_channel_type_names(iso, V2024)
    test_types = ["Inst", "Average", "SetPoint", "Alert", "invalid_type", "average"]
    for t in test_types
        entry = find(names, t)
        if entry !== nothing
            println("  '", rpad(t * "'", 15), " -> Valid\n    ", entry.description)
        else
            println("  '", rpad(t * "'", 15), " -> Invalid")
        end
    end
    println()
end

let
    println("4. FormatDataTypes: Accessing format types")
    println("--------------------------------------------")

    iso = Iso19848()
    types = format_data_types(iso, V2024)
    println("Format data types for v2024 (", length(types), "):")
    for entry in types
        println("  ", rpad(type_(entry), 10), " ", description(entry))
    end
    println()
end

let
    println("5. FormatDataTypes: Parsing and validation")
    println("--------------------------------------------")

    iso = Iso19848()
    types = format_data_types(iso, V2024)
    test_types =
        ["Decimal", "Integer", "Boolean", "String", "DateTime", "decimal", "INVALID"]
    for t in test_types
        fdt = find(types, t)
        if fdt !== nothing
            println("  '", rpad(t * "'", 11), " -> Valid (", type_(fdt), ")")
        else
            println("  '", rpad(t * "'", 11), " -> Invalid")
        end
    end
    println()
end

let
    println("6. FormatDataType: Validating decimal values")
    println("----------------------------------------------")

    iso = Iso19848()
    types = format_data_types(iso, V2024)
    decimal_type = find(types, "Decimal")
    test_values = ["123.456", "-456.789", "0.001", "999999.99", "not_a_number", ""]
    println("Validating Decimal values:")
    for v in test_values
        label = "'" * v * "'"
        result = validate(decimal_type, v) !== nothing ? "Valid" : "Invalid"
        println("  ", rpad(label, 18), " -> ", result)
    end
    println()
end

let
    println("7. FormatDataType: Validating integer values")
    println("----------------------------------------------")

    iso = Iso19848()
    types = format_data_types(iso, V2024)
    integer_type = find(types, "Integer")
    test_values = ["42", "-123", "0", "2147483647", "123.456", "not_an_integer"]
    println("Validating Integer values:")
    for v in test_values
        label = "'" * v * "'"
        result = validate(integer_type, v) !== nothing ? "Valid" : "Invalid"
        println("  ", rpad(label, 18), " -> ", result)
    end
    println()
end

let
    println("8. FormatDataType: Validating boolean and string values")
    println("---------------------------------------------------------")

    iso = Iso19848()
    types = format_data_types(iso, V2024)

    bool_type = find(types, "Boolean")
    println("Boolean validation:")
    for v in ["true", "false", "True", "FALSE", "1", "0", "maybe", "123", ""]
        label = "'" * v * "'"
        result = validate(bool_type, v) !== nothing ? "Valid" : "Invalid"
        println("  ", rpad(label, 11), " -> ", result)
    end
    println()

    string_type = find(types, "String")
    println("String validation (always valid):")
    for v in ["hello", "123", "", "with spaces", "special@chars!"]
        label = "'" * v * "'"
        result = validate(string_type, v) !== nothing ? "Valid" : "Invalid"
        println("  ", rpad(label, 20), " -> ", result)
    end
    println()
end

let
    println("9. FormatDataType: Validating DateTime values")
    println("-----------------------------------------------")

    iso = Iso19848()
    types = format_data_types(iso, V2024)
    dt_type = find(types, "DateTime")
    test_values = [
        "1994-11-20T10:25:33Z",
        "2024-12-31T23:59:59Z",
        "2000-01-01T00:00:00Z",
        "1994-11-20T10:25:33+02:00",
        "1994-11-20T10:25",
        "1994-11-20",
        "invalid_date",
    ]
    println("Validating DateTime values (exact format: yyyy-MM-ddTHH:mm:ssZ):")
    for v in test_values
        label = "'" * v * "'"
        result = validate(dt_type, v) !== nothing ? "Valid" : "Invalid"
        println("  ", rpad(label, 30), " -> ", result)
    end
    println()
end

let
    println("10. FormatDataType: Pattern matching with validated values")
    println("-----------------------------------------------------------")

    iso = Iso19848()
    types = format_data_types(iso, V2024)
    decimal_type = find(types, "Decimal")
    println("Converting temperature values using pattern matching:")
    for temp_str in ["23.5", "-15.2", "100.0", "not_a_number"]
        v = validate(decimal_type, temp_str)
        if v isa Decimal
            celsius = to_f64(v)
            fahrenheit = celsius * 9.0 / 5.0 + 32.0
            println("  ", lpad(temp_str, 8), "°C = ", round(fahrenheit; digits = 1), "°F")
        else
            println("  ", temp_str, " - Error: invalid decimal")
        end
    end
    println()
end

let
    println("11. Value: Creating and inspecting values")
    println("------------------------------------------")

    string_value = iso19848_value_from_string("sensor_data")
    integer_value = iso19848_value_from_integer(42)
    boolean_value = iso19848_value_from_boolean(true)
    decimal_value = iso19848_value_from_decimal(Decimal(123.456))
    datetime_value = iso19848_value_from_date_time(utc_now(DateTimeOffset))

    println("Value type inspection:")

    println("  String value:")
    println("    isString : ", string_value isa String)
    println("    isInteger: ", string_value isa Int64)
    println("    string() : ", string_value)

    println("\n  Integer value:")
    println("    isInteger: ", integer_value isa Int64)
    println("    isDecimal: ", integer_value isa Decimal)
    println("    integer(): ", integer_value)

    println("\n  Boolean value:")
    println("    isBoolean: ", boolean_value isa Bool)
    println("    boolean(): ", boolean_value)

    println("\n  Decimal value:")
    println("    isDecimal: ", decimal_value isa Decimal)
    println("    decimal(): ", decimal_value)

    println("\n  DateTime value:")
    println("    isDateTime: ", datetime_value isa DateTimeOffset)
    datetime_value isa DateTimeOffset && println("    dateTime(): has value")
    println()
end

let
    println("12. Value: Pattern matching")
    println("-----------------------------")

    values = Iso19848Value[
        iso19848_value_from_decimal(Decimal(23.5)),
        iso19848_value_from_integer(42),
        iso19848_value_from_boolean(true),
        iso19848_value_from_string("sensor_reading"),
        iso19848_value_from_date_time(utc_now(DateTimeOffset)),
    ]
    println("Processing values with pattern matching:")
    for v in values
        desc = if v isa Decimal
            "Decimal : " * string(v)
        elseif v isa Int64
            "Integer : " * string(v)
        elseif v isa Bool
            "Boolean : " * string(v)
        elseif v isa String
            "String  : " * v
        else
            "DateTime: " * string(v)
        end
        println("  ", desc)
    end
    println()
end

let
    println("13. Practical example: Sensor data validation pipeline")
    println("--------------------------------------------------------")

    iso = Iso19848()
    format_types = format_data_types(iso, V2024)

    readings = [
        (sensor_id = "TEMP_001", data_type = "Decimal", value = "23.5"),
        (sensor_id = "RPM_002", data_type = "Integer", value = "1500"),
        (sensor_id = "ALARM_003", data_type = "Boolean", value = "false"),
        (sensor_id = "STATUS_004", data_type = "String", value = "operational"),
        (sensor_id = "TIME_005", data_type = "DateTime", value = "2024-01-15T14:30:00Z"),
        (sensor_id = "INVALID_006", data_type = "Decimal", value = "not_a_number"),
    ]

    println("Processing sensor readings:")
    for r in readings
        println("\n  Sensor: ", r.sensor_id)
        println("    Type : ", r.data_type)
        println("    Value: ", r.value)
        fdt = find(format_types, r.data_type)
        if fdt === nothing
            println("    [ERROR] Unknown data type")
        elseif validate(fdt, r.value) !== nothing
            println("    [OK] Valid - Ready for storage/processing")
        else
            println("    [ERROR] Invalid")
        end
    end
    println()
end
