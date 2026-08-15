use vista_sdk::core::codebook_name::CodebookName;
use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;

#[test]
fn loads_all_versions() {
    let vis = Vis::instance();

    for version in vis.versions() {
        let codebooks = vis
            .codebooks(version)
            .expect("versions() only returns valid VIS versions");
        let _position = &codebooks[CodebookName::Position];
    }
}

#[test]
fn has_standard_value() {
    let vis = Vis::instance();
    let codebooks = vis
        .codebooks(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");

    assert!(codebooks[CodebookName::Position].has_standard_value("centre"));
}
