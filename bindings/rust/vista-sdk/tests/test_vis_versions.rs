use std::str::FromStr;

use vista_sdk::core::vis_version::VisVersion;

#[test]
fn to_string() {
    assert_eq!(VisVersion::V3_11a.to_string(), "3-11a");
}

#[test]
fn from_str_known_versions() {
    assert_eq!(VisVersion::from_str("3-4a"), Ok(VisVersion::V3_4a));
    assert_eq!(VisVersion::from_str("3-11a"), Ok(VisVersion::V3_11a));
}

#[test]
fn from_str_invalid_returns_err() {
    assert!(VisVersion::from_str("invalid").is_err());
}

#[test]
fn from_str_empty_string() {
    assert!(VisVersion::from_str("").is_err());
}

#[test]
fn from_str_whitespace_only() {
    assert!(VisVersion::from_str("   ").is_err());
    assert!(VisVersion::from_str("\t").is_err());
    assert!(VisVersion::from_str("\n").is_err());
}

#[test]
fn from_str_case_sensitive() {
    assert!(VisVersion::from_str("3-9A").is_err());
    assert!(VisVersion::from_str("3-9a").is_ok());
}
