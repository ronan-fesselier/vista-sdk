use vista_sdk::core::codebook_name::CodebookName;
use vista_sdk::core::gmod_path::OwnedGmodPath;
use vista_sdk::core::imo_number::ImoNumber;
use vista_sdk::core::local_id_builder::OwnedLocalIdBuilder;
use vista_sdk::core::universal_id::UniversalId;
use vista_sdk::core::universal_id_builder::OwnedUniversalIdBuilder;
use vista_sdk::core::vis::Vis;

fn main() {
    println!("=== vista-sdk UniversalId Sample ===\n");

    {
        println!("1. UniversalIdBuilder: Building a simple UniversalId");
        println!("------------------------------------------------------");

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

        let imo_number = ImoNumber::create(1234567).expect("1234567 should be a valid IMO number");

        let primary_item = OwnedGmodPath::from_short_path("411.1/C101.31-2", gmod, locations);
        let qty_tag = codebooks[CodebookName::Quantity].create_tag("temperature");

        if let (Some(primary_item), Some(qty_tag)) = (primary_item, qty_tag) {
            let local_id_builder = OwnedLocalIdBuilder::create(vis.latest())
                .with_primary_item(&primary_item)
                .with_metadata_tag(&qty_tag);

            let universal_id = OwnedUniversalIdBuilder::create(vis.latest())
                .with_imo_number(&imo_number)
                .with_local_id(&local_id_builder)
                .build()
                .expect("builder with IMO number and LocalIdBuilder should be valid");

            println!("Built UniversalId: {}", universal_id);
            println!("  IMO Number: {}", universal_id.imo_number());
            println!("  LocalId   : {}", universal_id.local_id());
        }

        println!();
    }

    {
        println!("2. UniversalIdBuilder: Building with full LocalId");
        println!("---------------------------------------------------");

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
            let local_id_builder = OwnedLocalIdBuilder::create(vis.latest())
                .with_primary_item(&primary_item)
                .with_secondary_item(&secondary_item)
                .with_metadata_tag(&qty_tag)
                .with_metadata_tag(&cnt_tag)
                .with_metadata_tag(&pos_tag);

            let imo_number = ImoNumber::create_from_str("IMO9074729")
                .expect("IMO9074729 should be a valid IMO number");
            let universal_id = OwnedUniversalIdBuilder::create(vis.latest())
                .with_imo_number(&imo_number)
                .with_local_id(&local_id_builder)
                .build()
                .expect("builder with IMO number and LocalIdBuilder should be valid");

            println!("Built UniversalId: {}", universal_id);
            println!("  Format       : {{naming-entity}}/{{imo-number}}{{local-id}}");
            println!("  Naming entity: {}", UniversalId::naming_entity());
            println!("  IMO Number   : {}", universal_id.imo_number());
        }

        println!();
    }

    {
        println!("3. UniversalId: Parsing from string");
        println!("-------------------------------------");

        let universal_id_str =
            "data.dnv.com/IMO1234567/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature";

        let universal_id = OwnedUniversalIdBuilder::from_string(universal_id_str);

        if let Some(universal_id) = universal_id {
            println!("Parsed successfully: {}", universal_id);
            println!("  IMO Number : {}", universal_id.imo_number());
            println!("  LocalId    : {}", universal_id.local_id());
            println!("  VIS Version: {}", universal_id.local_id().version());
        } else {
            println!("Parse failed");
        }

        println!();
    }

    {
        println!("4. UniversalId: Parsing with error handling");
        println!("----------------------------------------------");

        let test_strings = [
            "data.dnv.com/IMO1234567/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature",
            "data.dnv.com/IMO9074729/dnv-v2/vis-3-4a/621.21/S90/sec/411.1/C101/meta/qty-mass/cnt-fuel.oil/pos-inlet",
            "data.dnv.com/IMO9785811/dnv-v2/vis-3-7a/612.21/C701.23/C633/meta/calc~accumulate",
            "",
            "data.dnv.com/INVALID/dnv-v2/vis-3-4a/411.1/meta/qty-temperature",
            "wrong.entity/IMO1234567/dnv-v2/vis-3-4a/411.1/meta/qty-temperature",
            "data.dnv.com/IMO1234567/dnv-v2/vis-3-4a/INVALID/meta/qty-temperature",
        ];

        for str in test_strings {
            let (universal_id, errors) = OwnedUniversalIdBuilder::from_string_with_errors(str);

            let display = if str.is_empty() {
                "(empty)".to_string()
            } else if str.len() > 60 {
                format!("{}...", &str[..60])
            } else {
                str.to_string()
            };
            println!("Parsing: \"{}\"", display);

            if let Some(universal_id) = universal_id {
                println!("  Success");
                println!("    IMO: {}", universal_id.imo_number());
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
        println!("5. UniversalIdBuilder: Modifying existing UniversalIds");
        println!("--------------------------------------------------------");

        let original = OwnedUniversalIdBuilder::from_string(
            "data.dnv.com/IMO1234567/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature",
        );

        if let Some(original) = original {
            println!("Original                    : {}", original);

            let vis = Vis::instance();
            let codebooks = vis
                .codebooks(vis.latest())
                .expect("latest() should always be a valid VIS version");

            let new_imo = ImoNumber::create(9074729).expect("9074729 should be a valid IMO number");
            let modified = original
                .builder()
                .with_imo_number(&new_imo)
                .build()
                .expect("builder with new IMO number should be valid");

            println!("Modified (new IMO)          : {}", modified);

            let cnt_tag = codebooks[CodebookName::Content].create_tag("water");
            if let Some(cnt_tag) = cnt_tag {
                let new_local_id_builder =
                    original.local_id().builder().with_metadata_tag(&cnt_tag);
                let modified2 = original
                    .builder()
                    .with_local_id(&new_local_id_builder)
                    .build()
                    .expect("builder with modified LocalIdBuilder should be valid");

                println!("Modified (added content tag): {}", modified2);
            }
        }

        println!();
    }

    {
        println!("6. UniversalId: Accessing components");
        println!("--------------------------------------");

        let universal_id = OwnedUniversalIdBuilder::from_string(
            "data.dnv.com/IMO9074729/dnv-v2/vis-3-4a/621.21/S90/sec/411.1/C101/meta/qty-mass/cnt-fuel.oil/pos-inlet",
        );

        if let Some(universal_id) = universal_id {
            println!("UniversalId: {}\n", universal_id);

            println!("IMO Number:");
            println!("  Value: {}", universal_id.imo_number());

            println!("\nLocalId components:");
            let local_id = universal_id.local_id();
            println!("  VIS Version   : {}", local_id.version());
            println!("  Primary item  : {}", local_id.primary_item());

            if let Some(secondary_item) = local_id.secondary_item() {
                println!("  Secondary item: {}", secondary_item);
            }

            println!("\nMetadata tags:");
            if let Some(quantity) = local_id.quantity() {
                println!("  Quantity: {}", quantity.value());
            }
            if let Some(content) = local_id.content() {
                println!("  Content : {}", content.value());
            }
            if let Some(position) = local_id.position() {
                println!("  Position: {}", position.value());
            }
        }

        println!();
    }

    {
        println!("7. UniversalId: Equality and comparison");
        println!("-----------------------------------------");

        let uid1 = OwnedUniversalIdBuilder::from_string(
            "data.dnv.com/IMO1234567/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature",
        );
        let uid2 = OwnedUniversalIdBuilder::from_string(
            "data.dnv.com/IMO1234567/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature",
        );
        let uid3 = OwnedUniversalIdBuilder::from_string(
            "data.dnv.com/IMO9074729/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature",
        );
        let uid4 = OwnedUniversalIdBuilder::from_string(
            "data.dnv.com/IMO1234567/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-pressure",
        );

        if let (Some(uid1), Some(uid2), Some(uid3), Some(uid4)) = (uid1, uid2, uid3, uid4) {
            println!("UniversalId1: Same IMO, same LocalId");
            println!("UniversalId2: Same IMO, same LocalId");
            println!("UniversalId3: Different IMO, same LocalId");
            println!("UniversalId4: Same IMO, different LocalId\n");

            println!("uid1 == uid2? {} (same IMO & LocalId)", *uid1 == *uid2);
            println!("uid1 == uid3? {} (different IMO)", *uid1 == *uid3);
            println!("uid1 == uid4? {} (different LocalId)", *uid1 == *uid4);
            println!("uid1 != uid3? {}", *uid1 != *uid3);
        }

        println!();
    }

    {
        println!("8. UniversalIdBuilder: Validation");
        println!("-----------------------------------");

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

        let imo_number = ImoNumber::create(1234567).expect("1234567 should be a valid IMO number");
        let primary_item = OwnedGmodPath::from_short_path("411.1/C101.31-2", gmod, locations);
        let qty_tag = codebooks[CodebookName::Quantity].create_tag("temperature");

        if let (Some(primary_item), Some(qty_tag)) = (primary_item, qty_tag) {
            let local_id_builder = OwnedLocalIdBuilder::create(vis.latest())
                .with_primary_item(&primary_item)
                .with_metadata_tag(&qty_tag);

            let valid_builder = OwnedUniversalIdBuilder::create(vis.latest())
                .with_imo_number(&imo_number)
                .with_local_id(&local_id_builder);

            println!("Valid builder:");
            println!("  Is valid: {}", valid_builder.is_valid());

            let missing_imo =
                OwnedUniversalIdBuilder::create(vis.latest()).with_local_id(&local_id_builder);
            println!("\nBuilder missing IMO number:");
            println!("  Is valid: {}", missing_imo.is_valid());

            let missing_local_id =
                OwnedUniversalIdBuilder::create(vis.latest()).with_imo_number(&imo_number);
            println!("\nBuilder missing LocalId:");
            println!("  Is valid: {}", missing_local_id.is_valid());

            println!("\nAttempting to build without IMO number:");
            match missing_imo.build() {
                Ok(_) => println!("  ERROR: Invalid build succeeded (unexpected)"),
                Err(error) => println!("  Correctly rejected: {}", error),
            }
        }

        println!();
    }

    {
        println!("9. UniversalId: Real-world vessel examples");
        println!("---------------------------------------------");

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

        let sensors = [
            ("MV Queen Mary 2", 9241061, "411.1/C101.31-2", "temperature"),
            ("MS Oasis of the Seas", 9383936, "621.21/S90", "mass"),
            (
                "MS Symphony of the Seas",
                9744001,
                "612.21/C701.23",
                "pressure",
            ),
        ];

        for (vessel_name, imo_number, sensor_path, quantity) in sensors {
            let imo = ImoNumber::create(imo_number).expect("valid IMO number");
            let path = OwnedGmodPath::from_short_path(sensor_path, gmod, locations);
            let tag = codebooks[CodebookName::Quantity].create_tag(quantity);

            if let (Some(path), Some(tag)) = (path, tag) {
                let local_id_builder = OwnedLocalIdBuilder::create(vis.latest())
                    .with_primary_item(&path)
                    .with_metadata_tag(&tag);

                let universal_id = OwnedUniversalIdBuilder::create(vis.latest())
                    .with_imo_number(&imo)
                    .with_local_id(&local_id_builder)
                    .build()
                    .expect("builder with IMO number and LocalIdBuilder should be valid");

                println!("{}:", vessel_name);
                println!("  {}\n", universal_id);
            }
        }
    }

    {
        println!("10. UniversalIdBuilder: Remove and add operations");
        println!("---------------------------------------------------");

        let original = OwnedUniversalIdBuilder::from_string(
            "data.dnv.com/IMO1234567/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature",
        );

        if let Some(original) = original {
            println!("Original: {}", original);

            let without_imo = original.builder().without_imo_number();
            println!("\nAfter without_imo_number():");
            println!("  Is valid: {}", without_imo.is_valid());

            let without_local_id = original.builder().without_local_id();
            println!("\nAfter without_local_id():");
            println!("  Is valid: {}", without_local_id.is_valid());

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

            let new_imo = ImoNumber::create(9074729).expect("9074729 should be a valid IMO number");
            let path = OwnedGmodPath::from_short_path("621.21/S90", gmod, locations);
            let tag = codebooks[CodebookName::Quantity].create_tag("mass");

            if let (Some(path), Some(tag)) = (path, tag) {
                let new_local_id_builder = OwnedLocalIdBuilder::create(vis.latest())
                    .with_primary_item(&path)
                    .with_metadata_tag(&tag);
                let rebuilt = without_imo
                    .with_imo_number(&new_imo)
                    .with_local_id(&new_local_id_builder)
                    .build()
                    .expect("builder with new IMO number and LocalIdBuilder should be valid");

                println!("\nRebuilt with new components: {}", rebuilt);
            }
        }

        println!();
    }
}
