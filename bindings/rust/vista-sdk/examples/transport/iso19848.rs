use vista_sdk::transport::iso19848::{Iso19848, Value};
use vista_sdk::transport::iso19848_version::Iso19848Version;
use vista_sdk::types::date_time_offset::DateTimeOffset;
use vista_sdk::types::decimal::Decimal;

fn main() {
    println!("=== vista-sdk ISO 19848 Sample ===\n");

    {
        println!("1. ISO19848: Singleton and version management");
        println!("------------------------------------------------");

        let iso = Iso19848::instance();
        let latest = iso.latest();
        let versions = iso.versions();

        println!("Latest ISO 19848 version: {}", latest);
        println!("Available versions ({}):", versions.len());
        for v in &versions {
            println!("  - {}", v);
        }
        println!();
    }

    {
        println!("2. DataChannelTypeNames: Accessing type names");
        println!("------------------------------------------------");

        let iso = Iso19848::instance();
        let names = iso.data_channel_type_names(Iso19848Version::V2024).unwrap();

        println!("Data channel type names for v2024 ({}):", names.len());
        for entry in names.iter() {
            println!("  {:<20} {}", entry.type_, entry.description);
        }
        println!();
    }

    {
        println!("3. DataChannelTypeNames: Parsing and validation");
        println!("--------------------------------------------------");

        let iso = Iso19848::instance();
        let names = iso.data_channel_type_names(Iso19848Version::V2024).unwrap();

        let test_types = [
            "Inst",
            "Average",
            "SetPoint",
            "Alert",
            "invalid_type",
            "average",
        ];

        for type_str in &test_types {
            match names.from_str(type_str) {
                Some(entry) => println!(
                    "  '{:<15} -> Valid\n    {}",
                    format!("{}\'", type_str),
                    entry.description
                ),
                None => println!("  '{:<15} -> Invalid", format!("{}\'", type_str)),
            }
        }
        println!();
    }

    {
        println!("4. FormatDataTypes: Accessing format types");
        println!("---------------------------------------------");

        let iso = Iso19848::instance();
        let types = iso.format_data_types(Iso19848Version::V2024).unwrap();

        println!("Format data types for v2024 ({}):", types.len());
        for entry in types.iter() {
            println!("  {:<10} {}", entry.type_(), entry.description());
        }
        println!();
    }

    {
        println!("5. FormatDataTypes: Parsing and validation");
        println!("---------------------------------------------");

        let iso = Iso19848::instance();
        let types = iso.format_data_types(Iso19848Version::V2024).unwrap();

        let test_types = [
            "Decimal", "Integer", "Boolean", "String", "DateTime", "decimal", "INVALID",
        ];

        for type_str in &test_types {
            match types.from_str(type_str) {
                Some(t) => println!(
                    "  '{:<11} -> Valid ({})",
                    format!("{}\'", type_str),
                    t.type_()
                ),
                None => println!("  '{:<11} -> Invalid", format!("{}\'", type_str)),
            }
        }
        println!();
    }

    {
        println!("6. FormatDataType: Validating decimal values");
        println!("-----------------------------------------------");

        let iso = Iso19848::instance();
        let types = iso.format_data_types(Iso19848Version::V2024).unwrap();
        let decimal_type = types.from_str("Decimal").unwrap();

        let test_values = [
            "123.456",
            "-456.789",
            "0.001",
            "999999.99",
            "not_a_number",
            "",
        ];

        println!("Validating Decimal values:");
        for value in &test_values {
            let label = format!("'{}'", value);
            match decimal_type.validate(value) {
                Some(_) => println!("  {:<18} -> Valid", label),
                None => println!("  {:<18} -> Invalid", label),
            }
        }
        println!();
    }

    {
        println!("7. FormatDataType: Validating integer values");
        println!("-----------------------------------------------");

        let iso = Iso19848::instance();
        let types = iso.format_data_types(Iso19848Version::V2024).unwrap();
        let integer_type = types.from_str("Integer").unwrap();

        let test_values = ["42", "-123", "0", "2147483647", "123.456", "not_an_integer"];

        println!("Validating Integer values:");
        for value in &test_values {
            let label = format!("'{}'", value);
            match integer_type.validate(value) {
                Some(_) => println!("  {:<18} -> Valid", label),
                None => println!("  {:<18} -> Invalid", label),
            }
        }
        println!();
    }

    {
        println!("8. FormatDataType: Validating boolean and string values");
        println!("----------------------------------------------------------");

        let iso = Iso19848::instance();
        let types = iso.format_data_types(Iso19848Version::V2024).unwrap();

        let bool_type = types.from_str("Boolean").unwrap();
        println!("Boolean validation:");
        for value in &[
            "true", "false", "True", "FALSE", "1", "0", "maybe", "123", "",
        ] {
            let label = format!("'{}'", value);
            let result = if bool_type.validate(value).is_some() {
                "Valid"
            } else {
                "Invalid"
            };
            println!("  {:<11} -> {}", label, result);
        }

        println!();

        let string_type = types.from_str("String").unwrap();
        println!("String validation (always valid):");
        for value in &["hello", "123", "", "with spaces", "special@chars!"] {
            let label = format!("'{}'", value);
            let result = if string_type.validate(value).is_some() {
                "Valid"
            } else {
                "Invalid"
            };
            println!("  {:<20} -> {}", label, result);
        }
        println!();
    }

    {
        println!("9. FormatDataType: Validating DateTime values");
        println!("------------------------------------------------");

        let iso = Iso19848::instance();
        let types = iso.format_data_types(Iso19848Version::V2024).unwrap();
        let dt_type = types.from_str("DateTime").unwrap();

        let test_values = [
            "1994-11-20T10:25:33Z",
            "2024-12-31T23:59:59Z",
            "2000-01-01T00:00:00Z",
            "1994-11-20T10:25:33+02:00",
            "1994-11-20T10:25",
            "1994-11-20",
            "invalid_date",
        ];

        println!("Validating DateTime values (exact format: yyyy-MM-ddTHH:mm:ssZ):");
        for value in &test_values {
            let label = format!("'{}'", value);
            let result = if dt_type.validate(value).is_some() {
                "Valid"
            } else {
                "Invalid"
            };
            println!("  {:<30} -> {}", label, result);
        }
        println!();
    }

    {
        println!("10. FormatDataType: Pattern matching with validated values");
        println!("-------------------------------------------------------------");

        let iso = Iso19848::instance();
        let types = iso.format_data_types(Iso19848Version::V2024).unwrap();
        let decimal_type = types.from_str("Decimal").unwrap();

        println!("Converting temperature values using pattern matching:");

        for temp_str in &["23.5", "-15.2", "100.0", "not_a_number"] {
            match decimal_type.validate(temp_str) {
                Some(Value::Decimal(d)) => {
                    let celsius = d.to_f64();
                    let fahrenheit = celsius * 9.0 / 5.0 + 32.0;
                    println!("  {:>8}°C = {:.1}°F", temp_str, fahrenheit);
                }
                Some(_) => {}
                None => println!("  {} - Error: invalid decimal", temp_str),
            }
        }
        println!();
    }

    {
        println!("11. Value: Creating and inspecting values");
        println!("--------------------------------------------");

        let string_value = Value::from_string("sensor_data").unwrap();
        let integer_value = Value::from_integer(42);
        let boolean_value = Value::from_boolean(true);
        let decimal_value = Value::from_decimal(Decimal::from_f64(123.456));
        let datetime_value = Value::from_date_time(DateTimeOffset::now());

        println!("Value type inspection:");

        println!("  String value:");
        println!(
            "    isString : {}",
            matches!(string_value, Value::String(_))
        );
        println!(
            "    isInteger: {}",
            matches!(string_value, Value::Integer(_))
        );
        if let Value::String(ref s) = string_value {
            println!("    string() : {}", s);
        }

        println!("\n  Integer value:");
        println!(
            "    isInteger: {}",
            matches!(integer_value, Value::Integer(_))
        );
        println!(
            "    isDecimal: {}",
            matches!(integer_value, Value::Decimal(_))
        );
        if let Value::Integer(i) = integer_value {
            println!("    integer(): {}", i);
        }

        println!("\n  Boolean value:");
        println!(
            "    isBoolean: {}",
            matches!(boolean_value, Value::Boolean(_))
        );
        if let Value::Boolean(b) = boolean_value {
            println!("    boolean(): {}", b);
        }

        println!("\n  Decimal value:");
        println!(
            "    isDecimal: {}",
            matches!(decimal_value, Value::Decimal(_))
        );
        if let Value::Decimal(ref d) = decimal_value {
            println!("    decimal(): {}", d);
        }

        println!("\n  DateTime value:");
        println!(
            "    isDateTime: {}",
            matches!(datetime_value, Value::DateTime(_))
        );
        if matches!(datetime_value, Value::DateTime(_)) {
            println!("    dateTime(): has value");
        }

        println!();
    }

    {
        println!("12. Value: Pattern matching");
        println!("-----------------------------");

        let values = vec![
            Value::from_decimal(Decimal::from_f64(23.5)),
            Value::from_integer(42),
            Value::from_boolean(true),
            Value::from_string("sensor_reading").unwrap(),
            Value::from_date_time(DateTimeOffset::now()),
        ];

        println!("Processing values with pattern matching:");

        for value in &values {
            let description = match value {
                Value::Decimal(d) => format!("Decimal : {}", d),
                Value::Integer(i) => format!("Integer : {}", i),
                Value::Boolean(b) => format!("Boolean : {}", b),
                Value::String(s) => format!("String  : {}", s),
                Value::DateTime(dt) => format!("DateTime: {}", dt),
            };
            println!("  {}", description);
        }
        println!();
    }

    {
        println!("13. Practical example: Sensor data validation pipeline");
        println!("---------------------------------------------------------");

        let iso = Iso19848::instance();
        let format_types = iso.format_data_types(Iso19848Version::V2024).unwrap();

        struct SensorReading {
            sensor_id: &'static str,
            data_type: &'static str,
            value: &'static str,
        }

        let readings = [
            SensorReading {
                sensor_id: "TEMP_001",
                data_type: "Decimal",
                value: "23.5",
            },
            SensorReading {
                sensor_id: "RPM_002",
                data_type: "Integer",
                value: "1500",
            },
            SensorReading {
                sensor_id: "ALARM_003",
                data_type: "Boolean",
                value: "false",
            },
            SensorReading {
                sensor_id: "STATUS_004",
                data_type: "String",
                value: "operational",
            },
            SensorReading {
                sensor_id: "TIME_005",
                data_type: "DateTime",
                value: "2024-01-15T14:30:00Z",
            },
            SensorReading {
                sensor_id: "INVALID_006",
                data_type: "Decimal",
                value: "not_a_number",
            },
        ];

        println!("Processing sensor readings:");

        for r in &readings {
            println!("\n  Sensor: {}", r.sensor_id);
            println!("    Type : {}", r.data_type);
            println!("    Value: {}", r.value);

            match format_types.from_str(r.data_type) {
                Some(fmt) => match fmt.validate(r.value) {
                    Some(_) => println!("    [OK] Valid - Ready for storage/processing"),
                    None => println!("    [ERROR] Invalid"),
                },
                None => println!("    [ERROR] Unknown data type"),
            }
        }
        println!();
    }
}
