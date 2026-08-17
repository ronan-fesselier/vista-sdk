use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;

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
