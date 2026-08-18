use vista_sdk::core::gmod_path::OwnedGmodPath;
use vista_sdk::core::vis::Vis;

fn main() {
    println!("=== vista-sdk GmodPath Sample ===\n");

    {
        println!("1. GmodPath: Parsing short paths");
        println!("-----------------------------------");

        let vis = Vis::instance();
        let gmod = vis
            .gmod(vis.latest())
            .expect("latest() should always be a valid VIS version");
        let locations = vis
            .locations(vis.latest())
            .expect("latest() should always be a valid VIS version");

        // Parse simple short path
        if let Some(path1) = OwnedGmodPath::from_short_path("411.1", gmod, locations) {
            println!("Parsed: \"411.1\"");
            println!("  Node     : {}", path1.node().code());
            println!("  Full path: {}", path1.to_full_path_string());
        }

        // Parse path with location
        if let Some(path2) = OwnedGmodPath::from_short_path("411.1-1P", gmod, locations) {
            println!("\nParsed: \"411.1-1P\"");
            println!("  Node     : {}", path2.node().code());
            if let Some(location) = path2.node().location() {
                println!("  Location : {}", location);
            }
            println!("  Full path: {}", path2.to_full_path_string());
        }

        // Parse multi-segment path
        if let Some(path3) = OwnedGmodPath::from_short_path("612.21-1/C701.13/S93", gmod, locations)
        {
            println!("\nParsed: \"612.21-1/C701.13/S93\"");
            println!("  Final node : {}", path3.node().code());
            println!("  Path length: {} nodes", path3.length());
            println!("  Full path  : {}", path3.to_full_path_string());
        }

        // Parse multi-segment path with errors
        let (path4, errors) =
            OwnedGmodPath::from_short_path_with_errors("C101.63-2P/S206.22", gmod, locations);
        if let Some(path4) = path4 {
            println!("\nParsed: \"C101.63-2P/S206.22\"");
            println!("  Final node : {}", path4.node().code());
            println!("  Path length: {} nodes", path4.length());
            println!("  Full path  : {}", path4.to_full_path_string());
        } else if errors.has_errors() {
            println!("\nFailed to parse \"C101.63-2P/S206.22\":");
            for error in errors.iter() {
                println!("  - {}", error.message);
            }
        }

        println!();
    }

    {
        println!("2. GmodPath: Parsing full paths");
        println!("----------------------------------");

        let vis = Vis::instance();
        let gmod = vis
            .gmod(vis.latest())
            .expect("latest() should always be a valid VIS version");
        let locations = vis
            .locations(vis.latest())
            .expect("latest() should always be a valid VIS version");

        // Full path starting from root (VE)
        if let Some(path) =
            OwnedGmodPath::from_full_path("VE/400a/410/411/411i/411.1-1P", gmod, locations)
        {
            println!("Parsed full path: \"VE/400a/410/411/411i/411.1-1P\"");
            println!("  Final node : {}", path.node().code());
            println!("  Path length: {} nodes", path.length());
            if let Some(location) = path.node().location() {
                println!("  Location   : {}", location);
            }
        }

        // Complex full path
        if let Some(path2) = OwnedGmodPath::from_full_path(
            "VE/600a/610/612/612.2/612.2i-1/612.21-1/CS10/C701/C701.1/C701.13/S93",
            gmod,
            locations,
        ) {
            println!("\nParsed: \"VE/600a/610/612/612.2/612.2i-1/612.21-1/CS10/C701/C701.1/C701.13/S93\"");
            println!("  Final node : {}", path2.node().code());
            println!("  Path length: {} nodes", path2.length());
            println!("  Full path  : {}", path2.to_full_path_string());
        }

        println!();
    }

    {
        println!("3. GmodPath: Invalid paths with error handling");
        println!("-------------------------------------------------");

        let vis = Vis::instance();
        let gmod = vis
            .gmod(vis.latest())
            .expect("latest() should always be a valid VIS version");
        let locations = vis
            .locations(vis.latest())
            .expect("latest() should always be a valid VIS version");

        let invalid_paths = [
            "",                 // Empty path
            "INVALID",          // Non-existent node
            "411.1-XYZ",        // Invalid location
            "VE/INVALID/411.1", // Invalid node in full path
        ];

        for path_str in invalid_paths {
            let (path, errors) =
                OwnedGmodPath::from_short_path_with_errors(path_str, gmod, locations);

            println!("Path: \"{}\"", path_str);
            if path.is_some() {
                println!("  Valid (unexpected!)");
            } else {
                println!("  Invalid - Errors:");
                for error in errors.iter() {
                    println!("    - {}", error.message);
                }
            }
            println!();
        }
    }

    {
        println!("4. GmodPath: Navigating path nodes");
        println!("-------------------------------------");

        let vis = Vis::instance();
        let gmod = vis
            .gmod(vis.latest())
            .expect("latest() should always be a valid VIS version");
        let locations = vis
            .locations(vis.latest())
            .expect("latest() should always be a valid VIS version");

        if let Some(path) = OwnedGmodPath::from_full_path(
            "VE/600a/610/612/612.2/612.2i-1/612.21-1/CS10/C701/C701.1/C701.13/S93",
            gmod,
            locations,
        ) {
            println!("Path: {}", path.to_full_path_string());
            println!("\nAll nodes in path:");

            // Iterate through parent nodes
            let mut index = 0;
            for parent in path.parents() {
                let mut node_str = parent.code().to_string();
                if let Some(location) = parent.location() {
                    node_str.push('-');
                    node_str.push_str(location);
                }
                println!(
                    "  [{:>2}] {:<12} ({})",
                    index,
                    node_str,
                    parent.metadata().name()
                );
                index += 1;
            }

            // Show the final target node
            let node = path.node();
            let mut node_str = node.code().to_string();
            if let Some(location) = node.location() {
                node_str.push('-');
                node_str.push_str(location);
            }
            println!(
                "  [{:>2}] {:<12} ({})",
                index,
                node_str,
                node.metadata().name()
            );

            println!("\nFinal node:");
            println!("  Code    : {}", path.node().code());
            println!("  Name    : {}", path.node().metadata().name());
            println!("  Category: {}", path.node().metadata().category());
        }

        println!();
    }

    {
        println!("5. GmodPath: Individualization");
        println!("--------------------------------");

        let vis = Vis::instance();
        let gmod = vis
            .gmod(vis.latest())
            .expect("latest() should always be a valid VIS version");
        let locations = vis
            .locations(vis.latest())
            .expect("latest() should always be a valid VIS version");

        // Parse path that can be individualized (411i and 411.1 are in the same individualizable set)
        if let Some(path) = OwnedGmodPath::from_short_path("411.1/C101.62/S205", gmod, locations) {
            println!("Path: {}", path);
            println!("Is individualizable: {}", path.is_individualizable());

            let sets: Vec<_> = path.individualizable_sets().collect();
            println!("Individualizable sets: {}", sets.len());

            for (i, set) in sets.iter().enumerate() {
                println!("\nSet {}:", i + 1);
                println!("  Nodes  : {}", set.to_string().unwrap_or_default());
                print!("  Indices: ");
                for j in 0..set.index_count() {
                    if let Some(idx) = set.index_at(j) {
                        print!("{} ", idx);
                    }
                }
                println!();
            }

            // Parse the same path individualized (location applied)
            if let Some(ind_path) =
                OwnedGmodPath::from_short_path("411.1-1/C101.62/S205", gmod, locations)
            {
                println!("\nIndividualized: {}", ind_path);
                println!("Full path     : {}", ind_path.to_full_path_string());
            }
        }

        println!();
    }

    {
        println!("6. GmodPath: Conversion between formats");
        println!("------------------------------------------");

        let vis = Vis::instance();
        let gmod = vis
            .gmod(vis.latest())
            .expect("latest() should always be a valid VIS version");
        let locations = vis
            .locations(vis.latest())
            .expect("latest() should always be a valid VIS version");

        // Parse short path
        if let Some(short_path) = OwnedGmodPath::from_short_path("411.1-1P", gmod, locations) {
            println!("Short path:{}", short_path);
            println!("Full path : {}", short_path.to_full_path_string());
        }

        // Parse full path
        if let Some(full_path) =
            OwnedGmodPath::from_full_path("VE/400a/410/411/411i/411.1-1P", gmod, locations)
        {
            println!("\nFull path : {}", full_path.to_full_path_string());
            println!("Short path: {}", full_path);
        }

        println!();
    }

    {
        println!("7. GmodPath: Comparison and equality");
        println!("---------------------------------------");

        let vis = Vis::instance();
        let gmod = vis
            .gmod(vis.latest())
            .expect("latest() should always be a valid VIS version");
        let locations = vis
            .locations(vis.latest())
            .expect("latest() should always be a valid VIS version");

        let path1 = OwnedGmodPath::from_short_path("411.1-1P", gmod, locations);
        let path2 = OwnedGmodPath::from_full_path("VE/400a/410/411/411i/411.1-1P", gmod, locations);
        let path3 = OwnedGmodPath::from_short_path("411.1-2P", gmod, locations);

        if let (Some(path1), Some(path2), Some(path3)) = (path1, path2, path3) {
            println!("Path 1: {}", path1);
            println!("Path 2: {}", path2);
            println!("Path 3: {}\n", path3);

            println!("Path 1 == Path 2? {}", *path1 == *path2);
            println!("Path 1 == Path 3? {}", *path1 == *path3);
            println!("Path 1 != Path 3? {}", *path1 != *path3);
        }

        println!();
    }

    {
        println!("8. GmodPath: Working with product types/selections");
        println!("-----------------------------------------------------");

        let vis = Vis::instance();
        let gmod = vis
            .gmod(vis.latest())
            .expect("latest() should always be a valid VIS version");
        let locations = vis
            .locations(vis.latest())
            .expect("latest() should always be a valid VIS version");

        // Example 1: Node with product selection
        if let Some(path1) = OwnedGmodPath::from_short_path("411.1-1P", gmod, locations) {
            let node = path1.node();
            println!("Path                 : {}", path1);
            println!("Node code            : {}", node.code());
            println!("Is product selection : {}", node.is_product_selection());

            if let Some(product_selection) = node.product_selection() {
                println!("Has product selection: {}", product_selection.code());
            }
        }

        // Example 2: Product node (from the boiler path)
        if let Some(path2) = OwnedGmodPath::from_short_path("612.21-1/C701.13/S93", gmod, locations)
        {
            let node = path2.node();
            println!("\nPath      : {}", path2);
            println!("Node code : {} ({})", node.code(), node.metadata().name());
            println!("Category  : {}", node.metadata().category());
            println!("Is product: {}", node.metadata().category() == "PRODUCT");
        }

        println!();
    }
}
