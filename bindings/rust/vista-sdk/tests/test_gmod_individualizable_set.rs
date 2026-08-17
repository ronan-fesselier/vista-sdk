use vista_sdk::core::gmod_individualizable_set::GmodIndividualizableSet;
use vista_sdk::core::gmod_path::GmodPath;
use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;

#[test]
fn node_count_and_node_at() {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(VisVersion::V3_4a)
        .expect("V3_4a must be available");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a must be available");

    let path = GmodPath::from_short_path("652.4/I101", gmod, locations).expect("must parse");
    let set = path.individualizable_set_at(0).expect("set 0 must exist");

    let count = set.node_count().expect("must not be built");
    assert_eq!(count, 1);
    let node = set.node_at(0).expect("node 0 must exist");
    assert_eq!(node.code(), "652.4");
}

#[test]
fn index_count_and_index_at() {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(VisVersion::V3_4a)
        .expect("V3_4a must be available");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a must be available");

    let path = GmodPath::from_short_path("652.4/I101", gmod, locations).expect("must parse");
    let set = path.individualizable_set_at(0).expect("set 0 must exist");

    let count = set.index_count();
    assert!(count > 0);
    let idx = set.index_at(0).expect("index 0 must exist");
    assert!(idx >= 0);
    assert!(set.index_at(count).is_none());
}

#[test]
fn to_string_contains_node_code() {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(VisVersion::V3_4a)
        .expect("V3_4a must be available");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a must be available");

    let path = GmodPath::from_short_path("652.4/I101", gmod, locations).expect("must parse");
    let set = path.individualizable_set_at(0).expect("set 0 must exist");

    let s = set.to_string().expect("must not be built");
    assert!(s.contains("652.4"));
}

#[test]
fn location_none_before_assignment() {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(VisVersion::V3_4a)
        .expect("V3_4a must be available");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a must be available");

    let path = GmodPath::from_short_path("652.4/I101", gmod, locations).expect("must parse");
    let set = path.individualizable_set_at(0).expect("set 0 must exist");

    let loc = set.location().expect("must not be built");
    assert!(loc.is_none());
}

#[test]
fn build_returns_path() {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(VisVersion::V3_4a)
        .expect("V3_4a must be available");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a must be available");

    let path = GmodPath::from_short_path("652.4/I101", gmod, locations).expect("must parse");
    let set = path.individualizable_set_at(0).expect("set 0 must exist");

    let built = set.build().expect("build must succeed");
    assert!(!built.to_string().is_empty());
}

#[test]
fn create_roundtrip() {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(VisVersion::V3_4a)
        .expect("V3_4a must be available");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a must be available");

    let path = GmodPath::from_short_path("652.4/I101", gmod, locations).expect("must parse");
    let set = path.individualizable_set_at(0).expect("set 0 must exist");

    let indices: Vec<i32> = (0..set.index_count())
        .map(|i| set.index_at(i).expect("must exist"))
        .collect();

    let created = GmodIndividualizableSet::create(&indices, &path).expect("create must succeed");
    assert_eq!(
        created.node_count().expect("must not be built"),
        set.node_count().expect("must not be built")
    );
}
