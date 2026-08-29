use vista_sdk::*;

#[test]
fn version_matches_the_requested_version() {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");

    assert_eq!(gmod.version(), VisVersion::V3_11a);
}

#[test]
fn root_node_returns_node_with_code_ve() {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");

    assert_eq!(gmod.root_node().code(), "VE");
}

#[test]
fn get_node_known_code_returns_matching_node() {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");

    let node = gmod.get_node("411.1").expect("411.1 should exist");
    assert_eq!(node.code(), "411.1");
}

#[test]
fn get_node_unknown_code_returns_err() {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");

    assert!(gmod.get_node("not-a-code").is_err());
}

#[test]
fn node_count_and_at_cover_all_nodes_including_root() {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");

    let count = gmod.node_count();
    assert!(count > 0);

    let found_root = gmod.iter().any(|node| node.code() == "VE");
    assert!(found_root);
    assert_eq!(gmod.iter().count(), count);
}

#[test]
fn node_at_out_of_range_returns_none() {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");
    let count = gmod.node_count();

    assert!(gmod.node_at(count).is_none());
}

#[test]
fn traverse_visits_root_and_stops_early() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");

    let mut visited = Vec::new();
    let completed = gmod.traverse(
        |_parents, node| {
            visited.push(node.code().to_string());
            if visited.len() >= 20 {
                TraversalHandlerResult::Stop
            } else {
                TraversalHandlerResult::Continue
            }
        },
        1,
    );

    assert!(!completed);
    assert!(visited.iter().any(|c| c == "VE"));
    assert_eq!(visited.len(), 20);
}

#[test]
fn traverse_skips_subtree() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");

    let mut visited = Vec::new();
    gmod.traverse(
        |_parents, node| {
            let c = node.code().to_string();
            visited.push(c.clone());
            if c == "400a" {
                TraversalHandlerResult::SkipSubtree
            } else {
                TraversalHandlerResult::Continue
            }
        },
        1,
    );

    assert!(!visited.iter().any(|c| c == "410"));
    assert!(visited.iter().any(|c| c == "VE"));
    assert!(visited.iter().any(|c| c == "500a"));
}

#[test]
fn is_product_type_assignment_valid_pair_returns_true() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");

    let parent = gmod.get_node("1241.31").expect("1241.31 should exist");
    let child = gmod.get_node("C1013").expect("C1013 should exist");

    assert!(Gmod::is_product_type_assignment(Some(parent), Some(child)));
    assert!(!Gmod::is_product_type_assignment(Some(child), Some(parent)));
    assert!(!Gmod::is_product_type_assignment(None, Some(child)));
    assert!(!Gmod::is_product_type_assignment(Some(parent), None));
}

#[test]
fn is_product_selection_assignment_valid_pair_returns_true() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");

    let child = gmod.get_node("CS1").expect("CS1 should exist");
    let parent = child
        .parent_at(0)
        .expect("CS1 should have at least one parent");

    assert!(Gmod::is_product_selection_assignment(
        Some(parent),
        Some(child)
    ));
    assert!(!Gmod::is_product_selection_assignment(
        Some(child),
        Some(parent)
    ));
    assert!(!Gmod::is_product_selection_assignment(None, Some(child)));
    assert!(!Gmod::is_product_selection_assignment(Some(parent), None));
}
