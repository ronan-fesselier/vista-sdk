use vista_sdk::*;

#[test]
fn to_string_returns_formatted_tag_without_trailing_separator() {
    let vis = Vis::instance();
    let codebooks = vis
        .codebooks(VisVersion::V3_4a)
        .expect("V3_4a should be a valid VIS version");

    let quantity_tag = codebooks[CodebookName::Quantity].create_tag("temperature");
    assert!(quantity_tag.is_some());
    assert_eq!(quantity_tag.unwrap().to_string(), "qty-temperature");

    let content_tag = codebooks[CodebookName::Content].create_tag("exhaust.gas");
    assert!(content_tag.is_some());
    assert_eq!(content_tag.unwrap().to_string(), "cnt-exhaust.gas");

    let position_tag = codebooks[CodebookName::Position].create_tag("inlet");
    assert!(position_tag.is_some());
    assert_eq!(position_tag.unwrap().to_string(), "pos-inlet");
}

#[test]
fn name_returns_correct_codebook_name() {
    let vis = Vis::instance();
    let cbs = vis.codebooks(VisVersion::V3_4a).expect("V3_4a");
    assert_eq!(cbs[CodebookName::Quantity].name(), CodebookName::Quantity);
    assert_eq!(cbs[CodebookName::Position].name(), CodebookName::Position);
    assert_eq!(cbs[CodebookName::Detail].name(), CodebookName::Detail);
}

#[test]
fn standard_values_count_is_positive() {
    let vis = Vis::instance();
    let cbs = vis.codebooks(VisVersion::V3_4a).expect("V3_4a");
    assert!(cbs[CodebookName::Quantity].standard_values_count() > 0);
}

#[test]
fn standard_value_at_returns_value_and_none_out_of_range() {
    let vis = Vis::instance();
    let cbs = vis.codebooks(VisVersion::V3_4a).expect("V3_4a");
    let cb = &cbs[CodebookName::Quantity];
    let count = cb.standard_values_count();
    assert!(count > 0);
    assert!(cb.standard_value_at(0).is_some());
    assert!(cb.standard_value_at(count - 1).is_some());
    assert!(cb.standard_value_at(count).is_none());
}

#[test]
fn groups_count_is_positive() {
    let vis = Vis::instance();
    let cbs = vis.codebooks(VisVersion::V3_4a).expect("V3_4a");
    assert!(cbs[CodebookName::Position].groups_count() > 0);
}

#[test]
fn group_at_returns_group_and_none_out_of_range() {
    let vis = Vis::instance();
    let cbs = vis.codebooks(VisVersion::V3_4a).expect("V3_4a");
    let cb = &cbs[CodebookName::Position];
    let count = cb.groups_count();
    assert!(count > 0);
    assert!(cb.group_at(0).is_some());
    assert!(cb.group_at(count).is_none());
}

#[test]
fn has_group_known_and_unknown() {
    let vis = Vis::instance();
    let cbs = vis.codebooks(VisVersion::V3_4a).expect("V3_4a");
    let cb = &cbs[CodebookName::Position];
    let group = cb.group_at(0).expect("at least one group");
    assert!(cb.has_group(group));
    assert!(!cb.has_group("__nonexistent_group__"));
}

#[test]
fn validate_position_valid_standard() {
    let vis = Vis::instance();
    let cbs = vis.codebooks(VisVersion::V3_4a).expect("V3_4a");
    assert_eq!(
        cbs[CodebookName::Position].validate_position("centre"),
        PositionValidationResult::Valid
    );
}

#[test]
fn validate_position_custom() {
    let vis = Vis::instance();
    let cbs = vis.codebooks(VisVersion::V3_4a).expect("V3_4a");
    assert_eq!(
        cbs[CodebookName::Position].validate_position("customvalue"),
        PositionValidationResult::Custom
    );
}

#[test]
fn validate_position_invalid() {
    let vis = Vis::instance();
    let cbs = vis.codebooks(VisVersion::V3_4a).expect("V3_4a");
    assert_eq!(
        cbs[CodebookName::Position].validate_position("INVALID VALUE WITH SPACES"),
        PositionValidationResult::Invalid
    );
}

#[test]
fn has_standard_value_false_for_unknown() {
    let vis = Vis::instance();
    let cbs = vis.codebooks(VisVersion::V3_4a).expect("V3_4a");
    assert!(!cbs[CodebookName::Quantity].has_standard_value("__nonexistent__"));
}

#[test]
fn create_tag_non_uri_reserved_returns_none() {
    let vis = Vis::instance();
    let cbs = vis.codebooks(VisVersion::V3_4a).expect("V3_4a");
    assert!(cbs[CodebookName::Quantity]
        .create_tag("invalid value with spaces")
        .is_none());
}

#[test]
fn create_tag_unknown_but_valid_value_returns_custom_tag() {
    let vis = Vis::instance();
    let cbs = vis.codebooks(VisVersion::V3_4a).expect("V3_4a");
    let tag = cbs[CodebookName::Quantity].create_tag("nonexistent");
    assert!(tag.is_some());
}
