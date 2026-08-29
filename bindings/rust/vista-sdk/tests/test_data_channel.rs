use vista_sdk::*;

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
    let r = Restriction::new().with_enumeration(&["A", "B", "C"]);
    assert_eq!(r.enumeration_count(), 3);
    assert_eq!(r.enumeration_at(0).as_deref(), Some("A"));
    assert_eq!(r.enumeration_at(2).as_deref(), Some("C"));
}

#[test]
fn restriction_set_total_digits_zero_sets_last_error_and_leaves_unset() {
    let result = Restriction::new().with_total_digits(0);
    assert!(result.is_err());
    let err = result.err().expect("should be err");
    assert!(!err.message.is_empty());
}

#[test]
fn restriction_set_white_space_round_trip() {
    let r = Restriction::new().with_white_space(WhiteSpace::Collapse);
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
    let result = Range::new(0.0, 10.0).with_low(20.0);
    assert!(result.is_err());
    let err = result.err().expect("should be err");
    assert!(!err.message.is_empty());
}

#[test]
fn range_set_low_equal_to_high_is_allowed() {
    let range = Range::new(0.0, 10.0)
        .with_low(10.0)
        .expect("low == high should be allowed");
    assert!((range.low() - 10.0).abs() < f64::EPSILON);
}

#[test]
fn range_set_high_equal_to_low_is_allowed() {
    let range = Range::new(0.0, 10.0)
        .with_high(0.0)
        .expect("high == low should be allowed");
    assert!((range.high() - 0.0).abs() < f64::EPSILON);
}

// format

#[test]
fn format_create_valid_type() {
    let format = Format::new("Decimal");
    assert_eq!(format.type_(), "Decimal");
}

#[test]
fn format_set_restriction_restriction_round_trip_via_copy() {
    let r = Restriction::new()
        .with_total_digits(5)
        .expect("valid total_digits");
    let format = Format::new("Decimal").with_restriction(&r);
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
    let dct = DataChannelType::new("Inst").with_update_cycle(-1.0);
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
    let doc = SerializableDocument::from_string("custom");
    let n = NameObject::new().with_custom_name_objects(doc);
    let stored = n.custom_name_objects().expect("custom should be set");
    assert_eq!(stored.as_str().as_deref(), Some("custom"));
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
    let property = Property::new(&dct, &format)
        .with_range(&range)
        .with_unit(&unit);
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
    let local_id = LocalId::from_string(VALID_LOCAL_ID_STR).expect("valid local id");
    let channel_id = DataChannelId::new(&local_id);
    let stored_str = channel_id
        .local_id_string()
        .expect("should have local id string");
    assert_eq!(stored_str, VALID_LOCAL_ID_STR);
}

#[test]
fn channel_id_set_short_id_round_trips() {
    let local_id = LocalId::from_string(VALID_LOCAL_ID_STR).expect("valid local id");
    let channel_id = DataChannelId::new(&local_id).with_short_id("SID-1");
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
    assert_eq!(header.ship_id(), Some(ship_id));
}

// data_channel

#[test]
fn data_channel_create_valid_property_succeeds() {
    let local_id = LocalId::from_string(VALID_LOCAL_ID_STR).expect("valid local id");
    let channel_id = DataChannelId::new(&local_id);
    let dct = DataChannelType::new("Inst");
    let format = Format::new("Boolean");
    let property = Property::new(&dct, &format);
    let _dc = DataChannel::new(&channel_id, &property);
}

#[test]
fn data_channel_create_invalid_property_decimal_without_range_unit_returns_err() {
    let local_id = LocalId::from_string(VALID_LOCAL_ID_STR).expect("valid local id");
    let channel_id = DataChannelId::new(&local_id);
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
    let local_id = LocalId::from_string(VALID_LOCAL_ID_STR).expect("valid local id");
    let channel_id = DataChannelId::new(&local_id).with_short_id("SID-1");
    let dct = DataChannelType::new("Inst");
    let format = Format::new("Boolean");
    let property = Property::new(&dct, &format);
    let dc = DataChannel::new(&channel_id, &property);

    let mut list = DataChannelList::new();
    assert!(list.add(&dc));
    assert_eq!(list.len(), 1);
    assert!(list.at(0).is_some());
    assert!(list.from_short_id("SID-1").is_some());
    assert!(list.from_local_id(&local_id).is_some());
    assert!(list.from_short_id("missing").is_none());
}

