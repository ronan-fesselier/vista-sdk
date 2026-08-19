use vista_sdk::core::imo_number::ImoNumber;
use vista_sdk::core::local_id::OwnedLocalId;
use vista_sdk::core::universal_id::{OwnedUniversalId, UniversalId};
use vista_sdk::core::universal_id_builder::{OwnedUniversalIdBuilder, UniversalIdBuilder};
use vista_sdk::core::vis_version::VisVersion;

const VALID_UNIVERSAL_ID_STR: &str =
    "data.dnv.com/IMO1234567/dnv-v2/vis-3-4a/621.21/S90/sec/411.1/C101/meta/qty-mass/cnt-fuel.oil/pos-inlet";
const VALID_LOCAL_ID_STR: &str =
    "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet";

#[test]
fn naming_entity_is_data_dnv_com() {
    assert_eq!(UniversalId::naming_entity(), "data.dnv.com");
}

#[test]
fn from_string_valid_returns_some() {
    let universal_id = OwnedUniversalId::from_string(VALID_UNIVERSAL_ID_STR);
    assert!(universal_id.is_some());
}

#[test]
fn from_string_invalid_returns_none() {
    let universal_id = OwnedUniversalId::from_string("not-a-universal-id");
    assert!(universal_id.is_none());
}

#[test]
fn to_string_round_trips_input() {
    let universal_id =
        OwnedUniversalId::from_string(VALID_UNIVERSAL_ID_STR).expect("valid UniversalId string");
    assert_eq!(universal_id.to_string(), VALID_UNIVERSAL_ID_STR);
}

#[test]
fn local_id_is_non_null() {
    let universal_id =
        OwnedUniversalId::from_string(VALID_UNIVERSAL_ID_STR).expect("valid UniversalId string");
    let _ = universal_id.local_id();
}

#[test]
fn builder_is_valid() {
    let universal_id =
        OwnedUniversalId::from_string(VALID_UNIVERSAL_ID_STR).expect("valid UniversalId string");
    assert!(universal_id.builder().is_valid());
}

#[test]
fn imo_number_matches_parsed_value() {
    let universal_id =
        OwnedUniversalId::from_string(VALID_UNIVERSAL_ID_STR).expect("valid UniversalId string");
    let imo = universal_id.imo_number();
    assert_eq!(imo.to_string(), "IMO1234567");
}

#[test]
fn equality_matches_identical_universal_ids() {
    let a =
        OwnedUniversalId::from_string(VALID_UNIVERSAL_ID_STR).expect("valid UniversalId string");
    let b =
        OwnedUniversalId::from_string(VALID_UNIVERSAL_ID_STR).expect("valid UniversalId string");
    assert_eq!(a, b);
}

#[test]
fn builder_naming_entity_is_data_dnv_com() {
    assert_eq!(UniversalIdBuilder::naming_entity(), "data.dnv.com");
}

#[test]
fn builder_create_valid_version_returns_invalid_state() {
    let builder = OwnedUniversalIdBuilder::create(VisVersion::V3_4a);
    assert!(!builder.is_valid());
}

#[test]
fn builder_build_invalid_returns_err() {
    let builder = OwnedUniversalIdBuilder::create(VisVersion::V3_4a);
    assert!(builder.build().is_err());
}

#[test]
fn builder_fluent_chain_builds_valid_universal_id() {
    let local_id = OwnedLocalId::from_string(VALID_LOCAL_ID_STR).expect("valid LocalId string");
    let imo = ImoNumber::create(9074729).expect("valid IMO number");

    let universal_id = OwnedUniversalIdBuilder::create(VisVersion::V3_4a)
        .with_imo_number(&imo)
        .with_local_id(local_id.builder())
        .build()
        .expect("builder with IMO and local id should be valid");

    assert!(universal_id.builder().is_valid());
}

#[test]
fn builder_without_imo_number_clears_imo() {
    let local_id = OwnedLocalId::from_string(VALID_LOCAL_ID_STR).expect("valid LocalId string");
    let imo = ImoNumber::create(9074729).expect("valid IMO number");

    let builder = OwnedUniversalIdBuilder::create(VisVersion::V3_4a)
        .with_imo_number(&imo)
        .with_local_id(local_id.builder());

    let without_imo = builder.without_imo_number();
    assert!(without_imo.imo_number().is_none());
    assert!(!without_imo.is_valid());
}

#[test]
fn builder_without_local_id_clears_local_id() {
    let local_id = OwnedLocalId::from_string(VALID_LOCAL_ID_STR).expect("valid LocalId string");
    let imo = ImoNumber::create(9074729).expect("valid IMO number");

    let builder = OwnedUniversalIdBuilder::create(VisVersion::V3_4a)
        .with_imo_number(&imo)
        .with_local_id(local_id.builder());

    let without_local_id = builder.without_local_id();
    assert!(without_local_id.local_id().is_none());
    assert!(!without_local_id.is_valid());
}

#[test]
fn builder_from_string_valid_returns_some() {
    let universal_id = OwnedUniversalIdBuilder::from_string(VALID_UNIVERSAL_ID_STR);
    assert!(universal_id.is_some());
}

#[test]
fn builder_from_string_invalid_returns_none() {
    let universal_id = OwnedUniversalIdBuilder::from_string("not-a-universal-id");
    assert!(universal_id.is_none());
}

#[test]
fn builder_from_string_round_trips_to_string() {
    let universal_id = OwnedUniversalIdBuilder::from_string(VALID_UNIVERSAL_ID_STR)
        .expect("valid UniversalId string");
    assert_eq!(universal_id.to_string(), VALID_UNIVERSAL_ID_STR);
}

#[test]
fn builder_from_string_with_errors_invalid_has_errors() {
    let (universal_id, errors) =
        OwnedUniversalIdBuilder::from_string_with_errors("not-a-universal-id");
    assert!(universal_id.is_none());
    assert!(errors.has_errors());
}

#[test]
fn builder_from_string_with_errors_valid_has_no_errors() {
    let (universal_id, errors) =
        OwnedUniversalIdBuilder::from_string_with_errors(VALID_UNIVERSAL_ID_STR);
    assert!(universal_id.is_some());
    assert!(!errors.has_errors());
}
