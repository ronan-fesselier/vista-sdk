use vista_sdk::core::location_builder::LocationBuilder;
use vista_sdk::core::location_group::LocationGroup;
use vista_sdk::core::vis::Vis;

fn main() {
    println!("=== vista-sdk Locations Sample ===\n");

    {
        println!("1. Locations: Accessing location data for a VIS version");
        println!("----------------------------------------------------------");

        let vis = Vis::instance();
        let version = vis.latest();
        let locations = vis
            .locations(version)
            .expect("latest() should always be a valid VIS version");

        println!("Locations for version   : {}", locations.version().as_str());
        println!(
            "Total relative locations: {}",
            locations.relative_location_count()
        );
        println!();
    }

    {
        println!("2. Location Parsing: Valid location strings");
        println!("------------------------------------------------");

        let vis = Vis::instance();
        let locations = vis
            .locations(vis.latest())
            .expect("latest() should always be a valid VIS version");

        let valid_locations = ["1", "5", "42", "1P", "2CF", "3SU", "10FI", "CFOU"];

        for loc_str in &valid_locations {
            let padding = if loc_str.len() < 7 {
                7 - loc_str.len()
            } else {
                0
            };
            print!("  '{}'{}-> ", loc_str, " ".repeat(padding));
            match locations.parse(loc_str) {
                Some(location) => println!("Valid: '{}'", location.value()),
                None => println!("Invalid (unexpected)"),
            }
        }
        println!();
    }

    {
        println!("3. Location Parsing: Invalid location strings with errors");
        println!("--------------------------------------------------------------");

        let vis = Vis::instance();
        let locations = vis
            .locations(vis.latest())
            .expect("latest() should always be a valid VIS version");

        let invalid_locations = [
            "",          // Empty
            "   ",       // Whitespace only
            "X",         // Invalid character
            "1X",        // Invalid character after number
            "ZPS",       // Invalid first character
            "PC",        // Multiple from same group (Side)
            "1PS",       // Multiple from same group (Side)
            "1UL",       // Multiple from same group (Vertical)
            "2IO",       // Multiple from same group (Transverse)
            "SP1",       // Number not at start
            "1SPA",      // Not alphabetically sorted
            "10PSFI",    // Multiple from Side (P+S) and multiple groups
            "ACFIMOPSU", // Multiple from Side (P+S) and Transverse (I+O)
        ];

        for loc_str in &invalid_locations {
            let (location, errors) = locations.parse_with_errors(loc_str);

            println!("  \"{}\" -> Invalid", loc_str);
            if location.is_none() && errors.has_errors() {
                println!("    Errors:");
                for error in errors.iter() {
                    println!("      - {}", error.message);
                }
            }
        }
        println!();
    }

    {
        println!("4. RelativeLocations: Exploring location metadata");
        println!("------------------------------------------------------");

        let vis = Vis::instance();
        let locations = vis
            .locations(vis.latest())
            .expect("latest() should always be a valid VIS version");

        println!("Location codes and their meanings:");
        for rel_loc in locations.relative_locations() {
            print!("  {} - {}", rel_loc.code(), rel_loc.name());
            if let Some(definition) = rel_loc.definition() {
                print!(" ({})", definition);
            }
            println!();
        }
        println!();
    }

    {
        println!("5. Location Groups: Accessing grouped location codes");
        println!("---------------------------------------------------------");

        let vis = Vis::instance();
        let locations = vis
            .locations(vis.latest())
            .expect("latest() should always be a valid VIS version");

        let groups = [
            (LocationGroup::Side, "Side:"),
            (LocationGroup::Vertical, "Vertical:"),
            (LocationGroup::Transverse, "Transverse:"),
            (LocationGroup::Longitudinal, "Longitudinal:"),
        ];

        println!("Location groups:");
        for (group, label) in groups {
            let count = locations.group_count(group);
            if count == 0 {
                continue;
            }
            print!("  {:<14}", label);
            for (i, rel_loc) in locations.group(group).enumerate() {
                if i > 0 {
                    print!(", ");
                }
                print!("{}", rel_loc.code());
            }
            println!();
        }
        println!();
    }

    {
        println!("6. LocationBuilder: Creating locations with fluent API");
        println!("------------------------------------------------------------");

        let vis = Vis::instance();
        let locations = vis
            .locations(vis.latest())
            .expect("latest() should always be a valid VIS version");

        let builder = LocationBuilder::create(locations)
            .with_number(1)
            .expect("1 should be a valid number");

        println!("Building location step by step:");
        println!("  After with_number(1)        : {}", builder);

        let builder = builder.with_side('P').expect("P should be a valid side");
        println!("  After with_side('P')        : {}", builder);

        let builder = builder
            .with_vertical('U')
            .expect("U should be a valid vertical");
        println!("  After with_vertical('U')    : {}", builder);

        let builder = builder
            .with_transverse('I')
            .expect("I should be a valid transverse");
        println!("  After with_transverse('I')  : {}", builder);

        let builder = builder
            .with_longitudinal('F')
            .expect("F should be a valid longitudinal");
        println!("  After with_longitudinal('F'): {}", builder);

        let location = builder.build();
        println!("  Final location              : {}\n", location.value());
    }

    {
        println!("7. LocationBuilder: Method chaining");
        println!("----------------------------------------");

        let vis = Vis::instance();
        let locations = vis
            .locations(vis.latest())
            .expect("latest() should always be a valid VIS version");

        let location = LocationBuilder::create(locations)
            .with_number(5)
            .expect("5 should be a valid number")
            .with_side('S')
            .expect("S should be a valid side")
            .with_vertical('M')
            .expect("M should be a valid vertical")
            .with_longitudinal('A')
            .expect("A should be a valid longitudinal")
            .build();

        println!("Location built with chaining: {}\n", location.value());
    }

    {
        println!("8. LocationBuilder: Parsing existing locations");
        println!("---------------------------------------------------");

        let vis = Vis::instance();
        let locations = vis
            .locations(vis.latest())
            .expect("latest() should always be a valid VIS version");

        let existing_loc = locations
            .parse("2CF")
            .expect("2CF should be a valid location");

        let builder = LocationBuilder::create(locations).with_location(&existing_loc);

        println!("Original location: {}", existing_loc.value());
        println!("Builder state after parsing:");
        println!(
            "  Number      :{}",
            builder
                .number()
                .map_or("none".to_string(), |n| n.to_string())
        );
        println!(
            "  Side        :{}",
            builder.side().map_or("none".to_string(), |c| c.to_string())
        );
        println!(
            "  Vertical    :{}",
            builder
                .vertical()
                .map_or("none".to_string(), |c| c.to_string())
        );
        println!(
            "  Transverse  :{}",
            builder
                .transverse()
                .map_or("none".to_string(), |c| c.to_string())
        );
        println!(
            "  Longitudinal:{}\n",
            builder
                .longitudinal()
                .map_or("none".to_string(), |c| c.to_string())
        );
    }

    {
        println!("9. LocationBuilder: Modifying locations");
        println!("--------------------------------------------");

        let vis = Vis::instance();
        let locations = vis
            .locations(vis.latest())
            .expect("latest() should always be a valid VIS version");

        let original = locations
            .parse("1P")
            .expect("1P should be a valid location");
        println!("Original: {}", original.value());

        let modified = LocationBuilder::create(locations)
            .with_location(&original)
            .without_side() // Remove side
            .with_vertical('U') // Add vertical
            .expect("U should be a valid vertical")
            .build();
        println!(
            "Modified (removed side, added vertical)   : {}",
            modified.value()
        );

        let variation = LocationBuilder::create(locations)
            .with_location(&original)
            .with_number(5) // Change number
            .expect("5 should be a valid number")
            .with_vertical('L') // Add vertical
            .expect("L should be a valid vertical")
            .build();
        println!(
            "Variation (changed number, added vertical): {}\n",
            variation.value()
        );
    }

    {
        println!("10. LocationBuilder: Using with_code() and with_number() methods");
        println!("-----------------------------------------------------------------------");

        let vis = Vis::instance();
        let locations = vis
            .locations(vis.latest())
            .expect("latest() should always be a valid VIS version");

        let builder = LocationBuilder::create(locations)
            .with_number(3) // int -> set number component
            .expect("3 should be a valid number")
            .with_code('C') // char -> automatically routes to with_side()
            .expect("C should route to a group")
            .with_code('M') // char -> automatically routes to with_vertical()
            .expect("M should route to a group")
            .with_code('O') // char -> automatically routes to with_transverse()
            .expect("O should route to a group")
            .with_code('F') // char -> automatically routes to with_longitudinal()
            .expect("F should route to a group");

        let location = builder.build();
        println!("Location built with with_code(): {}", location.value());
        println!("Components automatically routed to correct groups\n");
    }

    {
        println!("11. LocationBuilder: Removing components");
        println!("---------------------------------------------");

        let vis = Vis::instance();
        let locations = vis
            .locations(vis.latest())
            .expect("latest() should always be a valid VIS version");

        let builder = LocationBuilder::create(locations)
            .with_number(1)
            .expect("1 should be a valid number")
            .with_side('P')
            .expect("P should be a valid side")
            .with_vertical('U')
            .expect("U should be a valid vertical")
            .with_transverse('I')
            .expect("I should be a valid transverse")
            .with_longitudinal('F')
            .expect("F should be a valid longitudinal");

        println!("Full location                : {}", builder);

        let builder = builder.without_number();
        println!("After without_number()       : {}", builder);

        let builder = builder.without_transverse();
        println!("After without_transverse()   : {}", builder);

        let builder = builder.without_value(LocationGroup::Vertical);
        println!("After without_value(Vertical): {}\n", builder);
    }

    {
        println!("12. LocationBuilder: Validation errors");
        println!("--------------------------------------------");

        let vis = Vis::instance();
        let locations = vis
            .locations(vis.latest())
            .expect("latest() should always be a valid VIS version");

        println!("Testing invalid operations:");

        // Invalid number (< 1)
        let builder = LocationBuilder::create(locations);
        match builder.with_number(0) {
            Ok(_) => println!("  with_number(0): Accepted (unexpected)"),
            Err(e) => println!("  with_number(0): Rejected - {}", e),
        }

        // Invalid character for Side
        let builder = LocationBuilder::create(locations);
        match builder.with_side('X') {
            Ok(_) => println!("  with_side('X'): Accepted (unexpected)"),
            Err(e) => println!("  with_side('X'): Rejected - {}", e),
        }

        // Invalid character for with_code()
        let builder = LocationBuilder::create(locations);
        match builder.with_code('Z') {
            Ok(_) => println!("  with_code('Z'): Accepted (unexpected)"),
            Err(e) => println!("  with_code('Z'): Rejected - {}", e),
        }

        println!();
    }

    {
        println!("13. Complete workflow: Parse, modify, validate");
        println!("----------------------------------------------------");

        let vis = Vis::instance();
        let locations = vis
            .locations(vis.latest())
            .expect("latest() should always be a valid VIS version");

        let user_input = "3SU";
        let parsed_location = locations.parse(user_input);

        match parsed_location {
            Some(parsed) => {
                println!("User input: \"{}\" -> Valid", user_input);

                let modified = LocationBuilder::create(locations)
                    .with_location(&parsed)
                    .with_number(10)
                    .expect("10 should be a valid number")
                    .with_vertical('U')
                    .expect("U should be a valid vertical")
                    .with_longitudinal('F')
                    .expect("F should be a valid longitudinal")
                    .build();

                println!("Modified location: {}", modified.value());

                if let Some(revalidated) = locations.parse(modified.value()) {
                    println!("Modified location is valid: {}", revalidated.value());
                }
            }
            None => println!("User input: \"{}\" -> Invalid", user_input),
        }

        println!();
    }
}
