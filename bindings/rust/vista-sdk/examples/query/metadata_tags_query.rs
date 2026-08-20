use vista_sdk::core::codebook_name::CodebookName;
use vista_sdk::core::local_id::OwnedLocalId;
use vista_sdk::query::metadata_tags_query_builder::OwnedMetadataTagsQueryBuilder;

fn main() {
    println!("=== vista-sdk MetadataTagsQuery Sample ===\n");

    {
        println!("1. MetadataTagsQuery: Matching LocalIds by metadata tags");
        println!("-----------------------------------------------------------");

        let query = OwnedMetadataTagsQueryBuilder::create()
            .with_tag(CodebookName::Quantity, "temperature")
            .build();

        let local_id1 = OwnedLocalId::from_string(
            "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas",
        );
        let local_id2 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.63/meta/qty-pressure/cnt-air");

        if let (Some(local_id1), Some(local_id2)) = (local_id1, local_id2) {
            println!("Query: Match LocalIds with qty-temperature tag\n");
            println!("Test LocalId 1: {}", local_id1);
            println!("  Matches: {}\n", query.r#match(&local_id1));

            println!("Test LocalId 2: {}", local_id2);
            println!("  Matches: {}", query.r#match(&local_id2));
        }

        println!();
    }

    {
        println!("2. MetadataTagsQuery: Multiple tags with AND semantics");
        println!("---------------------------------------------------------");

        let query = OwnedMetadataTagsQueryBuilder::create()
            .with_tag(CodebookName::Quantity, "volume")
            .with_tag(CodebookName::Content, "cargo")
            .build();

        let local_id1 = OwnedLocalId::from_string(
            "/dnv-v2/vis-3-4a/1021.1i-6P/H123/meta/qty-volume/cnt-cargo/pos~percentage",
        );
        let local_id2 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/1021.1i-6P/H123/meta/qty-volume");

        if let (Some(local_id1), Some(local_id2)) = (local_id1, local_id2) {
            println!("Query: Match LocalIds with qty-volume AND cnt-cargo\n");
            println!("Test LocalId 1: {}", local_id1);
            println!("  Has qty-volume: true, Has cnt-cargo: true");
            println!("  Matches: {}\n", query.r#match(&local_id1));

            println!("Test LocalId 2: {}", local_id2);
            println!("  Has qty-volume: true, Has cnt-cargo: false");
            println!("  Matches: {}", query.r#match(&local_id2));
        }

        println!();
    }

    {
        println!("3. MetadataTagsQuery: Subset vs Exact matching");
        println!("-------------------------------------------------");

        let subset_query = OwnedMetadataTagsQueryBuilder::create()
            .with_tag(CodebookName::Quantity, "pressure")
            .with_allow_other_tags(true)
            .build();

        let exact_query = OwnedMetadataTagsQueryBuilder::create()
            .with_tag(CodebookName::Quantity, "pressure")
            .with_allow_other_tags(false)
            .build();

        let local_id1 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.63/meta/qty-pressure");
        let local_id2 =
            OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.63/meta/qty-pressure/cnt-air");

        if let (Some(local_id1), Some(local_id2)) = (local_id1, local_id2) {
            println!("Query: Match LocalIds with qty-pressure\n");

            println!("Test LocalId 1 (only qty-pressure): {}", local_id1);
            println!(
                "  Subset query matches: {}",
                subset_query.r#match(&local_id1)
            );
            println!(
                "  Exact query matches: {}\n",
                exact_query.r#match(&local_id1)
            );

            println!("Test LocalId 2 (qty-pressure + cnt-air): {}", local_id2);
            println!(
                "  Subset query matches: {} (allows extra tags)",
                subset_query.r#match(&local_id2)
            );
            println!(
                "  Exact query matches: {} (no extra tags allowed)",
                exact_query.r#match(&local_id2)
            );
        }

        println!();
    }
}
