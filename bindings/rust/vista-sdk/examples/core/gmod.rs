use vista_sdk::core::vis::Vis;

fn main() {
    println!("=== vista-sdk Gmod Sample ===\n");

    {
        println!("1. Gmod: Accessing Gmod for a VIS version");
        println!("--------------------------------------------");

        let vis = Vis::instance();
        let version = vis.latest();
        let gmod = vis
            .gmod(version)
            .expect("latest() should always be a valid VIS version");

        println!("Gmod for version   : {}", gmod.version().as_str());

        let node_count = gmod.iter().count();

        println!("Total nodes in Gmod: {}", node_count);
        println!("Root node code     : {}", gmod.root_node().code());
        println!();
    }

    {
        println!("2. GmodNode: Accessing nodes by code");
        println!("---------------------------------------");

        let vis = Vis::instance();
        let gmod = vis
            .gmod(vis.latest())
            .expect("latest() should always be a valid VIS version");

        // Access node using get_node
        let node1 = gmod.get_node("411.1").expect("411.1 should exist");
        println!("Node code    : {}", node1.code());
        println!("Node name    : {}", node1.metadata().name());
        println!("Node category: {}", node1.metadata().category());
        println!("Node type    : {}", node1.metadata().r#type());

        // Safe access using get_node
        if let Ok(node2) = gmod.get_node("C101.31") {
            println!("\nFound node: {}", node2.code());
            println!("Name      : {}", node2.metadata().name());
        }

        // Try invalid code
        let invalid = gmod.get_node("INVALID");
        println!("\nInvalid code lookup: {}", invalid.is_ok());

        println!();
    }

    {
        println!("3. GmodNode: Navigating the tree hierarchy");
        println!("---------------------------------------------");

        let vis = Vis::instance();
        let gmod = vis
            .gmod(vis.latest())
            .expect("latest() should always be a valid VIS version");

        let node = gmod.get_node("411.1").expect("411.1 should exist");

        // Parents (can have multiple in Gmod graph)
        if node.parent_count() > 0 {
            println!("Parents of {} ({}):", node.code(), node.parent_count());
            for parent in node.parents() {
                println!("  - {}", parent.code());
            }
        }

        // Children
        println!("\nChildren of {} ({}):", node.code(), node.child_count());
        let max_display = 5;
        for (count, child) in node.children().enumerate() {
            if count >= max_display {
                println!("  ... and {} more", node.child_count() - max_display);
                break;
            }
            println!("  - {} ({})", child.code(), child.metadata().name());
        }

        // Root check
        println!("\nIs root  ? {}", node.is_root());
        println!("Root node: {}", gmod.root_node().code());
        println!("Is root  ? {}", gmod.root_node().is_root());

        println!();
    }

    {
        println!("4. GmodNode: Node metadata");
        println!("----------------------------");

        let vis = Vis::instance();
        let gmod = vis
            .gmod(vis.latest())
            .expect("latest() should always be a valid VIS version");

        let node = gmod.get_node("S222.11").expect("S222.11 should exist");
        let metadata = node.metadata();

        println!("Node                : {}", node.code());
        println!("Category            : {}", metadata.category());
        println!("Type                : {}", metadata.r#type());
        println!("Name                : {}", metadata.name());

        if let Some(definition) = metadata.definition() {
            println!("Definition      : {}", definition);
        }

        if let Some(install_substructure) = metadata.install_substructure() {
            println!("Install substructure: {}", install_substructure);
        }

        println!();
    }

    {
        println!("5. GmodNode: Product types and selections");
        println!("--------------------------------------------");

        let vis = Vis::instance();
        let gmod = vis
            .gmod(vis.latest())
            .expect("latest() should always be a valid VIS version");

        // Product type example
        let node1 = gmod.get_node("411.3").expect("411.3 should exist");
        println!("Node             : {}", node1.code());
        println!("Has product type : {}", node1.product_type().is_some());
        if let Some(prod_type) = node1.product_type() {
            println!("Product type code: {}", prod_type.code());
            println!("Product type name: {}", prod_type.metadata().name());
        }

        // Product selection example
        let node2 = gmod.get_node("411.2").expect("411.2 should exist");
        println!("\nNode: {}", node2.code());
        println!(
            "Has product selection : {}",
            node2.product_selection().is_some()
        );
        if let Some(prod_sel) = node2.product_selection() {
            println!("Product selection code: {}", prod_sel.code());
            println!("Product selection name: {}", prod_sel.metadata().name());
        }

        // Check if node is product selection
        if let Ok(cs_node) = gmod.get_node("CS1") {
            println!("\nNode: {}", cs_node.code());
            println!("Is product selection: {}", cs_node.is_product_selection());
        }

        println!();
    }

    {
        println!("6. GmodNode: Mappability");
        println!("--------------------------");

        let vis = Vis::instance();
        let gmod = vis
            .gmod(vis.latest())
            .expect("latest() should always be a valid VIS version");

        let test_codes = [
            "VE",      // Root - not mappable
            "411",     // Mappable
            "411.1",   // Not mappable
            "C101",    // Mappable
            "C101.31", // Mappable
            "CS1",     // Product selection - not mappable
        ];

        for code in test_codes {
            if let Ok(node) = gmod.get_node(code) {
                println!("  {:<7}: {}", code, node.is_mappable());
            }
        }

        println!();
    }

    {
        println!("7. Gmod: Iterating all nodes");
        println!("------------------------------");

        let vis = Vis::instance();
        let gmod = vis
            .gmod(vis.latest())
            .expect("latest() should always be a valid VIS version");

        println!("First 10 nodes:");
        for (count, node) in gmod.iter().enumerate() {
            if count >= 10 {
                break;
            }
            println!("  {:<8} - {}", node.code(), node.metadata().name());
        }

        println!();
    }

    {
        println!("8. GmodNode: Finding specific node types");
        println!("-------------------------------------------");

        let vis = Vis::instance();
        let gmod = vis
            .gmod(vis.latest())
            .expect("latest() should always be a valid VIS version");

        // Find PRODUCT TYPE nodes
        println!("Sample PRODUCT TYPE nodes:");
        let mut count = 0;
        for node in gmod.iter() {
            if node.metadata().category() == "PRODUCT" && node.metadata().r#type() == "TYPE" {
                if count >= 5 {
                    break;
                }
                count += 1;
                println!("  {:<10} - {}", node.code(), node.metadata().name());
            }
        }

        // Find ASSET FUNCTION LEAF nodes
        println!("\nSample ASSET FUNCTION LEAF nodes:");
        let mut count = 0;
        for node in gmod.iter() {
            if node.is_leaf_node() && node.metadata().category() == "ASSET FUNCTION" {
                if count >= 5 {
                    break;
                }
                count += 1;
                println!("  {:<10} - {}", node.code(), node.metadata().name());
            }
        }

        println!();
    }

    {
        println!("9. GmodNode: Tree navigation example");
        println!("----------------------------------------");

        let vis = Vis::instance();
        let gmod = vis
            .gmod(vis.latest())
            .expect("latest() should always be a valid VIS version");

        // Navigate from a node to root
        let node = gmod.get_node("C101.31").expect("C101.31 should exist");
        println!("Path from {} to root:", node.code());

        let mut current = Some(node);
        let mut depth = 0;
        let mut visited = std::collections::HashSet::new();
        while let Some(n) = current {
            if !visited.insert(n.code()) {
                println!(
                    "{}- [cycle detected at {}]",
                    " ".repeat(depth * 2),
                    n.code()
                );
                break;
            }

            println!(
                "{}- {} ({})",
                " ".repeat(depth * 2),
                n.code(),
                n.metadata().name()
            );

            // Navigate to first parent (if any)
            current = n.parent_at(0);
            depth += 1;
        }

        println!();
    }

    {
        println!("10. GmodNode: Node properties comparison");
        println!("-------------------------------------------");

        let vis = Vis::instance();
        let gmod = vis
            .gmod(vis.latest())
            .expect("latest() should always be a valid VIS version");

        let codes = ["411.1", "C101", "CS1", "F201"];

        println!(
            "{:<12}{:<20}{:<15}{:<12}{:<12}",
            "Code", "Category", "Type", "Mappable", "IsLeaf"
        );
        println!("{}", "-".repeat(71));

        for code in codes {
            if let Ok(node) = gmod.get_node(code) {
                println!(
                    "{:<12}{:<20}{:<15}{:<12}{:<12}",
                    code,
                    node.metadata().category(),
                    node.metadata().r#type(),
                    node.is_mappable(),
                    node.is_leaf_node()
                );
            }
        }

        println!();
    }

    {
        println!("11. Gmod: Working with different versions");
        println!("--------------------------------------------");

        let vis = Vis::instance();

        // Compare node counts across versions
        println!("Node counts across VIS versions:");
        for version in vis.versions() {
            let gmod = vis
                .gmod(version)
                .expect("versions() only returns valid VIS versions");
            let count = gmod.iter().count();

            println!("  {:<5}: {:>4} nodes", version.as_str(), count);
        }

        println!();
    }
}
