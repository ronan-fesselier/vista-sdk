use vista_sdk::core::error::last_error;
use vista_sdk::core::imo_number::ImoNumber;
use vista_sdk::core::local_id::OwnedLocalId;
use vista_sdk::transport::datachannel::data_channel::{
    ConfigurationReference, DataChannel, DataChannelId, DataChannelList, DataChannelListPackage,
    DataChannelType, Format, Header, NameObject, Package, Property, Range, Restriction, Unit,
    VersionInformation, WhiteSpace,
};
use vista_sdk::transport::serializable_document::SerializableDocument;
use vista_sdk::transport::ship_id::ShipId;
use vista_sdk::types::date_time_offset::DateTimeOffset;

const VALID_LOCAL_ID_STR: &str =
    "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet";

// restriction

#[test]
fn restriction_create_fresh_has_no_fields_set() {
    let r = Restriction::new();
    assert_eq!(r.fraction_digits(), None);
    assert_eq!(r.length(), None);
    assert_eq!(r.enumeration_count(), 0);
}

#[test]
fn restriction_set_enumeration_enumeration_at_round_trip() {
    let mut r = Restriction::new();
    r.set_enumeration(&["A", "B", "C"]);
    assert_eq!(r.enumeration_count(), 3);
    assert_eq!(r.enumeration_at(0).as_deref(), Some("A"));
    assert_eq!(r.enumeration_at(2).as_deref(), Some("C"));
}

#[test]
fn restriction_set_total_digits_zero_sets_last_error_and_leaves_unset() {
    let mut r = Restriction::new();
    r.set_total_digits(0);
    let err = last_error();
    assert!(!err.message.is_empty());
    assert_eq!(r.total_digits(), None);
}

#[test]
fn restriction_set_white_space_round_trip() {
    let mut r = Restriction::new();
    r.set_white_space(WhiteSpace::Collapse);
    assert_eq!(r.white_space(), Some(WhiteSpace::Collapse));
}

// range

#[test]
fn range_create_valid_bounds() {
    let range = Range::new(0.0, 100.0);
    assert!((range.low() - 0.0).abs() < f64::EPSILON);
    assert!((range.high() - 100.0).abs() < f64::EPSILON);
}

#[test]
fn range_create_low_ge_high_returns_err() {
    let result = Range::try_new(100.0, 0.0);
    assert!(result.is_err());
    let err = last_error();
    assert!(!err.message.is_empty());
}

#[test]
fn range_set_low_greater_than_high_sets_last_error_leaves_unchanged() {
    let mut range = Range::new(0.0, 10.0);
    range.set_low(20.0);
    let err = last_error();
    assert!(!err.message.is_empty());
    assert!((range.low() - 0.0).abs() < f64::EPSILON);
}

// format

#[test]
fn format_create_valid_type() {
    let format = Format::new("Decimal");
    assert_eq!(format.type_(), "Decimal");
}

#[test]
fn format_set_restriction_restriction_round_trip_via_copy() {
    let mut format = Format::new("Decimal");
    let mut r = Restriction::new();
    r.set_total_digits(5);
    format.set_restriction(&r);
    let stored = format.restriction().expect("restriction should be set");
    assert_eq!(stored.total_digits(), Some(5));
}

#[test]
fn format_validate_value_valid_decimal_string() {
    let format = Format::new("Decimal");
    assert!(format.validate_value("42.5"));
}

// data_channel_type

#[test]
fn data_channel_type_create_valid_type_is_alert_reflects_type() {
    let dct = DataChannelType::new("Alert");
    assert!(dct.is_alert());
}

#[test]
fn data_channel_type_set_update_cycle_negative_sets_last_error_leaves_unset() {
    let mut dct = DataChannelType::new("Inst");
    dct.set_update_cycle(-1.0);
    let err = last_error();
    assert!(!err.message.is_empty());
    assert_eq!(dct.update_cycle(), None);
}

// name_object

