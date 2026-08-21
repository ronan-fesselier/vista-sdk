use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;
use vista_sdk::query::gmod_path_query_builder::OwnedGmodPathQueryBuilder;

#[test]
fn path_builder_always_matches_itself() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let path =
        vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path("411.1-1/C101", gmod, locations)
            .expect("411.1-1/C101 should parse");

    let query = OwnedGmodPathQueryBuilder::from_path(&path)
        .expect("path builder should build")
        .build();

    assert!(query.match_path(&path));
}

#[test]
fn path_builder_parametrized() {
    let cases: &[(&str, VisVersion, &[(&str, &[&str])], bool)] = &[
        ("411.1-1/C101", VisVersion::V3_4a, &[], true),
        (
            "411.1-1/C101",
            VisVersion::V3_4a,
            &[("411.1", &["1"])],
            true,
        ),
        (
            "411.1-1/C101",
            VisVersion::V3_4a,
            &[("411.1", &["A"])],
            false,
        ),
        (
            "433.1-P/C322.31/C173",
            VisVersion::V3_4a,
            &[("C322.31", &[])],
            true,
        ),
        (
            "433.1-P/C322.31-2/C173",
            VisVersion::V3_4a,
            &[("433.1", &["P"]), ("C322.31", &[])],
            true,
        ),
        (
            "433.1-P/C322.31-2/C173",
            VisVersion::V3_4a,
            &[("433.1", &["A"]), ("C322.31", &[])],
            false,
        ),
        (
            "433.1-P/C322.31-2/C173",
            VisVersion::V3_4a,
            &[("433.1", &["P"]), ("C322.31", &["1"])],
            false,
        ),
        (
            "433.1-A/C322.31-2/C173",
            VisVersion::V3_4a,
            &[("433.1", &["P"]), ("C322.31", &["1"])],
            false,
        ),
        (
            "433.1-A/C322.31-2/C173",
            VisVersion::V3_4a,
            &[("433.1", &[]), ("C322.31", &[])],
            true,
        ),
        (
            "433.1/C322.31-2/C173",
            VisVersion::V3_4a,
            &[("433.1", &["A"])],
            false,
        ),
        (
            "433.1/C322.31-2/C173",
            VisVersion::V3_4a,
            &[("433.1", &[])],
            true,
        ),
    ];

    for (path_str, version, parameters, expected_match) in cases {
        let vis = Vis::instance();
        let gmod = vis.gmod(*version).expect("version should be valid");
        let locations = vis.locations(*version).expect("version should be valid");

        let path =
            vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(path_str, gmod, locations)
                .unwrap_or_else(|| panic!("valid path should parse: {path_str}"));

        let mut builder = OwnedGmodPathQueryBuilder::from_path(&path)
            .unwrap_or_else(|e| panic!("{path_str}: {e:?}"));

        assert!(
            builder.build().match_path(&path),
            "unmodified builder should match itself: {path_str}"
        );

        for (node_code, loc_strs) in *parameters {
            if loc_strs.is_empty() {
                builder = builder
                    .path_with_node_all_locations(node_code, true)
                    .unwrap_or_else(|e| panic!("{path_str}: {node_code}: {e:?}"));
            } else {
                let parsed: Vec<_> = loc_strs
                    .iter()
                    .map(|s| {
                        locations
                            .parse(s)
                            .unwrap_or_else(|| panic!("location should parse: {s}"))
                    })
                    .collect();
                let refs: Vec<_> = parsed.iter().collect();
                builder = builder
                    .path_with_node_locations(node_code, &refs)
                    .unwrap_or_else(|e| panic!("{path_str}: {node_code}: {e:?}"));
            }
        }

        assert_eq!(
            builder.build().match_path(&path),
            *expected_match,
            "path={path_str} parameters={parameters:?}"
        );
    }
}

