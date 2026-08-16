use vista_sdk::core::location_builder::LocationBuilder;
use vista_sdk::core::location_group::LocationGroup;
use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;

#[test]
fn create_returns_a_builder() {
    let vis = Vis::instance();
    let locations = vis
        .locations(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");

    let _builder = LocationBuilder::create(locations);
}

#[test]
fn fluent_chain_builds_matching_location() {
    let vis = Vis::instance();
    let locations = vis
        .locations(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");

    let builder = LocationBuilder::create(locations)
        .with_number(11)
        .expect("11 should be a valid number")
        .with_side('P')
        .expect("P should be a valid side")
        .with_transverse('I')
        .expect("I should be a valid transverse")
        .with_longitudinal('F')
        .expect("F should be a valid longitudinal")
        .with_vertical('U')
        .expect("U should be a valid vertical");

    let location = builder.build();
    assert_eq!(location.value(), "11FIPU");
}

#[test]
fn with_number_is_immutable() {
    let vis = Vis::instance();
    let locations = vis
        .locations(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");

    let base = LocationBuilder::create(locations);
    assert_eq!(base.to_string(), "");

    let _with_num = base.with_number(11);
}

#[test]
fn with_side_invalid_character_returns_err() {
    let vis = Vis::instance();
    let locations = vis
        .locations(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");
    let builder = LocationBuilder::create(locations);

    let result = builder.with_side('A');
    assert!(result.is_err());
}

#[test]
fn with_number_invalid_number_returns_err() {
    let vis = Vis::instance();
    let locations = vis
        .locations(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");
    let builder = LocationBuilder::create(locations);

    let result = builder.with_number(0);
    assert!(result.is_err());
}

#[test]
fn without_number_clears_the_number_component() {
    let vis = Vis::instance();
    let locations = vis
        .locations(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");

    let builder = LocationBuilder::create(locations)
        .with_number(11)
        .expect("11 should be a valid number")
        .without_number();

    assert_eq!(builder.to_string(), "");
}

#[test]
fn with_code_auto_detects_group_and_matches_with_side() {
    let vis = Vis::instance();
    let locations = vis
        .locations(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");

    let via_code = LocationBuilder::create(locations)
        .with_code('P')
        .expect("P should route to a group");
    let via_side = LocationBuilder::create(locations)
        .with_side('P')
        .expect("P should be a valid side");

    assert_eq!(via_code.to_string(), via_side.to_string());
}

#[test]
fn with_location_reconstructs_matching_builder() {
    let vis = Vis::instance();
    let locations = vis
        .locations(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");
    let location = locations
        .parse("11FIPU")
        .expect("11FIPU should be a valid location");

    let builder = LocationBuilder::create(locations).with_location(&location);

    assert_eq!(builder.to_string(), "11FIPU");
}

#[test]
fn without_value_clears_the_targeted_component() {
    let vis = Vis::instance();
    let locations = vis
        .locations(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");
    let location = locations
        .parse("11FIPU")
        .expect("11FIPU should be a valid location");

    let full = LocationBuilder::create(locations).with_location(&location);
    let cleared = full.without_value(LocationGroup::Side);

    assert_eq!(cleared.side(), None);
}

#[test]
fn component_getters_reflect_the_fluent_chain() {
    let vis = Vis::instance();
    let locations = vis
        .locations(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");

    let builder = LocationBuilder::create(locations)
        .with_number(11)
        .expect("11 should be a valid number")
        .with_side('P')
        .expect("P should be a valid side")
        .with_transverse('I')
        .expect("I should be a valid transverse")
        .with_longitudinal('F')
        .expect("F should be a valid longitudinal")
        .with_vertical('U')
        .expect("U should be a valid vertical");

    assert_eq!(builder.number(), Some(11));
    assert_eq!(builder.side(), Some('P'));
    assert_eq!(builder.transverse(), Some('I'));
    assert_eq!(builder.longitudinal(), Some('F'));
    assert_eq!(builder.vertical(), Some('U'));
}

#[test]
fn component_getters_unset_components_return_none() {
    let vis = Vis::instance();
    let locations = vis
        .locations(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");
    let builder = LocationBuilder::create(locations);

    assert_eq!(builder.number(), None);
    assert_eq!(builder.side(), None);
    assert_eq!(builder.transverse(), None);
    assert_eq!(builder.longitudinal(), None);
    assert_eq!(builder.vertical(), None);
}

#[test]
fn version_matches_the_requested_vis_version() {
    let vis = Vis::instance();
    let locations = vis
        .locations(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");
    let builder = LocationBuilder::create(locations);

    assert_eq!(builder.version(), VisVersion::V3_11a);
}
