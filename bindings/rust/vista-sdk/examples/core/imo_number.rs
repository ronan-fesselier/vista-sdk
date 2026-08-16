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
        let imo1 = ImoNumber::create_from_str("IMO9074729")
            .expect("IMO9074729 should be a valid IMO number");
        println!("From 'IMO9074729': {}", imo1);

        // Without "IMO" prefix
        let imo2 =
            ImoNumber::create_from_str("9785811").expect("9785811 should be a valid IMO number");
        println!("From '9785811'   : {}", imo2);

        // Both create equivalent objects
        println!(
            "Are they equal? {}",
            imo1 == ImoNumber::create_from_str("IMO9074729").unwrap()
        );
        println!(
            "Are they equal? {}",
            imo2 == ImoNumber::create_from_str("IMO9785811").unwrap()
        );

        // Try invalid string (returns Err)
        println!("\nAttempting to create from invalid string ('IM9074729'):");
        match ImoNumber::create_from_str("IM9074729") {
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
        println!("4. ImoNumber::from_str(): Safe parsing without exceptions");
        println!("-------------------------------------------------------------");

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

            match ImoNumber::from_str(s) {
                Some(imo) => println!("{}", imo),
                None => println!("Invalid"),
            }
        }

        println!();
    }

    {
        println!("5. ImoNumber::from_str(): Optional usage patterns");
        println!("----------------------------------------------------");

        let input = "IMO9074729";

        // Pattern 1: Direct if with optional
        if let Some(imo) = ImoNumber::from_str(input) {
            println!("Parsed successfully: {}", imo);
        } else {
            println!("Parse failed");
        }

        // Pattern 2: is_some() check
        let result = ImoNumber::from_str("9785811");
        if result.is_some() {
            println!("Parsed successfully: {}", result.unwrap());
        }

        println!();
    }

    {
        println!("6. ImoNumber: Comparison operators");
        println!("-------------------------------------");

        let imo1 = ImoNumber::create(9074729).expect("9074729 should be a valid IMO number");
        let imo2 = ImoNumber::create_from_str("IMO9074729")
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
            ImoNumber::create_from_str("9074729").unwrap()
        );
        println!(
            "  From 'IMO9074729': {}",
            ImoNumber::create_from_str("IMO9074729").unwrap()
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

            println!("  Pattern 1: create_from_str() (returns Result on error):");
            match ImoNumber::create_from_str(user_input) {
                Ok(imo) => println!("    Success: {}", imo),
                Err(e) => println!("    Error: {}", e),
            }

            println!("  Pattern 2: from_str() with Option (no exceptions):");
            match ImoNumber::from_str(user_input) {
                Some(imo) => println!("    Success: {}", imo),
                None => println!("    Error: Invalid IMO number format"),
            }
        }

        println!();
    }
}
