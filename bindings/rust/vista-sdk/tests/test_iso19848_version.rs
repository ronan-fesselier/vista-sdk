use std::str::FromStr;
use vista_sdk::transport::iso19848_version::Iso19848Version;

#[test]
fn from_str_invalid_returns_err() {
    assert!(Iso19848Version::from_str("v9999").is_err());
    assert!(Iso19848Version::from_str("").is_err());
    assert!(Iso19848Version::from_str("2018").is_err());
}

#[test]
fn ordering_ascending() {
    assert!(Iso19848Version::V2018 < Iso19848Version::V2024);
}

#[test]
fn known_as_str_values() {
    assert_eq!(Iso19848Version::V2018.as_str(), "v2018");
    assert_eq!(Iso19848Version::V2024.as_str(), "v2024");
}
