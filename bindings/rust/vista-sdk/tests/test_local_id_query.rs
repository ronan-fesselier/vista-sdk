use vista_sdk::core::codebook_name::CodebookName;
use vista_sdk::core::gmod_path::OwnedGmodPath;
use vista_sdk::core::local_id::OwnedLocalId;
use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;
use vista_sdk::query::gmod_path_query_builder::OwnedGmodPathQueryBuilder;
use vista_sdk::query::local_id_query_builder::OwnedLocalIdQueryBuilder;
use vista_sdk::query::metadata_tags_query_builder::OwnedMetadataTagsQueryBuilder;

#[test]
fn empty_query_matches_all_local_ids() {
    let query = OwnedLocalIdQueryBuilder::create().build();

    let local_id = OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power")
        .expect("valid LocalId string");
    assert!(query.match_local_id(&local_id));

    let local_id2 = OwnedLocalId::from_string(
        "/dnv-v2/vis-3-4a/652.31/S90.3/S61/sec/652.1i-1P/meta/cnt-sea.water/state-opened",
    )
    .expect("valid LocalId string");
    assert!(query.match_local_id(&local_id2));
}

#[test]
fn from_local_id_builds_exact_match_query() {
    let local_id = OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power")
        .expect("valid LocalId string");

    let query = OwnedLocalIdQueryBuilder::from_local_id(&local_id)
        .expect("builder should build")
        .build();
    assert!(query.match_local_id(&local_id));
}

#[test]
fn from_local_id_string_overload_with_without_locations() {
    let builder = OwnedLocalIdQueryBuilder::from_string(
        "/dnv-v2/vis-3-7a/511.11/C101/meta/qty-pressure/cnt-lubricating.oil",
    )
    .expect("valid LocalId string");

    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_7a).expect("V3_7a should be valid");
    let locations = vis
        .locations(VisVersion::V3_7a)
        .expect("V3_7a should be valid");
    let primary_path = OwnedGmodPath::from_short_path("511.11/C101", gmod, locations)
        .expect("511.11/C101 should parse");

    let primary_query = OwnedGmodPathQueryBuilder::from_path(&primary_path)
        .expect("path builder should build")
        .without_locations()
        .expect("without_locations should succeed")
        .build();

    let query = builder
        .with_primary_item_query(&primary_query)
        .expect("with_primary_item_query should succeed")
        .build();

    let other = OwnedLocalId::from_string(
        "/dnv-v2/vis-3-7a/511.11-1/C101/meta/qty-pressure/cnt-lubricating.oil",
    )
    .expect("valid LocalId string");
    assert!(query.match_local_id(&other));
}

#[test]
fn with_primary_item_match_primary_item_without_locations() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let path = OwnedGmodPath::from_short_path("411.1/C101.31", gmod, locations)
        .expect("411.1/C101.31 should parse");

    let primary_query = OwnedGmodPathQueryBuilder::from_path(&path)
        .expect("path builder should build")
        .without_locations()
        .expect("without_locations should succeed")
        .build();

    let query = OwnedLocalIdQueryBuilder::create()
        .with_primary_item_query(&primary_query)
        .expect("with_primary_item_query should succeed")
        .build();

    let local_id1 = OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power")
        .expect("valid LocalId string");
    assert!(query.match_local_id(&local_id1));

    let local_id2 = OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1-1/C101.31/meta/qty-power")
        .expect("valid LocalId string");
    assert!(query.match_local_id(&local_id2));

    let local_id3 = OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C102.31/meta/qty-power")
        .expect("valid LocalId string");
    assert!(!query.match_local_id(&local_id3));
}

#[test]
fn with_primary_item_no_match_with_different_location_when_locations_required() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let path = OwnedGmodPath::from_short_path("411.1/C101.31-1", gmod, locations)
        .expect("411.1/C101.31-1 should parse");

    let query = OwnedLocalIdQueryBuilder::create()
        .with_primary_item(&path)
        .expect("with_primary_item should succeed")
        .build();

    let local_id = OwnedLocalId::from_string(
        "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
    )
    .expect("valid LocalId string");
    assert!(!query.match_local_id(&local_id));
}