#[test]
fn data_channel_list_add_duplicate_local_id_fails() {
    let local_id = LocalId::from_string(VALID_LOCAL_ID_STR).expect("valid local id");
    let channel_id = DataChannelId::new(&local_id);
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
    let local_id = LocalId::from_string(VALID_LOCAL_ID_STR).expect("valid local id");
    let channel_id = DataChannelId::new(&local_id);
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

fn make_header() -> Header {
    let imo = ImoNumber::create(9074729).expect("valid IMO");
    let ship_id = ShipId::from_imo_number(imo);
    let ts = DateTimeOffset::utc_now();
    let config_ref = ConfigurationReference::new("cfg-1", ts);
    Header::new(&ship_id, &config_ref)
}

#[test]
fn header_with_and_without_author() {
    let h = make_header().with_author("Alice");
    assert_eq!(h.author().as_deref(), Some("Alice"));
    let h2 = h.without_author();
    assert!(h2.author().is_none());
}

#[test]
fn header_with_and_without_date_created() {
    let ts = DateTimeOffset::utc_now();
    let h = make_header().with_date_created(ts);
    assert!(h.date_created().is_some());
    let h2 = h.without_date_created();
    assert!(h2.date_created().is_none());
}

#[test]
fn header_with_and_without_version_information() {
    let vi = VersionInformation::with_fields("dnv", "v2");
    let list = DataChannelList::new();
    let lp = DataChannelListPackage::new(&Package::new(
        &make_header().with_version_information(&vi),
        &list,
    ));
    assert!(lp
        .package()
        .expect("pkg")
        .header()
        .expect("header")
        .version_information()
        .is_some());
    let lp2 = DataChannelListPackage::new(&Package::new(
        &make_header().without_version_information(),
        &list,
    ));
    assert!(lp2
        .package()
        .expect("pkg")
        .header()
        .expect("header")
        .version_information()
        .is_none());
}

#[test]
fn header_ship_id_is_some_and_data_channel_list_id() {
    let list = DataChannelList::new();
    let lp = DataChannelListPackage::new(&Package::new(&make_header(), &list));
    let pkg = lp.package().expect("pkg");
    let href = pkg.header().expect("header");
    assert!(href.ship_id_is_some());
    assert!(!href.data_channel_list_id().id().is_empty());
}

#[test]
fn header_with_and_without_custom_headers() {
    let list = DataChannelList::new();
    let lp = DataChannelListPackage::new(&Package::new(
        &make_header().with_custom_headers(SerializableDocument::object()),
        &list,
    ));
    assert!(lp
        .package()
        .expect("pkg")
        .header()
        .expect("header")
        .custom_headers()
        .is_some());
    let lp2 = DataChannelListPackage::new(&Package::new(
        &make_header().without_custom_headers(),
        &list,
    ));
    assert!(lp2
        .package()
        .expect("pkg")
        .header()
        .expect("header")
        .custom_headers()
        .is_none());
}

#[test]
fn configuration_reference_with_and_without_version() {
    let ts = DateTimeOffset::utc_now();
    let cr = ConfigurationReference::new("id-1", ts).with_version("v1");
    assert_eq!(cr.version().as_deref(), Some("v1"));
    let cr2 = cr.without_version();
    assert!(cr2.version().is_none());
}

#[test]
fn version_information_builder_methods() {
    let vi = VersionInformation::new()
        .with_naming_rule("custom-rule")
        .with_naming_scheme_version("v3");
    assert_eq!(vi.naming_rule(), "custom-rule");
    assert_eq!(vi.naming_scheme_version(), "v3");
}

#[test]
fn version_information_with_and_without_reference_url() {
    let vi = VersionInformation::with_fields("dnv", "v2").with_reference_url("https://example.com");
    assert!(vi.reference_url().is_some());
    let vi2 = vi.without_reference_url();
    assert!(vi2.reference_url().is_none());
}

#[test]
fn data_channel_try_new_valid_succeeds() {
    let local_id = LocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature")
        .expect("valid");
    let dc_id = DataChannelId::new(&local_id);
    let restriction = Restriction::new()
        .with_min_inclusive(0.0)
        .with_max_inclusive(100.0);
    let format = Format::new("Decimal").with_restriction(&restriction);
    let dct = DataChannelType::new("Inst");
    let range = Range::new(0.0, 100.0);
    let unit = Unit::new("degC");
    let prop = Property::new(&dct, &format)
        .with_range(&range)
        .with_unit(&unit);
    assert!(DataChannel::try_new(&dc_id, &prop).is_ok());
}

#[test]
fn data_channel_id_without_short_id() {
    let local_id = LocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature")
        .expect("valid");
    let dc_id = DataChannelId::new(&local_id).with_short_id("S1");
    assert!(dc_id.short_id().is_some());
    let dc_id2 = dc_id.without_short_id();
    assert!(dc_id2.short_id().is_none());
}

#[test]
fn data_channel_id_local_id_string() {
    let local_id = LocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature")
        .expect("valid");
    let dc_id = DataChannelId::new(&local_id);
    assert!(dc_id.local_id_string().is_some());
}

#[test]
fn data_channel_id_with_and_without_name_object() {
    let local_id = LocalId::from_string("/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature")
        .expect("valid");
    let dc_id = DataChannelId::new(&local_id);
    let no = NameObject::new();
    let dc_id2 = dc_id.with_name_object(&no);
    assert!(dc_id2.name_object().is_some());
    let dc_id3 = dc_id2.without_name_object();
    assert!(dc_id3.name_object().is_none());
}

#[test]
fn package_with_header_and_with_data_channel_list() {
    let imo = ImoNumber::create(9074729).expect("valid IMO");
    let ship_id = ShipId::from_imo_number(imo);
    let ts = DateTimeOffset::utc_now();
    let config_ref = ConfigurationReference::new("cfg-1", ts);
    let header = Header::new(&ship_id, &config_ref);
    let list = DataChannelList::new();
    let mut pkg = Package::new(&header, &list);
    assert!(pkg.has_header());
    assert!(pkg.has_data_channel_list());
    let new_header = make_header();
    pkg = pkg.with_header(&new_header);
    assert!(pkg.has_header());
    let new_list = DataChannelList::new();
    pkg = pkg.with_data_channel_list(&new_list);
    assert!(pkg.has_data_channel_list());
}
