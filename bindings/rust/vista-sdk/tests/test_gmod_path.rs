use serde_json::Value;
use std::str::FromStr;
use vista_sdk::*;

static GMOD_PATHS_JSON: &str = include_str!("../../../../testdata/GmodPaths.json");
static INDIVIDUALIZABLE_SETS_JSON: &str =
    include_str!("../../../../testdata/IndividualizableSets.json");

#[test]
fn parse_valid_paths() {
    let doc: Value = serde_json::from_str(GMOD_PATHS_JSON).expect("valid JSON");
    let vis = Vis::instance();

    for item in doc["Valid"].as_array().expect("Valid array") {
        let path_str = item["path"].as_str().unwrap();
        let version_str = item["visVersion"].as_str().unwrap();
        let version = VisVersion::from_str(version_str)
            .unwrap_or_else(|_| panic!("unknown version: {version_str}"));

        let gmod = vis.gmod(version).expect("version should be valid");
        let locations = vis.locations(version).expect("version should be valid");

        let path = GmodPath::from_short_path(path_str, gmod, locations)
            .unwrap_or_else(|| panic!("valid path should parse: {path_str}"));

        assert_eq!(
            path_str,
            path.to_string(),
            "round-trip failed for: {path_str}"
        );
    }
}

#[test]
fn parse_invalid_paths() {
    let doc: Value = serde_json::from_str(GMOD_PATHS_JSON).expect("valid JSON");
    let vis = Vis::instance();

    for item in doc["Invalid"].as_array().expect("Invalid array") {
        let path_str = item["path"].as_str().unwrap();
        let version_str = item["visVersion"].as_str().unwrap();
        let version = VisVersion::from_str(version_str)
            .unwrap_or_else(|_| panic!("unknown version: {version_str}"));

        let gmod = vis.gmod(version).expect("version should be valid");
        let locations = vis.locations(version).expect("version should be valid");

        let result = GmodPath::from_short_path(path_str, gmod, locations);
        assert!(
            result.is_none(),
            "invalid path should not parse: {path_str}"
        );
    }
}

#[test]
fn full_path_iteration() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let expected = [
        (0, "VE"),
        (1, "400a"),
        (2, "410"),
        (3, "411"),
        (4, "411i"),
        (5, "411.1"),
        (6, "CS1"),
        (7, "C101"),
        (8, "C101.7"),
        (9, "C101.72"),
        (10, "I101"),
    ];

    let path = GmodPath::from_short_path("411.1/C101.72/I101", gmod, locations)
        .expect("411.1/C101.72/I101 should parse");

    assert_eq!(path.length(), expected.len());
    for (i, (_, code)) in expected.iter().enumerate() {
        assert_eq!(path.at(i).unwrap().code(), *code);
    }
}

#[test]
fn full_path_parsing_round_trip() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let cases = [
        (
            "411.1/C101.72/I101",
            "VE/400a/410/411/411i/411.1/CS1/C101/C101.7/C101.72/I101",
        ),
        (
            "612.21-1/C701.13/S93",
            "VE/600a/610/612/612.2/612.2i-1/612.21-1/CS10/C701/C701.1/C701.13/S93",
        ),
    ];

    for (short, expected_full) in cases {
        let path = GmodPath::from_short_path(short, gmod, locations)
            .unwrap_or_else(|| panic!("should parse: {short}"));

        assert_eq!(path.to_full_path_string(), expected_full, "short={short}");

        let reparsed = GmodPath::from_full_path(expected_full, gmod, locations)
            .unwrap_or_else(|| panic!("full path should parse: {expected_full}"));

        assert_eq!(path.to_string(), reparsed.to_string());
        assert_eq!(path.to_full_path_string(), reparsed.to_full_path_string());
    }
}

#[test]
fn individualizable_sets_short_paths() {
    let doc: Value = serde_json::from_str(INDIVIDUALIZABLE_SETS_JSON).expect("valid JSON");
    let vis = Vis::instance();

    for item in doc.as_array().expect("root array") {
        let is_full = item["isFullPath"].as_bool().unwrap();
        let path_str = item["path"].as_str().unwrap();
        let version_str = item["visVersion"].as_str().unwrap();
        let version = VisVersion::from_str(version_str)
            .unwrap_or_else(|_| panic!("unknown version: {version_str}"));

        let gmod = vis.gmod(version).expect("version should be valid");
        let locations = vis.locations(version).expect("version should be valid");

        let path_opt = if is_full {
            GmodPath::from_full_path(path_str, gmod, locations)
        } else {
            GmodPath::from_short_path(path_str, gmod, locations)
        };

        if item["expected"].is_null() {
            assert!(path_opt.is_none(), "expected parse failure for: {path_str}");
            continue;
        }

        let path = path_opt.unwrap_or_else(|| panic!("should parse: {path_str}"));
        let expected_sets: Vec<Vec<&str>> = item["expected"]
            .as_array()
            .unwrap()
            .iter()
            .map(|s| {
                s.as_array()
                    .unwrap()
                    .iter()
                    .map(|v| v.as_str().unwrap())
                    .collect()
            })
            .collect();

        assert_eq!(
            path.individualizable_set_count(),
            expected_sets.len(),
            "set count mismatch for: {path_str}"
        );

        for (i, expected_codes) in expected_sets.iter().enumerate() {
            let set = path.individualizable_set_at(i).unwrap();
            let node_count = set.node_count().unwrap();
            assert_eq!(
                node_count,
                expected_codes.len(),
                "node count mismatch in set {i} of: {path_str}"
            );
            for (j, expected_code) in expected_codes.iter().enumerate() {
                let node = set.node_at(j).unwrap();
                assert_eq!(
                    node.code(),
                    *expected_code,
                    "code mismatch at set {i}, node {j} of: {path_str}"
                );
            }
        }
    }
}

