use vista_sdk::core::codebook_name::CodebookName;
use vista_sdk::core::gmod_path::OwnedGmodPath;
use vista_sdk::core::local_id::OwnedLocalId;
use vista_sdk::core::local_id_builder::OwnedLocalIdBuilder;
use vista_sdk::core::local_id_mqtt::MqttLocalId;
use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;

fn main() {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(vis.latest())
        .expect("latest() should always be a valid VIS version");
    let locations = vis
        .locations(vis.latest())
        .expect("latest() should always be a valid VIS version");
    let codebooks = vis
        .codebooks(vis.latest())
        .expect("latest() should always be a valid VIS version");

    println!("=== vista-sdk LocalId Sample ===\n");

    {
        println!("1. LocalIdBuilder: Building a simple LocalId");
        println!("-----------------------------------------------");

        let primary_item = OwnedGmodPath::from_short_path("411.1/C101.31-2", gmod, locations);
        let quantity_tag = codebooks[CodebookName::Quantity].create_tag("temperature");

        if let (Some(primary_item), Some(quantity_tag)) = (primary_item, quantity_tag) {
            let local_id = OwnedLocalIdBuilder::create(vis.latest())
                .with_primary_item(&primary_item)
                .with_metadata_tag(&quantity_tag)
                .build()
                .expect("builder with primary item and metadata tag should be valid");

            println!("Built LocalId: {}", local_id);
            println!("  VIS Version : {}", local_id.version());
            println!("  Primary Item: {}", local_id.primary_item());
            println!(
                "  Quantity    : {}",
                local_id.quantity().expect("quantity tag was set").value()
            );
        }

        println!();
    }

    {
        println!("2. LocalIdBuilder: Building with multiple metadata tags");
        println!("----------------------------------------------------------");

        let primary_item = OwnedGmodPath::from_short_path("621.21/S90", gmod, locations);
        let qty_tag = codebooks[CodebookName::Quantity].create_tag("mass");
        let cnt_tag = codebooks[CodebookName::Content].create_tag("fuel.oil");
        let pos_tag = codebooks[CodebookName::Position].create_tag("inlet");

        if let (Some(primary_item), Some(qty_tag), Some(cnt_tag), Some(pos_tag)) =
            (primary_item, qty_tag, cnt_tag, pos_tag)
        {
            let local_id = OwnedLocalIdBuilder::create(vis.latest())
                .with_primary_item(&primary_item)
                .with_metadata_tag(&qty_tag)
                .with_metadata_tag(&cnt_tag)
                .with_metadata_tag(&pos_tag)
                .build()
                .expect("builder with primary item and metadata tags should be valid");

            println!("Built LocalId: {}", local_id);
            println!("  Metadata tags:");
            println!(
                "    Quantity: {}",
                local_id.quantity().expect("quantity tag was set").value()
            );
            println!(
                "    Content : {}",
                local_id.content().expect("content tag was set").value()
            );
            println!(
                "    Position: {}",
                local_id.position().expect("position tag was set").value()
            );
        }

        println!();
    }

    {
        println!("3. LocalIdBuilder: Building with secondary item");
        println!("--------------------------------------------------");

        let primary_item = OwnedGmodPath::from_short_path("621.21/S90", gmod, locations);
        let secondary_item = OwnedGmodPath::from_short_path("411.1/C101", gmod, locations);
        let qty_tag = codebooks[CodebookName::Quantity].create_tag("mass");
        let cnt_tag = codebooks[CodebookName::Content].create_tag("fuel.oil");
        let pos_tag = codebooks[CodebookName::Position].create_tag("inlet");

        if let (
            Some(primary_item),
            Some(secondary_item),
            Some(qty_tag),
            Some(cnt_tag),
            Some(pos_tag),
        ) = (primary_item, secondary_item, qty_tag, cnt_tag, pos_tag)
        {
            let local_id = OwnedLocalIdBuilder::create(vis.latest())
                .with_primary_item(&primary_item)
                .with_secondary_item(&secondary_item)
                .with_metadata_tag(&qty_tag)
                .with_metadata_tag(&cnt_tag)
                .with_metadata_tag(&pos_tag)
                .build()
                .expect("builder with primary/secondary item and metadata tags should be valid");

            println!("Built LocalId: {}", local_id);
            println!("  Primary item  : {}", local_id.primary_item());
            println!(
                "  Secondary item: {}",
                local_id.secondary_item().expect("secondary item was set")
            );
            println!("  Has secondary : {}", local_id.secondary_item().is_some());
        }

        println!();
    }

    {
        println!("4. LocalId: Parsing from string");
        println!("---------------------------------");

        let local_id_str = "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature";

        let local_id = OwnedLocalId::from_string(local_id_str);

        if let Some(local_id) = local_id {
            println!("Parsed successfully: {}", local_id);
            println!("  VIS Version : {}", local_id.version());
            println!("  Primary item: {}", local_id.primary_item());
            println!(
                "  Quantity    : {}",
                local_id.quantity().expect("quantity tag was set").value()
            );
        } else {
            println!("Parse failed");
        }

        println!();
    }

    {
        println!("5. LocalId: Parsing with error handling");
        println!("------------------------------------------");

        let test_strings = [
            "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature",
            "/dnv-v2/vis-3-4a/621.21/S90/sec/411.1/C101/meta/qty-mass/cnt-fuel.oil/pos-inlet",
            "/dnv-v2/vis-3-7a/612.21/C701.23/C633/meta/calc~accumulate",
            "",
            "/dnv-v2/INVALID/411.1/meta/qty-temperature",
            "/dnv-v2/vis-3-4a/INVALID/meta/qty-temperature",
            "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-invalid_quantity",
        ];

        for str in test_strings {
            let (local_id, errors) = OwnedLocalId::from_string_with_errors(str);

            println!(
                "Parsing: \"{}\"",
                if str.is_empty() { "(empty)" } else { str }
            );

            if let Some(local_id) = local_id {
                println!("  Success: {}", local_id);
            } else {
                println!("  Failed:");
                if errors.has_errors() {
                    for error in errors.iter() {
                        println!("    - {}", error.message);
                    }
                }
            }
            println!();
        }
    }

    {
        println!("6. LocalIdBuilder: Modifying existing LocalIds");
        println!("------------------------------------------------");

        let original =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature");

        if let Some(original) = original {
            println!("Original: {}", original);

            // Add content tag
            let cnt_tag = codebooks[CodebookName::Content].create_tag("water");

            // Remove quantity, add position
            let pos_tag = codebooks[CodebookName::Position].create_tag("outlet");

            if let Some(cnt_tag) = cnt_tag {
                let modified = original
                    .builder()
                    .with_metadata_tag(&cnt_tag)
                    .build()
                    .expect("builder with existing state plus a content tag should be valid");
                println!("Modified (added content): {}", modified);
            }

            if let Some(pos_tag) = pos_tag {
                let modified2 = original
                    .builder()
                    .without_metadata_tag(CodebookName::Quantity)
                    .with_metadata_tag(&pos_tag)
                    .build()
                    .expect(
                        "builder with existing state minus quantity plus position should be valid",
                    );
                println!("Modified (removed qty, added pos): {}", modified2);
            }
        }

        println!();
    }

    {
        println!("7. LocalIdBuilder: Verbose mode");
        println!("----------------------------------");

        let primary_item = OwnedGmodPath::from_short_path("411.1/C101.31-2", gmod, locations);
        let qty_tag = codebooks[CodebookName::Quantity].create_tag("temperature");

        if let (Some(primary_item), Some(qty_tag)) = (primary_item, qty_tag) {
            let normal_local_id = OwnedLocalIdBuilder::create(vis.latest())
                .with_primary_item(&primary_item)
                .with_metadata_tag(&qty_tag)
                .build()
                .expect("builder with primary item and metadata tag should be valid");

            println!("Normal mode : {}", normal_local_id);

            let verbose_local_id = OwnedLocalIdBuilder::create(vis.latest())
                .with_primary_item(&primary_item)
                .with_metadata_tag(&qty_tag)
                .with_verbose_mode(true)
                .build()
                .expect("builder with primary item and metadata tag should be valid");

            println!("Verbose mode: {}", verbose_local_id);
            println!("  Includes human-readable common names for better understanding");
        }

        println!();
    }

    {
        println!("8. LocalId: Accessing components");
        println!("----------------------------------");

        let local_id = OwnedLocalId::from_string(
            "/dnv-v2/vis-3-4a/621.21/S90/sec/411.1/C101/meta/qty-mass/cnt-fuel.oil/pos-inlet",
        );

        if let Some(local_id) = local_id {
            println!("LocalId: {}\n", local_id);

            println!("Components:");
            println!("  VIS Version   : {}", local_id.version());
            println!("  Primary item  : {}", local_id.primary_item());

            if let Some(secondary_item) = local_id.secondary_item() {
                println!("  Secondary item: {}", secondary_item);
            }

            println!("\nMetadata tags:");
            if let Some(quantity) = local_id.quantity() {
                println!("  Quantity   : {}", quantity.value());
            }
            if let Some(content) = local_id.content() {
                println!("  Content    : {}", content.value());
            }
            if let Some(calculation) = local_id.calculation() {
                println!("  Calculation: {}", calculation.value());
            }
            if let Some(state) = local_id.state() {
                println!("  State      : {}", state.value());
            }
            if let Some(command) = local_id.command() {
                println!("  Command    : {}", command.value());
            }
            if let Some(r#type) = local_id.r#type() {
                println!("  Type       : {}", r#type.value());
            }
            if let Some(position) = local_id.position() {
                println!("  Position   : {}", position.value());
            }
            if let Some(detail) = local_id.detail() {
                println!("  Detail     : {}", detail.value());
            }

            let tags = local_id.metadata_tags();
            println!("\nAll metadata tags ({}):", tags.len());
            for tag in tags {
                println!(
                    "  {}-{}{}",
                    vista_sdk::core::codebook_name::codebook_names::to_prefix(tag.name()),
                    tag.value(),
                    if tag.is_custom() { " (custom)" } else { "" }
                );
            }
        }

        println!();
    }

    {
        println!("9. LocalId: Equality and comparison");
        println!("-------------------------------------");

        let local_id1 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature");
        let local_id2 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature");
        let local_id3 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-pressure");

        if let (Some(local_id1), Some(local_id2), Some(local_id3)) =
            (local_id1, local_id2, local_id3)
        {
            println!("LocalId1: {}", local_id1);
            println!("LocalId2: {}", local_id2);
            println!("LocalId3: {}\n", local_id3);

            println!("LocalId1 == LocalId2? {}", local_id1 == local_id2);
            println!("LocalId1 == LocalId3? {}", local_id1 == local_id3);
            println!("LocalId1 != LocalId3? {}", local_id1 != local_id3);
        }

        println!();
    }

    {
        println!("10. LocalIdBuilder: Custom metadata tags");
        println!("-------------------------------------------");

        let primary_item = OwnedGmodPath::from_short_path("411.1/C101.31-2", gmod, locations);

        let custom_qty_tag = codebooks[CodebookName::Quantity].create_tag("my_custom_measurement");

        if let (Some(primary_item), Some(custom_qty_tag)) = (primary_item, custom_qty_tag) {
            let local_id = OwnedLocalIdBuilder::create(vis.latest())
                .with_primary_item(&primary_item)
                .with_metadata_tag(&custom_qty_tag)
                .build()
                .expect("builder with primary item and custom metadata tag should be valid");

            println!("LocalId with custom tag: {}", local_id);
            println!("  Has custom tag : {}", local_id.has_custom_tag());
            println!(
                "  Custom quantity: {}",
                local_id.quantity().expect("quantity tag was set").value()
            );
            println!(
                "  Is custom      : {}",
                local_id
                    .quantity()
                    .expect("quantity tag was set")
                    .is_custom()
            );
        }

        println!();
    }

    {
        println!("11. LocalIdBuilder: Validation");
        println!("--------------------------------");

        let primary_item = OwnedGmodPath::from_short_path("411.1/C101.31-2", gmod, locations);
        let qty_tag = codebooks[CodebookName::Quantity].create_tag("temperature");

        if let (Some(primary_item), Some(qty_tag)) = (primary_item, qty_tag) {
            let valid_builder = OwnedLocalIdBuilder::create(vis.latest())
                .with_primary_item(&primary_item)
                .with_metadata_tag(&qty_tag);

            println!("Valid builder:");
            println!("  Is valid: {}", valid_builder.is_valid());
            println!("  Is empty: {}", valid_builder.is_empty());

            // Empty builder
            let empty_builder = OwnedLocalIdBuilder::create(vis.latest());
            println!("\nEmpty builder:");
            println!("  Is valid: {}", empty_builder.is_valid());
            println!("  Is empty: {}", empty_builder.is_empty());

            // Builder missing metadata
            let incomplete_builder =
                OwnedLocalIdBuilder::create(vis.latest()).with_primary_item(&primary_item);
            println!("\nBuilder with primary item but no metadata:");
            println!("  Is valid: {}", incomplete_builder.is_valid());
            println!("  Is empty: {}", incomplete_builder.is_empty());

            // Try to build invalid
            println!("\nAttempting to build invalid LocalId:");
            match incomplete_builder.build() {
                Ok(_) => println!("  ERROR: Invalid build succeeded (unexpected)"),
                Err(error) => println!("  Correctly rejected: {}", error),
            }
        }

        println!();
    }

    {
        println!("12. mqtt::LocalId: MQTT-compatible formatting");
        println!("------------------------------------------------");

        let simple = [
            (
                "qty only",
                "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature",
            ),
            (
                "qty + cnt + pos",
                "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
            ),
            (
                "with secondary item",
                "/dnv-v2/vis-3-4a/621.21/S90/sec/411.1/C101/meta/qty-mass/cnt-fuel.oil/pos-inlet",
            ),
        ];

        for (label, local_id_str) in simple {
            let Some(local_id) = OwnedLocalId::from_string(local_id_str) else {
                continue;
            };

            let mqtt_local_id = MqttLocalId::create(local_id.builder())
                .expect("valid builder should produce an MqttLocalId");

            println!("{}:", label);
            println!("  Standard : {}", local_id);
            println!("  MQTT     : {}\n", mqtt_local_id);
        }

        // All 8 slots filled, including custom tags
        {
            let primary_item = OwnedGmodPath::from_short_path("411.1/C101.31-2", gmod, locations);
            let secondary_item = OwnedGmodPath::from_short_path("411.1/C101.31-5", gmod, locations);
            let qty_tag = codebooks[CodebookName::Quantity].create_tag("temperature");
            let cnt_tag = codebooks[CodebookName::Content].create_tag("exhaust.gas");
            let calc_tag = codebooks[CodebookName::Calculation].create_tag("average");
            let state_tag = codebooks[CodebookName::State].create_tag("high");
            let cmd_tag = codebooks[CodebookName::Command].create_tag("start");
            let type_tag = codebooks[CodebookName::Type].create_tag("instantaneous");
            let pos_tag = codebooks[CodebookName::Position].create_tag("inlet");
            let detail_tag = codebooks[CodebookName::Detail].create_tag("my_sensor_42");

            if let (
                Some(primary_item),
                Some(secondary_item),
                Some(qty_tag),
                Some(cnt_tag),
                Some(calc_tag),
                Some(state_tag),
                Some(cmd_tag),
                Some(type_tag),
                Some(pos_tag),
                Some(detail_tag),
            ) = (
                primary_item,
                secondary_item,
                qty_tag,
                cnt_tag,
                calc_tag,
                state_tag,
                cmd_tag,
                type_tag,
                pos_tag,
                detail_tag,
            ) {
                let builder = OwnedLocalIdBuilder::create(vis.latest())
                    .with_primary_item(&primary_item)
                    .with_secondary_item(&secondary_item)
                    .with_metadata_tag(&qty_tag)
                    .with_metadata_tag(&cnt_tag)
                    .with_metadata_tag(&calc_tag)
                    .with_metadata_tag(&state_tag)
                    .with_metadata_tag(&cmd_tag)
                    .with_metadata_tag(&type_tag)
                    .with_metadata_tag(&pos_tag)
                    .with_metadata_tag(&detail_tag);

                let standard_local_id = builder
                    .build()
                    .expect("builder with all slots filled should be valid");
                let mqtt_local_id = MqttLocalId::create(&builder)
                    .expect("valid builder should produce an MqttLocalId");

                println!("all 8 slots + secondary + free-form detail:");
                println!("  Standard : {}", standard_local_id);
                println!("  MQTT     : {}", mqtt_local_id);
                println!("  Slots    : qty/cnt/calc/state/cmd/type/pos/detail");
                let detail = standard_local_id.detail().expect("detail tag was set");
                println!(
                    "  Detail   : '{}' (isCustom: {})\n",
                    detail.value(),
                    detail.is_custom()
                );
            }
        }

        // Reading components
        {
            let primary_item = OwnedGmodPath::from_short_path("411.1/C101.31-2", gmod, locations);
            let secondary_item = OwnedGmodPath::from_short_path("411.1/C101.31-5", gmod, locations);
            let qty_tag = codebooks[CodebookName::Quantity].create_tag("temperature");
            let cnt_tag = codebooks[CodebookName::Content].create_tag("exhaust.gas");

            if let (Some(primary_item), Some(secondary_item), Some(qty_tag), Some(cnt_tag)) =
                (primary_item, secondary_item, qty_tag, cnt_tag)
            {
                let builder = OwnedLocalIdBuilder::create(VisVersion::V3_4a)
                    .with_primary_item(&primary_item)
                    .with_secondary_item(&secondary_item)
                    .with_metadata_tag(&qty_tag)
                    .with_metadata_tag(&cnt_tag);

                let mqtt_local_id = MqttLocalId::create(&builder)
                    .expect("valid builder should produce an MqttLocalId");

                println!("reading components:");
                println!("  Version       : {}", mqtt_local_id.version());
                println!("  Primary item  : {}", mqtt_local_id.primary_item());
                println!(
                    "  Secondary item: {}",
                    mqtt_local_id
                        .secondary_item()
                        .expect("secondary item was set")
                );
                println!(
                    "  Quantity      : {}",
                    mqtt_local_id.quantity().expect("quantity was set")
                );
                println!(
                    "  Content       : {}",
                    mqtt_local_id.content().expect("content was set")
                );
                println!(
                    "  Calculation   : {}",
                    mqtt_local_id
                        .calculation()
                        .map(|tag| tag.to_string())
                        .unwrap_or_else(|| "(none)".to_string())
                );
                println!();
            }
        }

        // Builder-level state not reflected in the MQTT format
        {
            let primary_item =
                OwnedGmodPath::from_short_path("411.1/C101.63/S206", gmod, locations);
            let qty_tag = codebooks[CodebookName::Quantity].create_tag("temperature");

            if let (Some(primary_item), Some(qty_tag)) = (primary_item, qty_tag) {
                let builder = OwnedLocalIdBuilder::create(VisVersion::V3_4a)
                    .with_verbose_mode(true)
                    .with_primary_item(&primary_item)
                    .with_metadata_tag(&qty_tag);

                let mqtt_local_id = MqttLocalId::create(&builder)
                    .expect("valid builder should produce an MqttLocalId");

                println!("builder-level state (accessible via .builder()):");
                println!("  isVerboseMode, hasCustomTag and metadataTags are not duplicated on mqtt::LocalId.");
                println!("  isVerboseMode() has no effect on the MQTT format (unlike sdk::LocalId::toString()).");
                println!(
                    "  builder().is_verbose_mode(): {}",
                    mqtt_local_id.builder().is_verbose_mode()
                );
                println!(
                    "  builder().has_custom_tag(): {}",
                    mqtt_local_id.builder().has_custom_tag()
                );
                println!("  builder().metadata_tags() count is not directly exposed; use quantity()/content()/etc.");
                println!(
                    "  MQTT (no '~' despite verbose_mode=true): {}",
                    mqtt_local_id
                );
                println!();
            }
        }

        // Equality
        {
            let primary_item = OwnedGmodPath::from_short_path("411.1/C101.31-2", gmod, locations);
            let qty_tag = codebooks[CodebookName::Quantity].create_tag("temperature");
            let cnt_tag = codebooks[CodebookName::Content].create_tag("exhaust.gas");

            if let (Some(primary_item), Some(qty_tag), Some(cnt_tag)) =
                (primary_item, qty_tag, cnt_tag)
            {
                let builder = OwnedLocalIdBuilder::create(VisVersion::V3_4a)
                    .with_primary_item(&primary_item)
                    .with_metadata_tag(&qty_tag);

                let a = MqttLocalId::create(&builder)
                    .expect("valid builder should produce an MqttLocalId");
                let b = MqttLocalId::create(&builder)
                    .expect("valid builder should produce an MqttLocalId");
                let with_cnt = builder.with_metadata_tag(&cnt_tag);
                let c = MqttLocalId::create(&with_cnt)
                    .expect("valid builder should produce an MqttLocalId");

                println!("equality:");
                println!("  a == b (same builder): {}", a == b);
                println!("  a == c (extra tag)   : {}", a == c);
                println!();
            }
        }

        println!("MQTT format differences vs standard:");
        println!("  - No leading '/'");
        println!("  - Underscores instead of slashes in paths");
        println!("  - No 'meta/' section");
        println!("  - '_' placeholder for absent metadata slots");
        println!("  - 8 fixed slots: qty/cnt/calc/state/cmd/type/pos/detail");

        println!();
    }
}
