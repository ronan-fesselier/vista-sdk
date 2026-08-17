use vista_sdk::core::gmod::TraversalHandlerResult;
use vista_sdk::core::vis::Vis;

fn main() {
    println!("=== vista-sdk GmodTraversal Sample ===\n");

    {
        println!("1. Gmod::traverse: Counting all nodes depth-first");
        println!("--------------------------------------------------");

        let vis = Vis::instance();
        let gmod = vis
            .gmod(vis.latest())
            .expect("latest() should always be a valid VIS version");

        let mut total = 0usize;
        gmod.traverse(
            |_parents, _node| {
                total += 1;
                TraversalHandlerResult::Continue
            },
            1,
        );

        println!("Total nodes visited (DFS): {}\n", total);
    }

    {
        println!("2. Gmod::traverse: Collecting nodes at depth 2");
        println!("------------------------------------------------");

        let vis = Vis::instance();
        let gmod = vis
            .gmod(vis.latest())
            .expect("latest() should always be a valid VIS version");

        let mut depth2 = Vec::new();
        gmod.traverse(
            |parents, node| {
                if parents.len() == 2 {
                    depth2.push(node.code().to_string());
                }
                TraversalHandlerResult::Continue
            },
            1,
        );

        println!("Nodes at depth 2: {}", depth2.len());
        for code in depth2.iter().take(5) {
            println!("  {}", code);
        }
        if depth2.len() > 5 {
            println!("  ... and {} more", depth2.len() - 5);
        }

        println!();
    }

    {
        println!("3. Gmod::traverse: Stopping early after 10 nodes");
        println!("-------------------------------------------------");

        let vis = Vis::instance();
        let gmod = vis
            .gmod(vis.latest())
            .expect("latest() should always be a valid VIS version");

        let mut visited = Vec::new();
        let completed = gmod.traverse(
            |_parents, node| {
                visited.push(node.code().to_string());
                if visited.len() >= 10 {
                    TraversalHandlerResult::Stop
                } else {
                    TraversalHandlerResult::Continue
                }
            },
            1,
        );

        println!("Traversal completed: {}", completed);
        println!("First 10 nodes in DFS order:");
        for code in &visited {
            println!("  {}", code);
        }

        println!();
    }

    {
        println!("4. Gmod::traverse: Skipping a subtree");
        println!("---------------------------------------");

        let vis = Vis::instance();
        let gmod = vis
            .gmod(vis.latest())
            .expect("latest() should always be a valid VIS version");

        let mut skipped_code = String::new();
        let mut first_child = true;
        let mut count = 0usize;

        gmod.traverse(
            |parents, node| {
                if parents.len() == 1 && first_child {
                    first_child = false;
                    skipped_code = node.code().to_string();
                    return TraversalHandlerResult::SkipSubtree;
                }
                count += 1;
                TraversalHandlerResult::Continue
            },
            1,
        );

        println!("Skipped subtree rooted at: {}", skipped_code);
        println!("Nodes visited after skip : {}\n", count);
    }

    {
        println!("5. Gmod::traverse: Accumulating state");
        println!("---------------------------------------");

        let vis = Vis::instance();
        let gmod = vis
            .gmod(vis.latest())
            .expect("latest() should always be a valid VIS version");

        let mut leaf_count = 0usize;
        let mut max_depth = 0usize;

        gmod.traverse(
            |parents, node| {
                if node.is_leaf_node() {
                    leaf_count += 1;
                }
                max_depth = max_depth.max(parents.len());
                TraversalHandlerResult::Continue
            },
            1,
        );

        println!("Leaf nodes visited: {}", leaf_count);
        println!("Max path depth    : {}\n", max_depth);
    }
}