#[test]
fn nodes_builder_parametrized() {
    let cases: &[(&str, VisVersion, &[(&str, &[&str])], bool)] = &[
        (
            "411.1-1/C101",
            VisVersion::V3_4a,
            &[("411.1", &["1"])],
            true,
        ),
        (
            "411.1-1/C101.61/S203.3/S110.2/C101",
            VisVersion::V3_7a,
            &[("411.1", &["1"])],
            true,
        ),
        (
            "411.1/C101.61-1/S203.3/S110.2/C101",
            VisVersion::V3_7a,
            &[("C101.61", &["1"])],
            true,
        ),
        (
            "511.11/C101.61-1/S203.3/S110.2/C101",
            VisVersion::V3_7a,
            &[("C101.61", &["1"])],
            true,
        ),
        (
            "411.1/C101.61-1/S203.3/S110.2/C101",
            VisVersion::V3_7a,
            &[("C101.61", &[])],
            true,
        ),
        (
            "511.11/C101.61-1/S203.3/S110.2/C101",
            VisVersion::V3_7a,
            &[("C101.61", &[])],
            true,
        ),
        (
            "221.11/C1141.421/C1051.7/C101.61-2/S203",
            VisVersion::V3_7a,
            &[("C101.61", &[])],
            true,
        ),
        (
            "411.1/C101.61-1/S203.3/S110.2/C101",
            VisVersion::V3_7a,
            &[("411.1", &[]), ("C101.61", &[])],
            true,
        ),
        (
            "511.11/C101.61-1/S203.3/S110.2/C101",
            VisVersion::V3_7a,
            &[("411.1", &[]), ("C101.61", &[])],
            false,
        ),
        (
            "411.1/C101.61/S203.3-1/S110.2/C101",
            VisVersion::V3_7a,
            &[("S203.3", &["1"])],
            true,
        ),
    ];

    for (path_str, version, parameters, expected_match) in cases {
        let vis = Vis::instance();
        let gmod = vis.gmod(*version).expect("version should be valid");
        let locations = vis.locations(*version).expect("version should be valid");

        let path =
            vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(path_str, gmod, locations)
                .unwrap_or_else(|| panic!("valid path should parse: {path_str}"));

        let mut builder = OwnedGmodPathQueryBuilder::create();

        for (node_code, loc_strs) in *parameters {
            let node = gmod
                .get_node(node_code)
                .unwrap_or_else(|e| panic!("{path_str}: {node_code}: {e:?}"));

            if loc_strs.is_empty() {
                builder = builder
                    .with_node_all_locations(node, true)
                    .unwrap_or_else(|e| panic!("{path_str}: {node_code}: {e:?}"));
            } else {
                let parsed: Vec<_> = loc_strs
                    .iter()
                    .map(|s| {
                        locations
                            .parse(s)
                            .unwrap_or_else(|| panic!("location should parse: {s}"))
                    })
                    .collect();
                let refs: Vec<_> = parsed.iter().collect();
                builder = builder
                    .with_node_locations(node, &refs)
                    .unwrap_or_else(|e| panic!("{path_str}: {node_code}: {e:?}"));
            }
        }

        assert_eq!(
            builder.build().match_path(&path),
            *expected_match,
            "path={path_str} parameters={parameters:?}"
        );
    }
}

#[test]
fn with_any_node_before_ignores_parent_nodes() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_9a).expect("V3_9a should be valid");
    let locations = vis
        .locations(VisVersion::V3_9a)
        .expect("V3_9a should be valid");

    let base_path = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "411.1/C101.31",
        gmod,
        locations,
    )
    .expect("411.1/C101.31 should parse");

    let query = OwnedGmodPathQueryBuilder::from_path(&base_path)
        .expect("path builder should build")
        .with_any_node_before("C101")
        .expect("C101 is in the path")
        .without_locations()
        .expect("without_locations should succeed")
        .build();

    assert!(query.match_path(&base_path));

    let path_diff_parent = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "511.11/C101.31",
        gmod,
        locations,
    )
    .expect("511.11/C101.31 should parse");
    assert!(query.match_path(&path_diff_parent));

    let path_with_loc = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "411.1/C101.31-2",
        gmod,
        locations,
    )
    .expect("411.1/C101.31-2 should parse");
    assert!(query.match_path(&path_with_loc));

    let path_diff_c_node = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "411.1/C102.31",
        gmod,
        locations,
    )
    .expect("411.1/C102.31 should parse");
    assert!(!query.match_path(&path_diff_c_node));
}

