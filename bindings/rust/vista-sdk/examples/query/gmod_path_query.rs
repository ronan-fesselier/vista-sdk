use vista_sdk::core::gmod_path::OwnedGmodPath;
use vista_sdk::core::vis::Vis;
use vista_sdk::query::gmod_path_query_builder::OwnedGmodPathQueryBuilder;

fn main() {
    println!("=== vista-sdk GmodPathQuery Sample ===\n");

    {
        println!("1. GmodPathQuery: Matching paths without locations");
        println!("-----------------------------------------------------");

        let vis = Vis::instance();
        let version = vis.latest();
        let gmod = vis.gmod(version).expect("version should be valid");
        let locations = vis.locations(version).expect("version should be valid");

        let base_path = OwnedGmodPath::from_short_path("411.1/C101.31", gmod, locations);
        let path1 = OwnedGmodPath::from_short_path("411.1/C101.31-2", gmod, locations);
        let path2 = OwnedGmodPath::from_short_path("411.1/C101.31-5", gmod, locations);
        let path3 = OwnedGmodPath::from_short_path("411.1/C101.63", gmod, locations);

        if let (Some(base_path), Some(path1), Some(path2), Some(path3)) =
            (base_path, path1, path2, path3)
        {
            let query = OwnedGmodPathQueryBuilder::from_path(&base_path)
                .expect("path builder should build")
                .without_locations()
                .expect("without_locations should succeed")
                .build();

            println!("Query: Match '411.1/C101.31' ignoring locations\n");

            println!("Test path 1: {}", path1);
            println!("  Matches: {}\n", query.match_path(&path1));

            println!("Test path 2: {}", path2);
            println!("  Matches: {}\n", query.match_path(&path2));

            println!("Test path 3: {}", path3);
            println!("  Matches: {} (different node)", query.match_path(&path3));
        }

        println!();
    }

    {
        println!("2. GmodPathQuery: Matching specific nodes (Nodes builder)");
        println!("-----------------------------------------------------------");

        let vis = Vis::instance();
        let version = vis.latest();
        let gmod = vis.gmod(version).expect("version should be valid");
        let locations = vis.locations(version).expect("version should be valid");

        let node411 = gmod.get_node("411.1");
        let node_c101 = gmod.get_node("C101");
        let path1 = OwnedGmodPath::from_short_path("411.1-1P/C101.31-2", gmod, locations);
        let path2 = OwnedGmodPath::from_short_path("411.1-2P/C101.63", gmod, locations);
        let path3 = OwnedGmodPath::from_short_path("511.11/C104", gmod, locations);

        if let (Ok(node411), Ok(node_c101), Some(path1), Some(path2), Some(path3)) =
            (node411, node_c101, path1, path2, path3)
        {
            let query = OwnedGmodPathQueryBuilder::create()
                .with_node_all_locations(node411, true)
                .expect("with_node_all_locations should succeed")
                .with_node_all_locations(node_c101, true)
                .expect("with_node_all_locations should succeed")
                .build();

            println!("Query: Match paths containing nodes '411.1' AND 'C101' (any locations)\n");

            println!("Test path 1: {}", path1);
            println!("  Matches: {}\n", query.match_path(&path1));

            println!("Test path 2: {}", path2);
            println!("  Matches: {}\n", query.match_path(&path2));

            println!("Test path 3: {}", path3);
            println!(
                "  Matches: {} (different first node)",
                query.match_path(&path3)
            );
        }

        println!();
    }

    {
        println!("3. GmodPathQuery: withAnyNodeBefore - match any parent");
        println!("--------------------------------------------------------");

        let vis = Vis::instance();
        let version = vis.latest();
        let gmod = vis.gmod(version).expect("version should be valid");
        let locations = vis.locations(version).expect("version should be valid");

        let base_path = OwnedGmodPath::from_short_path("411.1/C101.63/S206", gmod, locations);
        let path1 = OwnedGmodPath::from_short_path("411.1-1/C101.63/S206", gmod, locations);
        let path2 = OwnedGmodPath::from_short_path("411.1-2/C101.63/S206", gmod, locations);
        let path3 = OwnedGmodPath::from_short_path("411.1/C101.31-2", gmod, locations);

        if let (Some(base_path), Some(path1), Some(path2), Some(path3)) =
            (base_path, path1, path2, path3)
        {
            let query = OwnedGmodPathQueryBuilder::from_path(&base_path)
                .expect("path builder should build")
                .without_locations()
                .expect("without_locations should succeed")
                .with_any_node_before("S206")
                .expect("S206 is in the path")
                .build();

            println!("Query: Match any path ending with S206 (ignore parents)\n");

            println!("Test path 1: {}", path1);
            println!("  Matches: {}\n", query.match_path(&path1));

            println!("Test path 2: {}", path2);
            println!("  Matches: {}\n", query.match_path(&path2));

            println!("Test path 3: {}", path3);
            println!("  Matches: {} (no S206 node)", query.match_path(&path3));
        }

        println!();
    }

    {
        println!("4. GmodPathQuery: withAnyNodeAfter - match any children");
        println!("----------------------------------------------------------");

        let vis = Vis::instance();
        let version = vis.latest();
        let gmod = vis.gmod(version).expect("version should be valid");
        let locations = vis.locations(version).expect("version should be valid");

        let base_path = OwnedGmodPath::from_short_path("411.1/C101.31", gmod, locations);
        let path1 = OwnedGmodPath::from_short_path("411.1/C101.31-2", gmod, locations);
        let path2 = OwnedGmodPath::from_short_path("411.1/C101.63/S206", gmod, locations);
        let path3 = OwnedGmodPath::from_short_path("511.11/C104", gmod, locations);

        if let (Some(base_path), Some(path1), Some(path2), Some(path3)) =
            (base_path, path1, path2, path3)
        {
            let query = OwnedGmodPathQueryBuilder::from_path(&base_path)
                .expect("path builder should build")
                .without_locations()
                .expect("without_locations should succeed")
                .with_any_node_after("411.1")
                .expect("411.1 is in the path")
                .build();

            println!("Query: Match any path starting with '411.1' (ignore children)\n");

            println!("Test path 1: {}", path1);
            println!("  Matches: {}\n", query.match_path(&path1));

            println!("Test path 2: {}", path2);
            println!("  Matches: {}\n", query.match_path(&path2));

            println!("Test path 3: {}", path3);
            println!(
                "  Matches: {} (different first node)",
                query.match_path(&path3)
            );
        }

        println!();
    }
}
