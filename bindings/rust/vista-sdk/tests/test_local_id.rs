use vista_sdk::*;

#[test]
fn build_with_primary_item_and_metadata_tag_succeeds() {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");
    let codebooks = vis
        .codebooks(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");

    let primary_item =
        GmodPath::from_short_path("411.1/C101.31-2", gmod, locations).expect("valid short path");
    let qty_tag = codebooks[CodebookName::Quantity]
        .create_tag("temperature")
        .expect("valid quantity value");

    let local_id = LocalIdBuilder::create(VisVersion::V3_4a)
        .with_primary_item(&primary_item)
        .with_metadata_tag(&qty_tag)
        .build()
        .expect("builder with primary item and metadata tag should be valid");

    assert_eq!(
        local_id.to_string(),
        "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature"
    );
    assert_eq!(local_id.version(), VisVersion::V3_4a);
    assert_eq!(
        local_id.quantity().expect("quantity was set").value(),
        "temperature"
    );
}

#[test]
fn build_without_primary_item_returns_err() {
    let builder = LocalIdBuilder::create(VisVersion::V3_4a);

    let result = builder.build();
    assert!(result.is_err());
}

#[test]
fn from_string_round_trips_to_string() {
    let local_id_str = "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature";

    let local_id = LocalId::from_string(local_id_str).expect("valid LocalId string");
    assert_eq!(local_id.to_string(), local_id_str);
}

#[test]
fn from_string_invalid_returns_none() {
    let local_id = LocalId::from_string("not a local id");
    assert!(local_id.is_none());
}

#[test]
fn from_string_with_errors_reports_errors_on_failure() {
    let (local_id, errors) = LocalId::from_string_with_errors("");

    assert!(local_id.is_none());
    assert!(errors.has_errors());
}

#[test]
fn from_string_with_errors_succeeds_without_errors() {
    let local_id_str = "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature";
    let (local_id, errors) = LocalId::from_string_with_errors(local_id_str);

    assert!(local_id.is_some());
    assert!(!errors.has_errors());
}

#[test]
fn secondary_item_is_none_when_not_set() {
    let local_id = LocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature")
        .expect("valid LocalId string");

    assert!(local_id.secondary_item().is_none());
}

#[test]
fn secondary_item_is_some_when_set() {
    let local_id = LocalId::from_string(
        "/dnv-v2/vis-3-4a/621.21/S90/sec/411.1/C101/meta/qty-mass/cnt-fuel.oil/pos-inlet",
    )
    .expect("valid LocalId string");

    assert!(local_id.secondary_item().is_some());
}

#[test]
fn metadata_tags_returns_all_set_tags() {
    let local_id = LocalId::from_string(
        "/dnv-v2/vis-3-4a/621.21/S90/sec/411.1/C101/meta/qty-mass/cnt-fuel.oil/pos-inlet",
    )
    .expect("valid LocalId string");

    let tags = local_id.metadata_tags();
    assert_eq!(tags.len(), 3);
}

#[test]
fn builder_modifying_via_without_and_with_metadata_tag() {
    let vis = Vis::instance();
    let codebooks = vis
        .codebooks(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");

    let original = LocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature")
        .expect("valid LocalId string");

    let pos_tag = codebooks[CodebookName::Position]
        .create_tag("outlet")
        .expect("valid position value");

    let modified = original
        .builder()
        .without_metadata_tag(CodebookName::Quantity)
        .with_metadata_tag(&pos_tag)
        .build()
        .expect("builder without quantity plus position should be valid");

    assert_eq!(
        modified.to_string(),
        "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/pos-outlet"
    );
}

#[test]
fn equality_matches_identical_local_ids() {
    let local_id1 = LocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature")
        .expect("valid LocalId string");
    let local_id2 = LocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature")
        .expect("valid LocalId string");
    let local_id3 = LocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-pressure")
        .expect("valid LocalId string");

    assert_eq!(*local_id1, *local_id2);
    assert_ne!(*local_id1, *local_id3);
}

#[test]
fn custom_metadata_tag_is_flagged_as_custom() {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");
    let codebooks = vis
        .codebooks(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");

    let primary_item =
        GmodPath::from_short_path("411.1/C101.31-2", gmod, locations).expect("valid short path");
    let custom_qty_tag = codebooks[CodebookName::Quantity]
        .create_tag("my_custom_measurement")
        .expect("valid custom value");

    let local_id = LocalIdBuilder::create(VisVersion::V3_4a)
        .with_primary_item(&primary_item)
        .with_metadata_tag(&custom_qty_tag)
        .build()
        .expect("builder with primary item and custom metadata tag should be valid");

    assert!(local_id.has_custom_tag());
    assert!(local_id.quantity().expect("quantity was set").is_custom());
}

#[test]
fn builder_is_valid_and_is_empty_reflect_state() {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");
    let codebooks = vis
        .codebooks(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");

    let primary_item =
        GmodPath::from_short_path("411.1/C101.31-2", gmod, locations).expect("valid short path");
    let qty_tag = codebooks[CodebookName::Quantity]
        .create_tag("temperature")
        .expect("valid quantity value");

    let empty_builder = LocalIdBuilder::create(VisVersion::V3_4a);
    assert!(!empty_builder.is_valid());
    assert!(empty_builder.is_empty());

    let incomplete_builder =
        LocalIdBuilder::create(VisVersion::V3_4a).with_primary_item(&primary_item);
    assert!(!incomplete_builder.is_valid());
    assert!(!incomplete_builder.is_empty());

    let valid_builder = LocalIdBuilder::create(VisVersion::V3_4a)
        .with_primary_item(&primary_item)
        .with_metadata_tag(&qty_tag);
    assert!(valid_builder.is_valid());
    assert!(!valid_builder.is_empty());
}

fn mqtt_builder(
    gmod: &vista_sdk::core::gmod::Gmod,
    locations: &vista_sdk::core::locations::Locations,
    codebooks: &vista_sdk::core::codebooks::Codebooks,
) -> LocalIdBuilder {
    let primary_item =
        GmodPath::from_short_path("411.1/C101.31-2", gmod, locations).expect("valid short path");
    let qty_tag = codebooks[CodebookName::Quantity]
        .create_tag("temperature")
        .expect("valid quantity value");
    let cnt_tag = codebooks[CodebookName::Content]
        .create_tag("exhaust.gas")
        .expect("valid content value");
    let pos_tag = codebooks[CodebookName::Position]
        .create_tag("inlet")
        .expect("valid position value");

    LocalIdBuilder::create(VisVersion::V3_4a)
        .with_primary_item(&primary_item)
        .with_metadata_tag(&qty_tag)
        .with_metadata_tag(&cnt_tag)
        .with_metadata_tag(&pos_tag)
}

#[test]
fn mqtt_local_id_to_string_uses_underscores_and_placeholders() {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");
    let codebooks = vis
        .codebooks(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");

    let builder = mqtt_builder(gmod, locations, codebooks);
    let mqtt_local_id =
        MqttLocalId::create(&builder).expect("valid builder should produce an MqttLocalId");

    assert_eq!(
        mqtt_local_id.to_string(),
        "dnv-v2/vis-3-4a/411.1_C101.31-2/_/qty-temperature/cnt-exhaust.gas/_/_/_/_/pos-inlet/_"
    );
}

#[test]
fn mqtt_local_id_has_no_leading_slash() {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");
    let codebooks = vis
        .codebooks(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");

    let builder = mqtt_builder(gmod, locations, codebooks);
    let mqtt_local_id =
        MqttLocalId::create(&builder).expect("valid builder should produce an MqttLocalId");

    assert!(!mqtt_local_id.to_string().starts_with('/'));
}

#[test]
fn mqtt_local_id_getters_reflect_builder_state() {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");
    let codebooks = vis
        .codebooks(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");

    let primary_item =
        GmodPath::from_short_path("411.1/C101.31-2", gmod, locations).expect("valid short path");
    let secondary_item =
        GmodPath::from_short_path("411.1/C101.31-5", gmod, locations).expect("valid short path");
    let qty_tag = codebooks[CodebookName::Quantity]
        .create_tag("temperature")
        .expect("valid quantity value");
    let cnt_tag = codebooks[CodebookName::Content]
        .create_tag("exhaust.gas")
        .expect("valid content value");

    let builder = LocalIdBuilder::create(VisVersion::V3_4a)
        .with_primary_item(&primary_item)
        .with_secondary_item(&secondary_item)
        .with_metadata_tag(&qty_tag)
        .with_metadata_tag(&cnt_tag);

    let mqtt_local_id =
        MqttLocalId::create(&builder).expect("valid builder should produce an MqttLocalId");

    assert_eq!(mqtt_local_id.version(), VisVersion::V3_4a);
    assert_eq!(mqtt_local_id.primary_item(), &*primary_item);
    assert_eq!(mqtt_local_id.secondary_item(), Some(&*secondary_item));
    assert_eq!(
        mqtt_local_id.quantity().expect("quantity was set").value(),
        "temperature"
    );
    assert_eq!(
        mqtt_local_id.content().expect("content was set").value(),
        "exhaust.gas"
    );
    assert!(mqtt_local_id.calculation().is_none());
    assert!(mqtt_local_id.state().is_none());
    assert!(mqtt_local_id.command().is_none());
    assert!(mqtt_local_id.r#type().is_none());
    assert!(mqtt_local_id.position().is_none());
    assert!(mqtt_local_id.detail().is_none());
}

#[test]
fn mqtt_local_id_builder_exposes_state_not_reflected_in_format() {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");
    let codebooks = vis
        .codebooks(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");

    let primary_item =
        GmodPath::from_short_path("411.1/C101.63/S206", gmod, locations).expect("valid short path");
    let qty_tag = codebooks[CodebookName::Quantity]
        .create_tag("temperature")
        .expect("valid quantity value");

    let builder = LocalIdBuilder::create(VisVersion::V3_4a)
        .with_verbose_mode(true)
        .with_primary_item(&primary_item)
        .with_metadata_tag(&qty_tag);

    let mqtt_local_id =
        MqttLocalId::create(&builder).expect("valid builder should produce an MqttLocalId");

    assert!(mqtt_local_id.builder().is_verbose_mode());
    assert!(!mqtt_local_id.builder().has_custom_tag());
    assert!(!mqtt_local_id.to_string().contains('~'));
}

#[test]
fn mqtt_local_id_equality() {
    let vis = Vis::instance();
    let gmod = vis
        .gmod(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");
    let locations = vis
        .locations(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");
    let codebooks = vis
        .codebooks(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");

    let primary_item =
        GmodPath::from_short_path("411.1/C101.31-2", gmod, locations).expect("valid short path");
    let qty_tag = codebooks[CodebookName::Quantity]
        .create_tag("temperature")
        .expect("valid quantity value");
    let pressure_tag = codebooks[CodebookName::Quantity]
        .create_tag("pressure")
        .expect("valid quantity value");

    let builder = LocalIdBuilder::create(VisVersion::V3_4a)
        .with_primary_item(&primary_item)
        .with_metadata_tag(&qty_tag);
    let other_builder = LocalIdBuilder::create(VisVersion::V3_4a)
        .with_primary_item(&primary_item)
        .with_metadata_tag(&pressure_tag);

    let a = MqttLocalId::create(&builder).expect("valid builder should produce an MqttLocalId");
    let b = MqttLocalId::create(&builder).expect("valid builder should produce an MqttLocalId");
    let c =
        MqttLocalId::create(&other_builder).expect("valid builder should produce an MqttLocalId");

    assert_eq!(a, b);
    assert_ne!(a, c);
}

#[test]
fn mqtt_local_id_create_from_invalid_builder_returns_none() {
    let builder = LocalIdBuilder::create(VisVersion::V3_4a);

    assert!(MqttLocalId::create(&builder).is_none());
}

#[test]
fn builder_version_reflects_vis_version() {
    let b = LocalIdBuilder::create(VisVersion::V3_4a);
    assert_eq!(b.version(), Some(VisVersion::V3_4a));
}

#[test]
fn builder_without_vis_version_clears_version() {
    let b = LocalIdBuilder::create(VisVersion::V3_4a).without_vis_version();
    assert_eq!(b.version(), None);
}

#[test]
fn builder_without_primary_item_clears_primary_item() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("gmod");
    let locations = vis.locations(VisVersion::V3_4a).expect("locations");
    let path = GmodPath::from_short_path("411.1/C101.31-2", gmod, locations).expect("valid");
    let b = LocalIdBuilder::create(VisVersion::V3_4a)
        .with_primary_item(&path)
        .without_primary_item();
    assert!(b.primary_item().is_none());
}

#[test]
fn builder_with_and_without_secondary_item() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("gmod");
    let locations = vis.locations(VisVersion::V3_4a).expect("locations");
    let primary = GmodPath::from_short_path("411.1/C101.31-2", gmod, locations).expect("valid");
    let secondary = GmodPath::from_short_path("411.1/C101.31-2", gmod, locations).expect("valid");
    let b = LocalIdBuilder::create(VisVersion::V3_4a)
        .with_primary_item(&primary)
        .with_secondary_item(&secondary);
    assert!(b.secondary_item().is_some());
    let b2 = b.without_secondary_item();
    assert!(b2.secondary_item().is_none());
}

#[test]
fn builder_without_metadata_tag_clears_tag() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("gmod");
    let locations = vis.locations(VisVersion::V3_4a).expect("locations");
    let codebooks = vis.codebooks(VisVersion::V3_4a).expect("codebooks");
    let path = GmodPath::from_short_path("411.1/C101.31-2", gmod, locations).expect("valid");
    let qty = codebooks[CodebookName::Quantity]
        .create_tag("temperature")
        .expect("valid");
    let b = LocalIdBuilder::create(VisVersion::V3_4a)
        .with_primary_item(&path)
        .with_metadata_tag(&qty)
        .without_metadata_tag(CodebookName::Quantity);
    assert!(b.quantity().is_none());
}

#[test]
fn builder_individual_metadata_tag_accessors() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("gmod");
    let locations = vis.locations(VisVersion::V3_4a).expect("locations");
    let codebooks = vis.codebooks(VisVersion::V3_4a).expect("codebooks");
    let path = GmodPath::from_short_path("411.1/C101.31-2", gmod, locations).expect("valid");
    let qty = codebooks[CodebookName::Quantity]
        .create_tag("temperature")
        .expect("qty");
    let cnt = codebooks[CodebookName::Content]
        .create_tag("exhaust.gas")
        .expect("cnt");
    let pos = codebooks[CodebookName::Position]
        .create_tag("inlet")
        .expect("pos");
    let b = LocalIdBuilder::create(VisVersion::V3_4a)
        .with_primary_item(&path)
        .with_metadata_tag(&qty)
        .with_metadata_tag(&cnt)
        .with_metadata_tag(&pos);
    assert!(b.quantity().is_some());
    assert!(b.content().is_some());
    assert!(b.position().is_some());
    assert!(b.calculation().is_none());
    assert!(b.state().is_none());
    assert!(b.command().is_none());
    assert!(b.detail().is_none());
}

#[test]
fn builder_verbose_mode_roundtrip() {
    let b = LocalIdBuilder::create(VisVersion::V3_4a).with_verbose_mode(true);
    assert!(b.is_verbose_mode());
    let b2 = b.with_verbose_mode(false);
    assert!(!b2.is_verbose_mode());
}

#[test]
fn builder_is_empty_metadata_reflects_state() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("gmod");
    let locations = vis.locations(VisVersion::V3_4a).expect("locations");
    let codebooks = vis.codebooks(VisVersion::V3_4a).expect("codebooks");
    let path = GmodPath::from_short_path("411.1/C101.31-2", gmod, locations).expect("valid");
    let qty = codebooks[CodebookName::Quantity]
        .create_tag("temperature")
        .expect("valid");
    let empty = LocalIdBuilder::create(VisVersion::V3_4a).with_primary_item(&path);
    assert!(empty.is_empty_metadata());
    let with_tag = empty.with_metadata_tag(&qty);
    assert!(!with_tag.is_empty_metadata());
}

#[test]
fn builder_has_custom_tag_false_for_standard_value() {
    let vis = Vis::instance();
    let gmod = vis.gmod(VisVersion::V3_4a).expect("gmod");
    let locations = vis.locations(VisVersion::V3_4a).expect("locations");
    let codebooks = vis.codebooks(VisVersion::V3_4a).expect("codebooks");
    let path = GmodPath::from_short_path("411.1/C101.31-2", gmod, locations).expect("valid");
    let qty = codebooks[CodebookName::Quantity]
        .create_tag("temperature")
        .expect("valid");
    let b = LocalIdBuilder::create(VisVersion::V3_4a)
        .with_primary_item(&path)
        .with_metadata_tag(&qty);
    assert!(!b.has_custom_tag());
}
