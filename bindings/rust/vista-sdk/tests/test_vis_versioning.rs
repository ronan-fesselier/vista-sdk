use vista_sdk::core::gmod_path::OwnedGmodPath;
use vista_sdk::core::local_id::OwnedLocalId;
use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;

#[test]
fn convert_path_across_versions() {
    let cases = [
        ("411.1/C101.72/I101", "411.1/C101.72/I101"),
        ("323.51/H362.1", "323.61/H362.1"),
        ("321.38/C906", "321.39/C906"),
        ("511.331/C221", "511.31/C121.31/C221"),
        (
            "1012.21/C1147.221/C1051.7/C101.22",
            "1012.21/C1147.221/C1051.7/C101.93",
        ),
        (
            "1012.21/C1147.221/C1051.7/C101.61/S203.6",
            "1012.21/C1147.221/C1051.7/C101.311/C467.5",
        ),
        ("001", "001"),
        (
            "1012.21/C1147.221/C1051.7/C101.661i/C624",
            "1012.21/C1147.221/C1051.7/C101.661i/C621",
        ),
    ];

    let vis = Vis::instance();
    let source_version = VisVersion::V3_4a;
    let target_version = VisVersion::V3_6a;

    let source_gmod = vis.gmod(source_version).expect("V3_4a should be valid");
    let source_locations = vis
        .locations(source_version)
        .expect("V3_4a should be valid");

    for (input_path, expected_path) in cases {
        let source_path = OwnedGmodPath::from_short_path(input_path, source_gmod, source_locations)
            .unwrap_or_else(|| panic!("should parse: {input_path}"));

        let converted = vis
            .convert_path(source_version, &source_path, target_version)
            .unwrap_or_else(|_| panic!("should convert: {input_path}"));

        assert_eq!(converted.to_string(), expected_path);
    }
}

#[test]
fn convert_path_out_of_scope_returns_err() {
    let vis = Vis::instance();
    let source_version = VisVersion::V3_7a;
    let target_version = VisVersion::V3_8a;

    let gmod = vis.gmod(source_version).expect("V3_7a should be valid");
    let locations = vis
        .locations(source_version)
        .expect("V3_7a should be valid");

    let source_path = OwnedGmodPath::from_short_path("244.1i/H101.111/H401", gmod, locations)
        .expect("should parse");

    let result = vis.convert_path(source_version, &source_path, target_version);
    assert!(result.is_err());
}

#[test]
fn convert_node_across_versions() {
    let cases = [
        ("1014.211", "1014.211"),
        ("323.5", "323.6"),
        ("412.72", "412.7i"),
        ("323.4", "323.5"),
        ("C101.212", "C101.22"),
    ];

    let vis = Vis::instance();
    let source_version = VisVersion::V3_4a;
    let target_version = VisVersion::V3_6a;

    let source_gmod = vis.gmod(source_version).expect("V3_4a should be valid");
    let target_gmod = vis.gmod(target_version).expect("V3_6a should be valid");

    for (input_code, expected_code) in cases {
        let source_node = source_gmod
            .get_node(input_code)
            .unwrap_or_else(|_| panic!("should exist: {input_code}"));
        let expected_node = target_gmod
            .get_node(expected_code)
            .unwrap_or_else(|_| panic!("should exist: {expected_code}"));

        let converted = vis
            .convert_node(source_version, source_node, target_version)
            .unwrap_or_else(|_| panic!("should convert: {input_code}"));

        assert_eq!(converted.code(), expected_node.code());
    }
}

#[test]
fn convert_local_id_across_versions() {
    let cases = [
        (
            "/dnv-v2/vis-3-4a/411.1/C101/sec/411.1/C101.64i/S201/meta/cnt-condensate",
            "/dnv-v2/vis-3-5a/411.1/C101/sec/411.1/C101.64/S201/meta/cnt-condensate",
        ),
        (
            "/dnv-v2/vis-3-4a/411.1/C101.64i-1/S201.1/C151.2/S110/meta/cnt-hydraulic.oil/state-running",
            "/dnv-v2/vis-3-9a/411.1/C101.64-1/S201.1/C151.2/S110/meta/cnt-hydraulic.oil/state-running",
        ),
    ];

    let vis = Vis::instance();

    for (source_str, target_str) in cases {
        let source_local_id =
            OwnedLocalId::from_string(source_str).expect("should parse source LocalId");
        let target_local_id =
            OwnedLocalId::from_string(target_str).expect("should parse target LocalId");

        let converted = vis
            .convert_local_id(&source_local_id, target_local_id.version())
            .expect("should convert LocalId");

        assert_eq!(converted.to_string(), target_str);
    }
}

#[test]
fn convert_local_id_builder_across_versions() {
    let vis = Vis::instance();

    let source_local_id = OwnedLocalId::from_string(
        "/dnv-v2/vis-3-4a/411.1/C101/sec/411.1/C101.64i/S201/meta/cnt-condensate",
    )
    .expect("should parse source LocalId");

    let converted_builder = vis
        .convert_local_id_builder(source_local_id.builder(), VisVersion::V3_5a)
        .expect("should convert LocalIdBuilder");

    assert_eq!(
        converted_builder.to_string(),
        "/dnv-v2/vis-3-5a/411.1/C101/sec/411.1/C101.64/S201/meta/cnt-condensate"
    );
}