#[test]
fn with_primary_item_match_primary_with_different_location_using_without_locations() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let path = OwnedGmodPath::from_short_path("411.1-2/C101.63/S206", gmod, locations)
        .expect("411.1-2/C101.63/S206 should parse");

    let primary_query = OwnedGmodPathQueryBuilder::from_path(&path)
        .expect("path builder should build")
        .without_locations()
        .expect("without_locations should succeed")
        .build();

    let query = OwnedLocalIdQueryBuilder::create()
        .with_primary_item_query(&primary_query)
        .expect("with_primary_item_query should succeed")
        .build();

    let local_id = OwnedLocalId::from_string(
        "/dnv-v2/vis-3-4a/411.1-1/C101.63/S206/~propulsion.engine/~cooling.system/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
    )
    .expect("valid LocalId string");
    assert!(query.match_local_id(&local_id));
}

#[test]
fn with_primary_item_no_match_with_different_first_node() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let path =
        OwnedGmodPath::from_short_path("411.1", gmod, locations).expect("411.1 should parse");

    let primary_query = OwnedGmodPathQueryBuilder::from_path(&path)
        .expect("path builder should build")
        .without_locations()
        .expect("without_locations should succeed")
        .build();

    let query = OwnedLocalIdQueryBuilder::create()
        .with_primary_item_query(&primary_query)
        .expect("with_primary_item_query should succeed")
        .build();

    let local_id = OwnedLocalId::from_string(
        "/dnv-v2/vis-3-4a/511.11-21O/C101.67/S208/meta/qty-pressure/cnt-air/state-low",
    )
    .expect("valid LocalId string");
    assert!(!query.match_local_id(&local_id));
}

#[test]
fn with_primary_item_nodes_builder_match_by_node_code() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");

    let node_c101 = gmod.get_node("C101").expect("C101 should exist");

    let primary_query = OwnedGmodPathQueryBuilder::create()
        .with_node_all_locations(node_c101, true)
        .expect("with_node_all_locations should succeed")
        .build();

    let query = OwnedLocalIdQueryBuilder::create()
        .with_primary_item_query(&primary_query)
        .expect("with_primary_item_query should succeed")
        .build();

    let local_id1 = OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power")
        .expect("valid LocalId string");
    assert!(query.match_local_id(&local_id1));

    let local_id2 = OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C102.31/meta/qty-power")
        .expect("valid LocalId string");
    assert!(!query.match_local_id(&local_id2));
}

#[test]
fn with_primary_item_with_node_match_all_locations_via_path_configure() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_7a).expect("V3_7a should be valid");

    let node433 = gmod.get_node("433.1").expect("433.1 should exist");

    let local_id = OwnedLocalId::from_string(
        "/dnv-v2/vis-3-7a/433.1-S/C322.91/S205/meta/qty-conductivity/detail-relative",
    )
    .expect("valid LocalId string");

    let primary_query = OwnedGmodPathQueryBuilder::from_path(local_id.primary_item())
        .expect("path builder should build")
        .path_with_node_all_locations("433.1", true)
        .expect("path_with_node_all_locations should succeed")
        .build();

    let query = OwnedLocalIdQueryBuilder::from_local_id(&local_id)
        .expect("builder should build")
        .with_primary_item_query(&primary_query)
        .expect("with_primary_item_query should succeed")
        .build();

    assert!(query.match_local_id(&local_id));

    let _ = node433;
}