#[test]
fn with_any_node_before_matches_any_parent_for_sensor() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let sensor_path = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "411.1/C101.63/S206",
        gmod,
        locations,
    )
    .expect("411.1/C101.63/S206 should parse");

    let query = OwnedGmodPathQueryBuilder::from_path(&sensor_path)
        .expect("path builder should build")
        .without_locations()
        .expect("without_locations should succeed")
        .with_any_node_before("S206")
        .expect("S206 is in the path")
        .build();

    let path_loc1 = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "411.1-1/C101.63/S206",
        gmod,
        locations,
    )
    .expect("411.1-1/C101.63/S206 should parse");
    let path_loc2 = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "411.1-2/C101.63/S206",
        gmod,
        locations,
    )
    .expect("411.1-2/C101.63/S206 should parse");
    assert!(query.match_path(&path_loc1));
    assert!(query.match_path(&path_loc2));

    let path_c10131 = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "411.1/C101.31-2",
        gmod,
        locations,
    )
    .expect("411.1/C101.31-2 should parse");
    let path_s203 = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "411.1/C101.61/S203",
        gmod,
        locations,
    )
    .expect("411.1/C101.61/S203 should parse");
    assert!(!query.match_path(&path_c10131));
    assert!(!query.match_path(&path_s203));
}

#[test]
fn with_any_node_before_preserves_location_requirement_on_target_node() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let base_path = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "433.1-P/C322.31",
        gmod,
        locations,
    )
    .expect("433.1-P/C322.31 should parse");

    let query = OwnedGmodPathQueryBuilder::from_path(&base_path)
        .expect("path builder should build")
        .with_any_node_before("C322")
        .expect("C322 is in the path")
        .build();

    assert!(query.match_path(&base_path));

    let path_diff_parent_loc = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "433.1-S/C322.31",
        gmod,
        locations,
    )
    .expect("433.1-S/C322.31 should parse");
    assert!(query.match_path(&path_diff_parent_loc));

    let path_no_parent_loc = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "433.1/C322.31",
        gmod,
        locations,
    )
    .expect("433.1/C322.31 should parse");
    assert!(query.match_path(&path_no_parent_loc));
}

#[test]
fn with_any_node_after_ignores_children_via_prefix_matching() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let base_path = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "411.1/C101.31",
        gmod,
        locations,
    )
    .expect("411.1/C101.31 should parse");

    let query = OwnedGmodPathQueryBuilder::from_path(&base_path)
        .expect("path builder should build")
        .without_locations()
        .expect("without_locations should succeed")
        .with_any_node_after("411.1")
        .expect("411.1 is in the path")
        .build();

    let path_c10131 = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "411.1/C101.31-2",
        gmod,
        locations,
    )
    .expect("411.1/C101.31-2 should parse");
    let path_s206 = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "411.1/C101.63/S206",
        gmod,
        locations,
    )
    .expect("411.1/C101.63/S206 should parse");
    let path_loc1 = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "411.1-1/C101.61/S203",
        gmod,
        locations,
    )
    .expect("411.1-1/C101.61/S203 should parse");
    assert!(query.match_path(&path_c10131));
    assert!(query.match_path(&path_s206));
    assert!(query.match_path(&path_loc1));

    let path_511 = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "511.11/C101.63/S206",
        gmod,
        locations,
    )
    .expect("511.11/C101.63/S206 should parse");
    let path_652 = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "652.31/S90.3/S61",
        gmod,
        locations,
    )
    .expect("652.31/S90.3/S61 should parse");
    assert!(!query.match_path(&path_511));
    assert!(!query.match_path(&path_652));
}

