use vista_sdk::core::location_group::LocationGroup;
use vista_sdk::core::vis::Vis;
use vista_sdk::core::vis_version::VisVersion;

#[test]
fn version_matches_the_requested_vis_version() {
    let vis = Vis::instance();
    let locations = vis
        .locations(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");

    assert_eq!(locations.version(), VisVersion::V3_11a);
}

#[test]
fn parse_valid_string_returns_matching_location() {
    let vis = Vis::instance();
    let locations = vis
        .locations(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");

    let location = locations
        .parse("11FIPU")
        .expect("11FIPU should be a valid location");
    assert_eq!(location.value(), "11FIPU");
}

#[test]
fn parse_invalid_string_returns_none() {
    let vis = Vis::instance();
    let locations = vis
        .locations(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");

    assert!(locations.parse("not-a-location").is_none());
}

#[test]
fn parse_with_errors_valid_string_yields_empty_errors() {
    let vis = Vis::instance();
    let locations = vis
        .locations(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");

    let (location, errors) = locations.parse_with_errors("11FIPU");
    assert!(location.is_some());
    assert!(!errors.has_errors());
}

#[test]
fn parse_with_errors_invalid_string_yields_non_empty_errors() {
    let vis = Vis::instance();
    let locations = vis
        .locations(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");

    let (location, errors) = locations.parse_with_errors("not-a-location");
    assert!(location.is_none());
    assert!(errors.has_errors());
    assert!(errors.count() > 0);

    let first = errors.get(0).expect("first error entry should exist");
    assert!(!first.r#type.is_empty());
}

#[test]
fn relative_location_count_and_at_cover_all_entries() {
    let vis = Vis::instance();
    let locations = vis
        .locations(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");

    let count = locations.relative_location_count();
    assert!(count > 0);

    let first = locations
        .relative_location_at(0)
        .expect("first relative location should exist");
    assert_ne!(first.code(), '\0');

    assert_eq!(locations.relative_locations().count(), count);
}

#[test]
fn relative_location_at_out_of_range_returns_none() {
    let vis = Vis::instance();
    let locations = vis
        .locations(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");
    let count = locations.relative_location_count();

    assert!(locations.relative_location_at(count).is_none());
}

#[test]
fn group_count_and_at_side_group_has_entries() {
    let vis = Vis::instance();
    let locations = vis
        .locations(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");

    let count = locations.group_count(LocationGroup::Side);
    assert!(count > 0);

    let first = locations.group_at(LocationGroup::Side, 0);
    assert!(first.is_some());

    assert_eq!(locations.group(LocationGroup::Side).count(), count);
}

#[test]
fn group_at_out_of_range_returns_none() {
    let vis = Vis::instance();
    let locations = vis
        .locations(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");
    let count = locations.group_count(LocationGroup::Side);

    assert!(locations.group_at(LocationGroup::Side, count).is_none());
}

#[test]
fn relative_location_accessors_reflect_a_real_entry() {
    let vis = Vis::instance();
    let locations = vis
        .locations(VisVersion::V3_11a)
        .expect("V3_11a should be a valid VIS version");
    let entry = locations
        .relative_location_at(0)
        .expect("first relative location should exist");

    assert_ne!(entry.code(), '\0');
    assert!(!entry.name().is_empty());
    assert!(!entry.location_value().is_empty());
}
