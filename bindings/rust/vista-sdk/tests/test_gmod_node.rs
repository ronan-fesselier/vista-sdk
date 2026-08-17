use vista_sdk::core::gmod::Gmod;
use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;

fn with_gmod<F: FnOnce(&Gmod)>(f: F) {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(VisVersion::V3_4a)
        .expect("V3_4a must be available");
    f(gmod);
}

#[test]
fn root_node_properties() {
    with_gmod(|gmod| {
        let root = gmod.root_node();
        assert_eq!(root.code(), "VE");
        assert_eq!(root.version(), VisVersion::V3_4a);
        assert!(root.is_root());
        assert!(root.location().is_none());
        assert_eq!(root.parent_count(), 0);
        assert!(root.child_count() > 0);
    });
}

#[test]
fn children_iterator_matches_child_at() {
    with_gmod(|gmod| {
        let root = gmod.root_node();
        let from_iter: Vec<&str> = root.children().map(|n| n.code()).collect();
        let from_at: Vec<&str> = (0..root.child_count())
            .map(|i| root.child_at(i).unwrap().code())
            .collect();
        assert_eq!(from_iter, from_at);
        assert!(!from_iter.is_empty());
    });
}

#[test]
fn parents_iterator_matches_parent_at() {
    with_gmod(|gmod| {
        let node = gmod.get_node("411").expect("411 must exist");
        let from_iter: Vec<&str> = node.parents().map(|n| n.code()).collect();
        let from_at: Vec<&str> = (0..node.parent_count())
            .map(|i| node.parent_at(i).unwrap().code())
            .collect();
        assert_eq!(from_iter, from_at);
        assert!(!from_iter.is_empty());
    });
}

#[test]
fn is_child_and_is_child_code() {
    with_gmod(|gmod| {
        let root = gmod.root_node();
        let child = root.child_at(0).expect("root must have at least one child");
        assert!(root.is_child(child));
        assert!(root.is_child_code(child.code()));
        assert!(!root.is_child_code("UNKNOWN"));
    });
}

#[test]
fn leaf_node_is_leaf_and_mappable() {
    with_gmod(|gmod| {
        let node = gmod.get_node("F201.11").expect("F201.11 must exist");
        assert!(node.is_leaf_node());
        assert!(node.is_mappable());
    });
}

#[test]
fn display_contains_code() {
    with_gmod(|gmod| {
        let node = gmod.get_node("411.1").expect("411.1 must exist");
        let s = node.to_string();
        assert!(
            s.contains("411.1"),
            "Display output {s:?} should contain the node code"
        );
    });
}
