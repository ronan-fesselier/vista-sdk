use vista_sdk::core::imo_number::ImoNumber;

fn main() {
    println!("=== vista-sdk ImoNumber Sample ===\n");

    {
        println!("1. ImoNumber: Creating from integer");
        println!("----------------------------------------");

        // Create valid IMO numbers
        let imo1 = ImoNumber::create(9074729).expect("9074729 should be a valid IMO number");
        let imo2 = ImoNumber::create(9785811).expect("9785811 should be a valid IMO number");
        let imo3 = ImoNumber::create(1234567).expect("1234567 should be a valid IMO number");

        println!("Created IMO numbers:");
        println!("  {}", imo1);
        println!("  {}", imo2);
        println!("  {}", imo3);

        // Try invalid number (returns Err)
        println!("\nAttempting to create invalid IMO number (1234507):");
        match ImoNumber::create(1234507) {
            Ok(_) => println!("  ERROR: Invalid number accepted (unexpected)"),
            Err(e) => println!("  Correctly rejected: {}", e),
        }

        println!();
    }

    {
        println!("2. ImoNumber: Creating from string");
        println!("---------------------------------------");

        // With "IMO" prefix
        let imo1 = "IMO9074729"
            .parse::<ImoNumber>()
            .expect("IMO9074729 should be a valid IMO number");
        println!("From 'IMO9074729': {}", imo1);

        // Without "IMO" prefix
        let imo2 = "9785811"
            .parse::<ImoNumber>()
            .expect("9785811 should be a valid IMO number");
        println!("From '9785811'   : {}", imo2);

        // Both create equivalent objects
        println!(
            "Are they equal? {}",
            imo1 == "IMO9074729".parse::<ImoNumber>().unwrap()
        );
        println!(
            "Are they equal? {}",
            imo2 == "IMO9785811".parse::<ImoNumber>().unwrap()
        );

        // Try invalid string (returns Err)
        println!("\nAttempting to create from invalid string ('IM9074729'):");
        match "IM9074729".parse::<ImoNumber>() {
            Ok(_) => println!("  ERROR: Invalid string accepted (unexpected)"),
            Err(e) => println!("  Correctly rejected: {}", e),
        }

        println!();
    }

    {
        println!("3. ImoNumber::is_valid(): Validating integers");
        println!("---------------------------------------------------");

        let test_numbers = [
            9074729,  // Valid
            9785811,  // Valid
            1234567,  // Valid
            1234507,  // Invalid checksum
            123456,   // Too short
            12345678, // Too long
            0,        // Invalid
            -1,       // Invalid
        ];

        println!("Validation results:");
        for num in test_numbers {
            let valid = ImoNumber::is_valid(num);
            println!("  {:>10}: {}", num, valid);
        }

        println!();
    }

    {
        println!("4. ImoNumber: Safe parsing with .parse()");
        println!("------------------------------------------");

        let test_strings = [
            "9074729",
            "IMO9785811",
            "1234567",   // Valid
            "1234507",   // Invalid checksum
            "IM9074729", // Typo in prefix
            "",          // Empty
            "abc",       // Not a number
        ];

        println!("Parsing results:");
        for s in test_strings {
            let padding = 10usize.saturating_sub(s.len()) + 1;
            print!("  '{}'{}-> ", s, " ".repeat(padding));

            match s.parse::<ImoNumber>() {
                Ok(imo) => println!("{}", imo),
                Err(_) => println!("Invalid"),
            }
        }

        println!();
    }

    {
        println!("5. ImoNumber: Parse patterns");
        println!("------------------------------");

        let input = "IMO9074729";

        // Pattern 1: Result with match
        match input.parse::<ImoNumber>() {
            Ok(imo) => println!("Parsed successfully: {}", imo),
            Err(_) => println!("Parse failed"),
        }

        // Pattern 2: Option via .ok()
        if let Ok(imo) = "9785811".parse::<ImoNumber>() {
            println!("Parsed successfully: {}", imo);
        }

        println!();
    }

    {
        println!("6. ImoNumber: Comparison operators");
        println!("-------------------------------------");

        let imo1 = ImoNumber::create(9074729).expect("9074729 should be a valid IMO number");
        let imo2 = "IMO9074729"
            .parse::<ImoNumber>()
            .expect("IMO9074729 should be a valid IMO number");
        let imo3 = ImoNumber::create(9785811).expect("9785811 should be a valid IMO number");

        println!("Equality:");
        println!("  9074729 == IMO9074729? {}", imo1 == imo2);
        println!("  9074729 == 9785811   ? {}", imo1 == imo3);

        println!("\nInequality:");
        println!("  9074729 != 9785811   ? {}", imo1 != imo3);
        println!("  9074729 != IMO9074729? {}", imo1 != imo2);

        println!();
    }

    {
        println!("7. ImoNumber: toString() formatting");
        println!("----------------------------------------");

        let imo = ImoNumber::create(9074729).expect("9074729 should be a valid IMO number");

        println!("String representation:");
        println!("  to_string(): {}", imo);
        println!("  Format     : Always includes \"IMO\" prefix");
        println!("  Format     : Always 7 digits after \"IMO\"");

        // Different input formats, same output
        println!("\nConsistent output:");
        println!(
            "  From int 9074729 : {}",
            ImoNumber::create(9074729).unwrap()
        );
        println!(
            "  From '9074729'   : {}",
            "9074729".parse::<ImoNumber>().unwrap()
        );
        println!(
            "  From 'IMO9074729': {}",
            "IMO9074729".parse::<ImoNumber>().unwrap()
        );

        println!();
    }

    {
        println!("8. ImoNumber: Real-world examples");
        println!("--------------------------------------");

        struct Ship {
            name: &'static str,
            imo_number: i32,
        }

        let ships = [
            Ship {
                name: "Queen Mary 2",
                imo_number: 9241061,
            },
            Ship {
                name: "Peter Faber",
                imo_number: 8027781,
            },
            Ship {
                name: "Oasis of the Seas",
                imo_number: 9383936,
            },
            Ship {
                name: "Symphony of the Seas",
                imo_number: 9744001,
            },
        ];

        println!("Famous ships:");
        for ship in ships {
            if ImoNumber::is_valid(ship.imo_number) {
                let imo = ImoNumber::create(ship.imo_number).expect("validated above");
                println!("  {:<20}: {}", ship.name, imo);
            }
        }

        println!();
    }

    {
        println!("9. ImoNumber: Error handling patterns");
        println!("-----------------------------------------");

        let inputs = ["9074729", "1234507"]; // Valid, Invalid

        for user_input in inputs {
            println!("\nTesting with '{}':", user_input);

            println!("  Pattern 1: .parse::<ImoNumber>() (returns Result on error):");
            match user_input.parse::<ImoNumber>() {
                Ok(imo) => println!("    Success: {}", imo),
                Err(e) => println!("    Error: {}", e),
            }

            println!("  Pattern 2: .parse::<ImoNumber>() with Result (propagatable):");
            match user_input.parse::<ImoNumber>() {
                Ok(imo) => println!("    Success: {}", imo),
                Err(e) => println!("    Error: {}", e),
            }

            println!("  Pattern 3: .parse().ok() for Option (no diagnostic needed):");
            match user_input.parse::<ImoNumber>().ok() {
                Some(imo) => println!("    Success: {}", imo),
                None => println!("    Invalid IMO number"),
            }
        }

        println!();
    }
}
