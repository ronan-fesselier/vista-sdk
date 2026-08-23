use vista_sdk::transport::iso19848::{Iso19848, Value};
use vista_sdk::transport::iso19848_version::Iso19848Version;
use vista_sdk::types::date_time_offset::DateTimeOffset;
use vista_sdk::types::decimal::Decimal;

#[test]
fn instance_is_stable() {
    let a = Iso19848::instance();
    let b = Iso19848::instance();
    assert!(!std::ptr::eq(&a as *const _, &b as *const _));
    assert_eq!(a.versions(), b.versions());
}

#[test]
fn versions_non_empty_and_known() {
    let iso = Iso19848::instance();
    let versions = iso.versions();
    assert!(!versions.is_empty());
    assert!(versions.contains(&Iso19848Version::V2018));
    assert!(versions.contains(&Iso19848Version::V2024));
}

#[test]
fn latest_is_v2024() {
    let iso = Iso19848::instance();
    assert_eq!(iso.latest(), Iso19848Version::V2024);
}

#[test]
fn data_channel_type_names_non_empty() {
    let iso = Iso19848::instance();
    let names = iso.data_channel_type_names(Iso19848Version::V2024).unwrap();
    assert!(!names.is_empty());
    for entry in names.iter() {
        assert!(!entry.type_.is_empty());
        assert!(!entry.description.is_empty());
    }
}

#[test]
fn data_channel_type_names_from_str_known() {
    let iso = Iso19848::instance();
    let names = iso.data_channel_type_names(Iso19848Version::V2024).unwrap();
    let entry = names.from_str("Inst").unwrap();
    assert_eq!(entry.type_, "Inst");
}

#[test]
fn data_channel_type_names_from_str_unknown_returns_none() {
    let iso = Iso19848::instance();
    let names = iso.data_channel_type_names(Iso19848Version::V2024).unwrap();
    assert!(names.from_str("not-a-type").is_none());
}

#[test]
fn format_data_types_non_empty() {
    let iso = Iso19848::instance();
    let types = iso.format_data_types(Iso19848Version::V2024).unwrap();
    assert!(!types.is_empty());
    for entry in types.iter() {
        assert!(!entry.type_().is_empty());
        assert!(!entry.description().is_empty());
    }
}

#[test]
fn format_data_type_validate_decimal_valid() {
    let iso = Iso19848::instance();
    let types = iso.format_data_types(Iso19848Version::V2024).unwrap();
    let decimal_type = types.from_str("Decimal").unwrap();
    let value = decimal_type.validate("0.1").unwrap();
    assert!(matches!(value, Value::Decimal(_)));
}

#[test]
fn format_data_type_validate_boolean_invalid_returns_none() {
    let iso = Iso19848::instance();
    let types = iso.format_data_types(Iso19848Version::V2024).unwrap();
    let bool_type = types.from_str("Boolean").unwrap();
    assert!(bool_type.validate("yes").is_none());
}

#[test]
fn value_from_string() {
    let v = Value::from_string("test").unwrap();
    assert!(matches!(v, Value::String(s) if s == "test"));
}

#[test]
fn value_from_integer() {
    let v = Value::from_integer(42);
    assert!(matches!(v, Value::Integer(42)));
}

#[test]
fn value_from_boolean() {
    let v = Value::from_boolean(true);
    assert!(matches!(v, Value::Boolean(true)));
}

#[test]
fn value_from_decimal() {
    let d = Decimal::from_str("3.14").unwrap();
    let v = Value::from_decimal(d);
    assert!(matches!(v, Value::Decimal(_)));
}

#[test]
fn value_from_date_time() {
    let dto = DateTimeOffset::epoch();
    let v = Value::from_date_time(dto);
    assert!(matches!(v, Value::DateTime(_)));
}

#[test]
fn value_to_string_integer() {
    let v = Value::from_integer(123);
    assert_eq!(v.to_string(), "123");
}

#[test]
fn value_display_string_variant() {
    let v = Value::from_string("hello").unwrap();
    assert_eq!(format!("{v}"), "hello");
}

#[test]
fn value_wrong_accessor_returns_none_via_match() {
    let v = Value::from_integer(1);
    assert!(!matches!(v, Value::String(_)));
    assert!(!matches!(v, Value::Boolean(_)));
}