#[test]
fn individualizable_sets_full_paths_match_short_paths() {
    let doc: Value = serde_json::from_str(INDIVIDUALIZABLE_SETS_JSON).expect("valid JSON");
    let vis = Vis::instance();

    for item in doc.as_array().expect("root array") {
        if item["isFullPath"].as_bool().unwrap() || item["expected"].is_null() {
            continue;
        }

        let path_str = item["path"].as_str().unwrap();
        let version_str = item["visVersion"].as_str().unwrap();
        let version = VisVersion::from_str(version_str)
            .unwrap_or_else(|_| panic!("unknown version: {version_str}"));

        let gmod = vis.gmod(version).expect("version should be valid");
        let locations = vis.locations(version).expect("version should be valid");

        let short = GmodPath::from_short_path(path_str, gmod, locations)
            .unwrap_or_else(|| panic!("short path should parse: {path_str}"));
        let full = GmodPath::from_full_path(&short.to_full_path_string(), gmod, locations)
            .unwrap_or_else(|| panic!("full path should parse from: {path_str}"));

        assert_eq!(
            short.individualizable_set_count(),
            full.individualizable_set_count(),
            "set count mismatch between short/full for: {path_str}"
        );

        for i in 0..short.individualizable_set_count() {
            let s_set = short.individualizable_set_at(i).unwrap();
            let f_set = full.individualizable_set_at(i).unwrap();
            let s_count = s_set.node_count().unwrap();
            let f_count = f_set.node_count().unwrap();
            assert_eq!(
                s_count, f_count,
                "node count mismatch in set {i} for: {path_str}"
            );
            for j in 0..s_count {
                assert_eq!(
                    s_set.node_at(j).unwrap().code(),
                    f_set.node_at(j).unwrap().code(),
                    "code mismatch at set {i}, node {j} for: {path_str}"
                );
            }
        }
    }
}

#[test]
fn valid_paths_individualizable_sets_have_unique_codes() {
    let doc: Value = serde_json::from_str(GMOD_PATHS_JSON).expect("valid JSON");
    let vis = Vis::instance();

    for item in doc["Valid"].as_array().expect("Valid array") {
        let path_str = item["path"].as_str().unwrap();
        let version_str = item["visVersion"].as_str().unwrap();
        let version = VisVersion::from_str(version_str)
            .unwrap_or_else(|_| panic!("unknown version: {version_str}"));

        let gmod = vis.gmod(version).expect("version should be valid");
        let locations = vis.locations(version).expect("version should be valid");

        let path = GmodPath::from_short_path(path_str, gmod, locations)
            .unwrap_or_else(|| panic!("should parse: {path_str}"));

        let mut seen = std::collections::HashSet::new();
        for i in 0..path.individualizable_set_count() {
            let set = path.individualizable_set_at(i).unwrap();
            for j in 0..set.node_count().unwrap() {
                let code = set.node_at(j).unwrap().code().to_string();
                assert!(
                    seen.insert(code.clone()),
                    "duplicate code {code} in individualizable sets of: {path_str}"
                );
            }
        }
    }
}

#[test]
fn parents_iterator_excludes_target_node() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let path =
        GmodPath::from_short_path("411.1/C101.72/I101", gmod, locations).expect("should parse");

    let parents: Vec<&str> = path.parents().map(|n| n.code()).collect();
    assert_eq!(parents.len(), path.length() - 1);
    assert_eq!(parents.last().copied(), Some("C101.72"));
    assert!(!parents.contains(&"I101"));
}

#[test]
fn is_mappable_and_is_individualizable() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let path =
        GmodPath::from_short_path("411.1/C101.72/I101", gmod, locations).expect("should parse");

    assert!(path.is_mappable());
    assert!(path.is_individualizable());
}

#[test]
fn without_locations_strips_location_tags() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let path = GmodPath::from_short_path("411.1-1/C101.72/I101", gmod, locations);
    if let Some(path) = path {
        let stripped = path.without_locations();
        assert!(stripped.length() <= path.length());
        for node in stripped.iter() {
            assert!(
                node.location().is_none(),
                "node {} should have no location after without_locations",
                node.code()
            );
        }
    } else {
        let path =
            GmodPath::from_short_path("411.1/C101.72/I101", gmod, locations).expect("should parse");
        let stripped = path.without_locations();
        assert_eq!(path.to_string(), stripped.to_string());
    }
}

#[test]
fn common_names_non_empty_on_known_path() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let path =
        GmodPath::from_short_path("411.1/C101.72/I101", gmod, locations).expect("should parse");

    let names: Vec<(usize, String)> = path.common_names().collect();
    assert!(
        !names.is_empty(),
        "path should have at least one common name"
    );
    for (depth, name) in &names {
        assert!(*depth < path.length(), "depth must be within path length");
        assert!(!name.is_empty(), "common name must not be empty");
    }
}

#[test]
fn from_short_path_with_errors_valid_has_no_errors() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let (path, errors) =
        GmodPath::from_short_path_with_errors("411.1/C101.72/I101", gmod, locations);
    assert!(path.is_some());
    assert!(!errors.has_errors());
}

#[test]
fn from_short_path_with_errors_invalid_returns_errors() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let (path, errors) = GmodPath::from_short_path_with_errors("INVALID/PATH", gmod, locations);
    assert!(path.is_none());
    assert!(errors.has_errors());
}

#[test]
fn from_full_path_with_errors_valid_has_no_errors() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let (path, errors) = GmodPath::from_full_path_with_errors(
        "VE/400a/410/411/411i/411.1/CS1/C101/C101.7/C101.72/I101",
        gmod,
        locations,
    );
    assert!(path.is_some());
    assert!(!errors.has_errors());
}