#[test]
fn with_secondary_item_match_secondary_item_without_locations() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("V3_4a should be valid");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let path = OwnedGmodPath::from_short_path("411.1/C101.31-2", gmod, locations)
        .expect("411.1/C101.31-2 should parse");

    let secondary_query = OwnedGmodPathQueryBuilder::from_path(&path)
        .expect("path builder should build")
        .without_locations()
        .expect("without_locations should succeed")
        .build();

    let query = OwnedLocalIdQueryBuilder::create()
        .with_secondary_item_query(&secondary_query)
        .expect("with_secondary_item_query should succeed")
        .build();

    let local_id = OwnedLocalId::from_string(
        "/dnv-v2/vis-3-4a/411.1/C101.63/S206/sec/411.1/C101.31-5/~propulsion.engine/~cooling.system/~for.propulsion.engine/~cylinder.5/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
    )
    .expect("valid LocalId string");
    assert!(query.match_local_id(&local_id));
}

#[test]
fn with_tags_match_by_content_tag() {
    let tags_query = OwnedMetadataTagsQueryBuilder::create()
        .with_tag(CodebookName::Content, "sea.water")
        .build();

    let query = OwnedLocalIdQueryBuilder::create()
        .with_tags(&tags_query)
        .expect("with_tags should succeed")
        .build();

    let local_id = OwnedLocalId::from_string(
        "/dnv-v2/vis-3-4a/652.31/S90.3/S61/sec/652.1i-1P/meta/cnt-sea.water/state-opened",
    )
    .expect("valid LocalId string");
    assert!(query.match_local_id(&local_id));
}

#[test]
fn with_tags_match_by_quantity_tag() {
    let vis = Vis::instance();
    let codebooks = vis
        .codebooks(VisVersion::V3_4a)
        .expect("V3_4a should be valid");

    let qty_tag = codebooks[CodebookName::Quantity]
        .create_tag("power")
        .expect("power should be a standard quantity");

    let tags_query = OwnedMetadataTagsQueryBuilder::create()
        .with_tag(CodebookName::Quantity, &qty_tag.value())
        .build();

    let query = OwnedLocalIdQueryBuilder::create()
        .with_tags(&tags_query)
        .expect("with_tags should succeed")
        .build();

    let local_id1 = OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power")
        .expect("valid LocalId string");
    assert!(query.match_local_id(&local_id1));

    let local_id2 =
        OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-temperature")
            .expect("valid LocalId string");
    assert!(!query.match_local_id(&local_id2));
}

#[test]
fn with_tags_metadata_tags_query_overload() {
    let tags_query = OwnedMetadataTagsQueryBuilder::create()
        .with_tag(CodebookName::Content, "sea.water")
        .build();

    let query = OwnedLocalIdQueryBuilder::create()
        .with_tags(&tags_query)
        .expect("with_tags should succeed")
        .build();

    let local_id = OwnedLocalId::from_string(
        "/dnv-v2/vis-3-4a/652.31/S90.3/S61/sec/652.1i-1P/meta/cnt-sea.water/state-opened",
    )
    .expect("valid LocalId string");
    assert!(query.match_local_id(&local_id));
}

#[test]
fn with_tags_chained_calls_replace_tags() {
    let query = OwnedLocalIdQueryBuilder::create()
        .with_tags(
            &OwnedMetadataTagsQueryBuilder::create()
                .with_tag(CodebookName::Quantity, "temperature")
                .build(),
        )
        .expect("with_tags should succeed")
        .with_tags(
            &OwnedMetadataTagsQueryBuilder::create()
                .with_tag(CodebookName::Content, "exhaust.gas")
                .build(),
        )
        .expect("with_tags should succeed")
        .build();

    let local_id_with_both =
        OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas")
            .expect("valid LocalId string");
    assert!(query.match_local_id(&local_id_with_both));

    let local_id_with_only_second =
        OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/meta/cnt-exhaust.gas")
            .expect("valid LocalId string");
    assert!(query.match_local_id(&local_id_with_only_second));

    let local_id_with_only_first =
        OwnedLocalId::from_string("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature")
            .expect("valid LocalId string");
    assert!(!query.match_local_id(&local_id_with_only_first));
}