#[test]
fn name_object_create_default_matches_annex_c_naming_rule() {
    let n = NameObject::new();
    assert_eq!(n.naming_rule(), "/dnv-v2");
}

#[test]
fn name_object_set_custom_name_objects_takes_ownership() {
    let mut n = NameObject::new();
    let doc = SerializableDocument::from_str("custom");
    n.set_custom_name_objects(doc);
    let stored = n.custom_name_objects().expect("custom should be set");
    assert_eq!(stored.as_str(), Some("custom"));
}

// unit

#[test]
fn unit_create_unit_symbol_round_trips() {
    let unit = Unit::new("Cel");
    assert_eq!(unit.unit_symbol(), "Cel");
    assert_eq!(unit.quantity_name(), None);
}

// property

#[test]
fn property_create_decimal_type_without_range_unit_fails_validation() {
    let dct = DataChannelType::new("Inst");
    let format = Format::new("Decimal");
    let property = Property::new(&dct, &format);
    assert!(!property.validate());
    let err = last_error();
    assert!(!err.message.is_empty());
}

#[test]
fn property_create_decimal_type_with_range_and_unit_validates() {
    let dct = DataChannelType::new("Inst");
    let format = Format::new("Decimal");
    let range = Range::new(0.0, 100.0);
    let unit = Unit::new("Cel");
    let mut property = Property::new(&dct, &format);
    property.set_range(&range);
    property.set_unit(&unit);
    assert!(property.validate());
}

#[test]
fn property_create_alert_type_without_alert_priority_fails_validation() {
    let dct = DataChannelType::new("Alert");
    let format = Format::new("Boolean");
    let property = Property::new(&dct, &format);
    assert!(!property.validate());
}

// configuration_reference

#[test]
fn configuration_reference_create_id_and_timestamp_round_trip() {
    let ts = DateTimeOffset::utc_now();
    let config_ref = ConfigurationReference::new("cfg-1", ts);
    assert_eq!(config_ref.id(), "cfg-1");
    assert_eq!(config_ref.version(), None);
}

// version_information

#[test]
fn version_information_create_default_matches_annex_c_defaults() {
    let v = VersionInformation::new();
    assert!(!v.naming_rule().is_empty());
    assert!(!v.naming_scheme_version().is_empty());
}

// channel_id

#[test]
fn channel_id_create_local_id_round_trips() {
    let local_id = OwnedLocalId::from_string(VALID_LOCAL_ID_STR).expect("valid local id");
    let channel_id = DataChannelId::new(&*local_id);
    let stored_str = channel_id
        .local_id_string()
        .expect("should have local id string");
    assert_eq!(stored_str, VALID_LOCAL_ID_STR);
}

#[test]
fn channel_id_set_short_id_round_trips() {
    let local_id = OwnedLocalId::from_string(VALID_LOCAL_ID_STR).expect("valid local id");
    let mut channel_id = DataChannelId::new(&*local_id);
    channel_id.set_short_id("SID-1");
    assert_eq!(channel_id.short_id().as_deref(), Some("SID-1"));
}

// header

#[test]
fn header_create_ship_id_and_data_channel_list_id_round_trip() {
    let imo = ImoNumber::create(9074729).expect("valid IMO");
    let ship_id = ShipId::from_imo_number(imo);
    let ts = DateTimeOffset::utc_now();
    let config_ref = ConfigurationReference::new("cfg-1", ts);
    let header = Header::new(&ship_id, &config_ref);
    assert!(header.ship_id_equals(&ship_id));
}

// data_channel

#[test]
fn data_channel_create_valid_property_succeeds() {
    let local_id = OwnedLocalId::from_string(VALID_LOCAL_ID_STR).expect("valid local id");
    let channel_id = DataChannelId::new(&*local_id);
    let dct = DataChannelType::new("Inst");
    let format = Format::new("Boolean");
    let property = Property::new(&dct, &format);
    let _dc = DataChannel::new(&channel_id, &property);
}