#[test]
fn with_any_node_after_ignores_suffix_mid_path() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let base_path = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "411.1/C101.63/S206",
        gmod,
        locations,
    )
    .expect("411.1/C101.63/S206 should parse");

    let query = OwnedGmodPathQueryBuilder::from_path(&base_path)
        .expect("path builder should build")
        .without_locations()
        .expect("without_locations should succeed")
        .with_any_node_after("C101.6")
        .expect("C101.6 is in the path")
        .build();

    let path_s206 = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "411.1/C101.63/S206",
        gmod,
        locations,
    )
    .expect("411.1/C101.63/S206 should parse");
    let path_loc = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "411.1-1/C101.61/S203",
        gmod,
        locations,
    )
    .expect("411.1-1/C101.61/S203 should parse");
    assert!(query.match_path(&path_s206));
    assert!(query.match_path(&path_loc));

    let path_c10131 = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "411.1/C101.31-2",
        gmod,
        locations,
    )
    .expect("411.1/C101.31-2 should parse");
    assert!(!query.match_path(&path_c10131));
}

#[test]
fn with_any_node_after_errors_when_node_not_in_path() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let base_path = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "411.1/C101.63/S206",
        gmod,
        locations,
    )
    .expect("411.1/C101.63/S206 should parse");

    let builder = OwnedGmodPathQueryBuilder::from_path(&base_path)
        .expect("path builder should build")
        .without_locations()
        .expect("without_locations should succeed");

    let result = builder.with_any_node_after("C101.31");
    assert!(
        result.is_err(),
        "with_any_node_after must fail when the node is not in the path"
    );

    let safe_query = OwnedGmodPathQueryBuilder::from_path(&base_path)
        .expect("path builder should build")
        .without_locations()
        .expect("without_locations should succeed")
        .build();
    assert!(safe_query.match_path(&base_path));
}

#[test]
fn without_locations_ignores_location_individualization() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let base_path = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "411.1-1/C101.31",
        gmod,
        locations,
    )
    .expect("411.1-1/C101.31 should parse");

    let query = OwnedGmodPathQueryBuilder::from_path(&base_path)
        .expect("path builder should build")
        .without_locations()
        .expect("without_locations should succeed")
        .build();

    for path_str in ["411.1-1/C101.31", "411.1-2/C101.31", "411.1/C101.31"] {
        let path =
            vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(path_str, gmod, locations)
                .unwrap_or_else(|| panic!("valid path should parse: {path_str}"));
        assert!(query.match_path(&path), "should match: {path_str}");
    }
}

#[test]
fn builder_immutability_returns_new_builder() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let path = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "411.1/C101.31",
        gmod,
        locations,
    )
    .expect("411.1/C101.31 should parse");

    let builder1 = OwnedGmodPathQueryBuilder::from_path(&path).expect("path builder should build");
    let query1 = builder1.build();

    let builder2 = builder1
        .without_locations()
        .expect("without_locations should succeed");
    let query2 = builder2.build();

    assert!(query1.match_path(&path));
    assert!(query2.match_path(&path));
}

#[test]
fn path_only_methods_error_on_nodes_builder() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let node = gmod.get_node("411.1").expect("411.1 should exist");

    let nodes_builder = OwnedGmodPathQueryBuilder::create()
        .with_node_all_locations(node, true)
        .expect("with_node_all_locations should succeed");

    assert!(
        nodes_builder
            .path_with_node_all_locations("411.1", true)
            .is_err(),
        "path_with_node_all_locations must error on a Nodes-variant builder"
    );
    assert!(
        nodes_builder.with_any_node_before("411.1").is_err(),
        "with_any_node_before must error on a Nodes-variant builder"
    );
    assert!(
        nodes_builder.with_any_node_after("411.1").is_err(),
        "with_any_node_after must error on a Nodes-variant builder"
    );
    assert!(
        nodes_builder.without_locations().is_err(),
        "without_locations must error on a Nodes-variant builder"
    );

    let path = vista_sdk::core::gmod_path::OwnedGmodPath::from_short_path(
        "411.1/C101.31",
        gmod,
        locations,
    )
    .expect("411.1/C101.31 should parse");
    let path_builder =
        OwnedGmodPathQueryBuilder::from_path(&path).expect("path builder should build");

    assert!(
        path_builder.with_node_all_locations(node, true).is_err(),
        "with_node_all_locations must error on a Path-variant builder"
    );
    assert!(
        path_builder.with_node_locations(node, &[]).is_err(),
        "with_node_locations must error on a Path-variant builder"
    );
}
