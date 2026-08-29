use vista_sdk::*;

fn with_gmod<F: FnOnce(&Gmod)>(f: F) {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(VisVersion::V3_11a)
        .expect("V3_11a must be available");
    f(gmod);
}

#[test]
fn root_node_metadata_category_and_type_are_non_empty() {
    with_gmod(|gmod| {
        let meta: &GmodNodeMetadata = gmod.root_node().metadata();
        assert!(!meta.category().is_empty());
        assert!(!meta.r#type().is_empty());
    });
}

#[test]
fn root_node_metadata_full_type_contains_category_and_type() {
    with_gmod(|gmod| {
        let meta = gmod.root_node().metadata();
        let full = meta.full_type();
        assert!(
            full.contains(meta.category()),
            "full_type '{full}' should contain category '{}'",
            meta.category()
        );
        assert!(
            full.contains(meta.r#type()),
            "full_type '{full}' should contain type '{}'",
            meta.r#type()
        );
    });
}

#[test]
fn root_node_metadata_name_is_non_empty() {
    with_gmod(|gmod| {
        let meta = gmod.root_node().metadata();
        assert!(!meta.name().is_empty());
    });
}

#[test]
fn node_411_1_metadata_category_is_asset_function() {
    with_gmod(|gmod| {
        let node = gmod.get_node("411.1").expect("411.1 must exist");
        let meta = node.metadata();
        assert_eq!(meta.category(), "ASSET FUNCTION");
    });
}

#[test]
fn node_411_1_metadata_type_is_non_empty() {
    with_gmod(|gmod| {
        let node = gmod.get_node("411.1").expect("411.1 must exist");
        let meta = node.metadata();
        assert!(!meta.r#type().is_empty());
    });
}

#[test]
fn common_name_is_some_or_none_without_panic() {
    with_gmod(|gmod| {
        let node = gmod.get_node("411.1").expect("411.1 must exist");
        let _ = node.metadata().common_name();
    });
}

#[test]
fn definition_is_some_or_none_without_panic() {
    with_gmod(|gmod| {
        let node = gmod.get_node("411.1").expect("411.1 must exist");
        let _ = node.metadata().definition();
    });
}

#[test]
fn common_definition_is_some_or_none_without_panic() {
    with_gmod(|gmod| {
        let node = gmod.get_node("411.1").expect("411.1 must exist");
        let _ = node.metadata().common_definition();
    });
}

#[test]
fn install_substructure_is_some_or_none_without_panic() {
    with_gmod(|gmod| {
        let node = gmod.get_node("411.1").expect("411.1 must exist");
        let _ = node.metadata().install_substructure();
    });
}

#[test]
fn normal_assignment_name_count_is_consistent_with_at() {
    with_gmod(|gmod| {
        let node = gmod.get_node("411.1").expect("411.1 must exist");
        let meta = node.metadata();
        let count = meta.normal_assignment_name_count();
        for i in 0..count {
            let (k, v) = meta
                .normal_assignment_name_at(i)
                .expect("index within count() must be valid");
            assert!(!k.is_empty());
            assert!(!v.is_empty());
        }
        assert!(meta.normal_assignment_name_at(count).is_none());
    });
}

#[test]
fn normal_assignment_names_iterator_matches_count() {
    with_gmod(|gmod| {
        let node = gmod.get_node("411.1").expect("411.1 must exist");
        let meta = node.metadata();
        assert_eq!(
            meta.normal_assignment_names().count(),
            meta.normal_assignment_name_count()
        );
    });
}

#[test]
fn all_nodes_metadata_non_empty_category() {
    with_gmod(|gmod| {
        for node in gmod.iter() {
            let meta = node.metadata();
            assert!(
                !meta.category().is_empty(),
                "node {} has empty category",
                node.code()
            );
        }
    });
}