#[test]
fn data_channel_create_invalid_property_decimal_without_range_unit_returns_err() {
    let local_id = OwnedLocalId::from_string(VALID_LOCAL_ID_STR).expect("valid local id");
    let channel_id = DataChannelId::new(&*local_id);
    let dct = DataChannelType::new("Inst");
    let format = Format::new("Decimal");
    let property = Property::new(&dct, &format);
    let result = DataChannel::try_new(&channel_id, &property);
    assert!(result.is_err());
    let err = last_error();
    assert!(!err.message.is_empty());
}

// data_channel_list

#[test]
fn data_channel_list_create_empty_list() {
    let list = DataChannelList::new();
    assert_eq!(list.len(), 0);
}

#[test]
fn data_channel_list_add_at_from_local_id_from_short_id_single_entry() {
    let local_id = OwnedLocalId::from_string(VALID_LOCAL_ID_STR).expect("valid local id");
    let mut channel_id = DataChannelId::new(&*local_id);
    channel_id.set_short_id("SID-1");
    let dct = DataChannelType::new("Inst");
    let format = Format::new("Boolean");
    let property = Property::new(&dct, &format);
    let dc = DataChannel::new(&channel_id, &property);

    let mut list = DataChannelList::new();
    assert!(list.add(&dc));
    assert_eq!(list.len(), 1);
    assert!(list.at(0).is_some());
    assert!(list.from_short_id("SID-1").is_some());
    assert!(list.from_local_id(&*local_id).is_some());
    assert!(list.from_short_id("missing").is_none());
}

#[test]
fn data_channel_list_add_duplicate_local_id_fails() {
    let local_id = OwnedLocalId::from_string(VALID_LOCAL_ID_STR).expect("valid local id");
    let channel_id = DataChannelId::new(&*local_id);
    let dct = DataChannelType::new("Inst");
    let format = Format::new("Boolean");
    let property = Property::new(&dct, &format);
    let dc = DataChannel::new(&channel_id, &property);

    let mut list = DataChannelList::new();
    assert!(list.add(&dc));
    assert!(!list.add(&dc));
    let err = last_error();
    assert!(!err.message.is_empty());
}

#[test]
fn data_channel_list_remove_and_clear() {
    let local_id = OwnedLocalId::from_string(VALID_LOCAL_ID_STR).expect("valid local id");
    let channel_id = DataChannelId::new(&*local_id);
    let dct = DataChannelType::new("Inst");
    let format = Format::new("Boolean");
    let property = Property::new(&dct, &format);
    let dc = DataChannel::new(&channel_id, &property);

    let mut list = DataChannelList::new();
    list.add(&dc);
    assert_eq!(list.len(), 1);
    assert!(list.remove(&dc));
    assert_eq!(list.len(), 0);
    list.add(&dc);
    list.clear();
    assert_eq!(list.len(), 0);
}

// package

#[test]
fn package_create_header_data_channel_list_round_trip() {
    let imo = ImoNumber::create(9074729).expect("valid IMO");
    let ship_id = ShipId::from_imo_number(imo);
    let ts = DateTimeOffset::utc_now();
    let config_ref = ConfigurationReference::new("cfg-1", ts);
    let header = Header::new(&ship_id, &config_ref);
    let list = DataChannelList::new();
    let package = Package::new(&header, &list);
    assert!(package.has_header());
    assert!(package.has_data_channel_list());
}

// list_package

#[test]
fn list_package_create_convenience_data_channel_list_accessor_matches_packages() {
    let imo = ImoNumber::create(9074729).expect("valid IMO");
    let ship_id = ShipId::from_imo_number(imo);
    let ts = DateTimeOffset::utc_now();
    let config_ref = ConfigurationReference::new("cfg-1", ts);
    let header = Header::new(&ship_id, &config_ref);
    let list = DataChannelList::new();
    let package = Package::new(&header, &list);
    let lp = DataChannelListPackage::new(&package);
    assert!(lp.has_package());
    assert!(lp.data_channel_list().is_some());
}
