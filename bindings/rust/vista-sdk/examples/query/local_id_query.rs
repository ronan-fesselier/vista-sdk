use vista_sdk::core::codebook_name::CodebookName;
use vista_sdk::core::gmod_path::OwnedGmodPath;
use vista_sdk::core::local_id::OwnedLocalId;
use vista_sdk::core::vis::Vis;
use vista_sdk::query::gmod_path_query_builder::OwnedGmodPathQueryBuilder;
use vista_sdk::query::local_id_query_builder::OwnedLocalIdQueryBuilder;
use vista_sdk::query::metadata_tags_query_builder::OwnedMetadataTagsQueryBuilder;

fn main() {
    println!("=== vista-sdk LocalIdQuery Sample ===\n");

    {
        println!("1. LocalIdQuery: Matching by primary item");
        println!("--------------------------------------------");

        let vis = Vis::instance();
        let version = vis.latest();
        let gmod = vis.gmod(version).expect("version should be valid");
        let locations = vis.locations(version).expect("version should be valid");

        let primary_path = OwnedGmodPath::from_short_path("411.1/C101.31", gmod, locations);
        let local_id1 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature");
        let local_id2 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31-5/meta/qty-pressure");
        let local_id3 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.63/meta/qty-temperature");

        if let (Some(primary_path), Some(local_id1), Some(local_id2), Some(local_id3)) =
            (primary_path, local_id1, local_id2, local_id3)
        {
            let path_query = OwnedGmodPathQueryBuilder::from_path(&primary_path)
                .expect("path builder should succeed")
                .without_locations()
                .expect("without_locations should succeed")
                .build();

            let query = OwnedLocalIdQueryBuilder::create()
                .with_primary_item_query(&path_query)
                .expect("with_primary_item_query should succeed")
                .build();

            println!("Query: Match LocalIds with primary item '411.1/C101.31' (any location)\n");

            println!("Test LocalId 1: {}", local_id1);
            println!("  Matches: {}\n", query.match_local_id(&local_id1));

            println!("Test LocalId 2: {}", local_id2);
            println!("  Matches: {}\n", query.match_local_id(&local_id2));

            println!("Test LocalId 3: {}", local_id3);
            println!(
                "  Matches: {} (different node)",
                query.match_local_id(&local_id3)
            );
        }

        println!();
    }

    {
        println!("2. LocalIdQuery: Combined query (primary item + tags)");
        println!("-------------------------------------------------------");

        let vis = Vis::instance();
        let version = vis.latest();
        let gmod = vis.gmod(version).expect("version should be valid");

        let node411 = gmod.get_node("411.1");
        let node_c101 = gmod.get_node("C101");
        let local_id1 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature");
        let local_id2 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-pressure");
        let local_id3 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.63/meta/qty-temperature");

        if let (Ok(node411), Ok(node_c101), Some(local_id1), Some(local_id2), Some(local_id3)) =
            (node411, node_c101, local_id1, local_id2, local_id3)
        {
            let path_query = OwnedGmodPathQueryBuilder::create()
                .with_node_all_locations(node411, true)
                .expect("with_node_all_locations should succeed")
                .with_node_all_locations(node_c101, true)
                .expect("with_node_all_locations should succeed")
                .build();

            let tags_query = OwnedMetadataTagsQueryBuilder::create()
                .with_tag(CodebookName::Quantity, "temperature")
                .build();

            let query = OwnedLocalIdQueryBuilder::create()
                .with_primary_item_query(&path_query)
                .expect("with_primary_item_query should succeed")
                .with_tags(&tags_query)
                .expect("with_tags should succeed")
                .build();

            println!("Query: Match LocalIds with primary='411.1/C101' AND qty-temperature\n");

            println!("Test LocalId 1: {}", local_id1);
            println!(
                "  Matches: {} (has both)\n",
                query.match_local_id(&local_id1)
            );

            println!("Test LocalId 2: {}", local_id2);
            println!(
                "  Matches: {} (missing tag)\n",
                query.match_local_id(&local_id2)
            );

            println!("Test LocalId 3: {}", local_id3);
            println!(
                "  Matches: {} (C101.63 is a subnode of C101)",
                query.match_local_id(&local_id3)
            );
        }

        println!();
    }

    {
        println!("3. LocalIdQuery: Secondary item requirement");
        println!("----------------------------------------------");

        let local_id1 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature");
        let local_id2 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/621.21/S90/sec/411.1/C101/meta/qty-mass");

        if let (Some(local_id1), Some(local_id2)) = (local_id1, local_id2) {
            let query = OwnedLocalIdQueryBuilder::create()
                .without_secondary_item()
                .expect("without_secondary_item should succeed")
                .build();

            println!("Query: Match LocalIds WITHOUT secondary item\n");

            println!("Test LocalId 1 (no secondary): {}", local_id1);
            println!("  Matches: {}\n", query.match_local_id(&local_id1));

            println!("Test LocalId 2 (has secondary): {}", local_id2);
            println!("  Matches: {}", query.match_local_id(&local_id2));
        }

        println!();
    }

    {
        println!("4. Building queries from existing LocalIds");
        println!("---------------------------------------------");

        let reference = OwnedLocalId::from_string(
            "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
        );
        let local_id1 = OwnedLocalId::from_string(
            "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
        );
        let local_id2 = OwnedLocalId::from_string(
            "/dnv-v2/vis-3-4a/411.1/C101.31-5/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
        );
        let local_id3 = OwnedLocalId::from_string(
            "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-pressure/cnt-exhaust.gas/pos-inlet",
        );

        if let (Some(reference), Some(local_id1), Some(local_id2), Some(local_id3)) =
            (reference, local_id1, local_id2, local_id3)
        {
            let query = OwnedLocalIdQueryBuilder::from_local_id(&reference)
                .expect("from_local_id should succeed")
                .build();

            println!("Reference LocalId: {}\n", reference);
            println!("Query: Match exact copy of reference LocalId\n");

            println!(
                "Test LocalId 1 (exact match):\n  Matches: {}\n",
                query.match_local_id(&local_id1)
            );
            println!(
                "Test LocalId 2 (different location):\n  Matches: {}\n",
                query.match_local_id(&local_id2)
            );
            println!(
                "Test LocalId 3 (different tag):\n  Matches: {}",
                query.match_local_id(&local_id3)
            );
        }

        println!();
    }

    {
        println!("5. LocalIdQuery: primary item via Nodes builder");
        println!("--------------------------------------------------");

        let vis = Vis::instance();
        let version = vis.latest();
        let gmod = vis.gmod(version).expect("version should be valid");

        let node411 = gmod.get_node("411.1");
        let node_c101 = gmod.get_node("C101");
        let local_id1 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature");
        let local_id2 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.63/meta/qty-pressure");
        let local_id3 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C102.31/meta/qty-temperature");

        if let (Ok(node411), Ok(node_c101), Some(local_id1), Some(local_id2), Some(local_id3)) =
            (node411, node_c101, local_id1, local_id2, local_id3)
        {
            let path_query = OwnedGmodPathQueryBuilder::create()
                .with_node_all_locations(node411, true)
                .expect("with_node_all_locations should succeed")
                .with_node_all_locations(node_c101, true)
                .expect("with_node_all_locations should succeed")
                .build();

            let query = OwnedLocalIdQueryBuilder::create()
                .with_primary_item_query(&path_query)
                .expect("with_primary_item_query should succeed")
                .build();

            println!(
                "Query: Match LocalIds with primary containing nodes 411.1 AND C101 (any locations)\n"
            );

            println!("Test LocalId 1: {}", local_id1);
            println!("  Matches: {}\n", query.match_local_id(&local_id1));

            println!("Test LocalId 2: {}", local_id2);
            println!("  Matches: {}\n", query.match_local_id(&local_id2));

            println!("Test LocalId 3: {}", local_id3);
            println!(
                "  Matches: {} (wrong second node)",
                query.match_local_id(&local_id3)
            );
        }

        println!();
    }

    {
        println!("6. LocalIdQuery: primary item via Path builder (without locations)");
        println!("---------------------------------------------------------------------");

        let vis = Vis::instance();
        let version = vis.latest();
        let gmod = vis.gmod(version).expect("version should be valid");
        let locations = vis.locations(version).expect("version should be valid");

        let base_path = OwnedGmodPath::from_short_path("411.1/C101.31", gmod, locations);
        let local_id1 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature");
        let local_id2 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31-5/meta/qty-pressure");
        let local_id3 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.63/meta/qty-temperature");

        if let (Some(base_path), Some(local_id1), Some(local_id2), Some(local_id3)) =
            (base_path, local_id1, local_id2, local_id3)
        {
            let path_query = OwnedGmodPathQueryBuilder::from_path(&base_path)
                .expect("path builder should succeed")
                .without_locations()
                .expect("without_locations should succeed")
                .build();

            let query = OwnedLocalIdQueryBuilder::create()
                .with_primary_item_query(&path_query)
                .expect("with_primary_item_query should succeed")
                .build();

            println!("Query: Match LocalIds with primary '411.1/C101.31' (any location)\n");

            println!("Test LocalId 1: {}", local_id1);
            println!("  Matches: {}\n", query.match_local_id(&local_id1));

            println!("Test LocalId 2: {}", local_id2);
            println!("  Matches: {}\n", query.match_local_id(&local_id2));

            println!("Test LocalId 3: {}", local_id3);
            println!(
                "  Matches: {} (different node)",
                query.match_local_id(&local_id3)
            );
        }

        println!();
    }

    {
        println!("7. LocalIdQuery: withAnyNodeBefore usage");
        println!("--------------------------------------------");

        let vis = Vis::instance();
        let version = vis.latest();
        let gmod = vis.gmod(version).expect("version should be valid");
        let locations = vis.locations(version).expect("version should be valid");

        let base_path = OwnedGmodPath::from_short_path("411.1/C101.31", gmod, locations);
        let local_id1 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature");
        let local_id2 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/E202.1/E31/meta/qty-pressure");
        let local_id3 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/621.21/S90/meta/qty-temperature");

        if let (Some(base_path), Some(local_id1), Some(local_id2), Some(local_id3)) =
            (base_path, local_id1, local_id2, local_id3)
        {
            let path_query = OwnedGmodPathQueryBuilder::from_path(&base_path)
                .expect("path builder should succeed")
                .with_any_node_before("C101")
                .expect("C101 is in the path")
                .without_locations()
                .expect("without_locations should succeed")
                .build();

            let query = OwnedLocalIdQueryBuilder::create()
                .with_primary_item_query(&path_query)
                .expect("with_primary_item_query should succeed")
                .build();

            println!(
                "Query: Match LocalIds with any node before C101 (ignoring nodes before C101)\n"
            );

            println!("Test LocalId 1 (411.1): {}", local_id1);
            println!("  Matches: {}\n", query.match_local_id(&local_id1));

            println!("Test LocalId 2 (411.1/E202.1): {}", local_id2);
            println!(
                "  Matches: {} (no C101)\n",
                query.match_local_id(&local_id2)
            );

            println!("Test LocalId 3 (621.21): {}", local_id3);
            println!("  Matches: {} (no C101)", query.match_local_id(&local_id3));
        }

        println!();
    }
}
