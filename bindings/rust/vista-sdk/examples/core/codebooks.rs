use vista_sdk::core::codebook::PositionValidationResult;
use vista_sdk::core::codebook_name::{codebook_names, CodebookName};
use vista_sdk::core::vis::Vis;

fn main() {
    println!("=== vista-sdk Codebooks Sample ===\n");

    {
        println!("1. VIS Singleton: Version management");
        println!("-------------------------------------------------------");

        let vis = Vis::instance();
        let all_versions = vis.versions();

        println!("Latest VIS version: {}", vis.latest().as_str());
        println!("Available versions ({}):", all_versions.len());
        for version in &all_versions {
            println!("  - {}", version.as_str());
        }
        println!();
    }

    {
        println!("2. Codebooks: Accessing codebooks for a version");
        println!("-------------------------------------------------------");

        let vis = Vis::instance();
        let codebooks = vis
            .codebooks(vis.latest())
            .expect("latest() should always be a valid VIS version");

        println!("Codebooks for version: {}", codebooks.version().as_str());

        let quantity = &codebooks[CodebookName::Quantity];
        let position = &codebooks[CodebookName::Position];
        let state = &codebooks[CodebookName::State];

        println!(
            "Quantity codebook has {:3} standard values",
            quantity.standard_values_count()
        );
        println!(
            "Position codebook has {:3} standard values",
            position.standard_values_count()
        );
        println!(
            "State    codebook has {:3} standard values",
            state.standard_values_count()
        );
        println!();
    }

    {
        println!("3. Codebook: Checking standard values and groups");
        println!("-------------------------------------------------------");

        let vis = Vis::instance();
        let codebooks = vis
            .codebooks(vis.latest())
            .expect("latest() should always be a valid VIS version");
        let quantity = &codebooks[CodebookName::Quantity];

        println!(
            "Has 'temperature'? {}",
            quantity.has_standard_value("temperature")
        );
        println!(
            "Has 'pressure'   ? {}",
            quantity.has_standard_value("pressure")
        );
        println!(
            "Has 'invalid_qty'? {}",
            quantity.has_standard_value("invalid_qty")
        );

        println!("\nGroups in Quantity codebook: {}", quantity.groups_count());
        for i in 0..quantity.groups_count() {
            if let Some(group) = quantity.group_at(i) {
                println!("  - {}", group);
            }
        }
        println!();
    }

    {
        println!("4. Position Codebook: Special numeric value handling");
        println!("-------------------------------------------------------");

        let vis = Vis::instance();
        let codebooks = vis
            .codebooks(vis.latest())
            .expect("latest() should always be a valid VIS version");
        let position = &codebooks[CodebookName::Position];

        println!("Has '1'        ? {}", position.has_standard_value("1"));
        println!("Has '03'       ? {}", position.has_standard_value("03"));
        println!("Has '42'       ? {}", position.has_standard_value("42"));
        println!("Has '1234'     ? {}", position.has_standard_value("1234"));
        println!("Has 'centre'   ? {}", position.has_standard_value("centre"));
        println!("Has 'port'     ? {}", position.has_standard_value("port"));
        println!(
            "Has 'starboard'? {}",
            position.has_standard_value("starboard")
        );
        println!(
            "Has 'invalid'  ? {}",
            position.has_standard_value("invalid")
        );
        println!();
    }

    {
        println!("5. MetadataTag: Creating tags from standard values");
        println!("-------------------------------------------------------");

        let vis = Vis::instance();
        let codebooks = vis
            .codebooks(vis.latest())
            .expect("latest() should always be a valid VIS version");
        let quantity = &codebooks[CodebookName::Quantity];

        if let Some(temp_tag) = quantity.create_tag("temperature") {
            println!("Temperature tag created:");
            println!(
                "  Name:        {}",
                codebook_names::to_prefix(temp_tag.name())
            );
            println!("  Value:       {}", temp_tag.value());
            println!("  Prefix:      {}", temp_tag.prefix());
            println!("  Is Custom:   {}", temp_tag.is_custom());
            println!("  String repr: {}", temp_tag);
        }
        println!();
    }

    {
        println!("6. MetadataTag: Creating custom tags");
        println!("-------------------------------------------------------");

        let vis = Vis::instance();
        let codebooks = vis
            .codebooks(vis.latest())
            .expect("latest() should always be a valid VIS version");
        let quantity = &codebooks[CodebookName::Quantity];

        if let Some(custom_tag) = quantity.create_tag("custom_measurement") {
            println!("Custom tag created:");
            println!(
                "  Name:        {}",
                codebook_names::to_prefix(custom_tag.name())
            );
            println!("  Value:       {}", custom_tag.value());
            println!("  Prefix:      {}", custom_tag.prefix());
            println!("  Is Custom:   {}", custom_tag.is_custom());
            println!("  String repr: {}", custom_tag);
        }
        println!();
    }

    {
        println!("7. Position Validation");
        println!("-------------------------------------------------------");

        let vis = Vis::instance();
        let codebooks = vis
            .codebooks(vis.latest())
            .expect("latest() should always be a valid VIS version");
        let position = &codebooks[CodebookName::Position];

        let test_positions = [
            "centre",
            "port",
            "1",
            "42",
            "centre-starboard",
            "port-1",
            "centre-starboard-2",
            "invalid position!",
            "starboard-centre",
        ];

        println!("Position validation results:");
        for pos in &test_positions {
            let result = position.validate_position(pos);
            let label = match result {
                PositionValidationResult::Valid => "Valid",
                PositionValidationResult::Custom => "Custom",
                PositionValidationResult::Invalid => "Invalid",
                PositionValidationResult::InvalidOrder => "Invalid (wrong order)",
                PositionValidationResult::InvalidGrouping => "Invalid (duplicate groups)",
            };
            println!("  '{:<18}': {}", pos, label);
        }
        println!();
    }

    {
        println!("8. CodebookName: String conversions");
        println!("-------------------------------------------------------");

        println!("Enum to prefix conversions:");
        println!(
            "  Quantity -> '{}'",
            codebook_names::to_prefix(CodebookName::Quantity)
        );
        println!(
            "  Position -> '{}'",
            codebook_names::to_prefix(CodebookName::Position)
        );
        println!(
            "  State    -> '{}'",
            codebook_names::to_prefix(CodebookName::State)
        );
        println!(
            "  Command  -> '{}'",
            codebook_names::to_prefix(CodebookName::Command)
        );
        println!(
            "  Detail   -> '{}'",
            codebook_names::to_prefix(CodebookName::Detail)
        );

        println!("Prefix to enum conversions:");
        if let Some(name) = codebook_names::from_prefix("qty") {
            println!("  'qty' -> {}", name);
        }
        if let Some(name) = codebook_names::from_prefix("pos") {
            println!("  'pos' -> {}", name);
        }
        println!();
    }

    {
        println!("9. Invalid operations");
        println!("-------------------------------------------------------");

        let vis = Vis::instance();
        let codebooks = vis
            .codebooks(vis.latest())
            .expect("latest() should always be a valid VIS version");
        let quantity = &codebooks[CodebookName::Quantity];

        println!(
            "Empty string tag created ? {}",
            quantity.create_tag("").is_some()
        );
        println!(
            "Whitespace tag created   ? {}",
            quantity.create_tag("   ").is_some()
        );

        match codebook_names::from_prefix("invalid_prefix") {
            None => println!("Invalid prefix rejected  : not a valid prefix"),
            Some(_) => println!("Invalid prefix accepted  : (unexpected)"),
        }
        println!();
    }

    {
        println!("10. Building a complete metadata path");
        println!("-------------------------------------------------------");

        let vis = Vis::instance();
        let codebooks = vis
            .codebooks(vis.latest())
            .expect("latest() should always be a valid VIS version");

        let qty_tag = codebooks[CodebookName::Quantity].create_tag("temperature");
        let cnt_tag = codebooks[CodebookName::Content].create_tag("oil");
        let pos_tag = codebooks[CodebookName::Position].create_tag("1");
        let state_tag = codebooks[CodebookName::State].create_tag("running");

        if let (Some(qty), Some(cnt), Some(pos), Some(state)) =
            (qty_tag, cnt_tag, pos_tag, state_tag)
        {
            println!("Building metadata path:");
            println!("  After Quantity: {}", qty);
            println!("  After Content : {}", cnt);
            println!("  After Position: {}", pos);
            println!("  After State   : {}", state);
        }
        println!();
    }
}
