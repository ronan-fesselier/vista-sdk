use std::str::FromStr;

use vista_sdk::core::vis_version::VisVersion;

#[test]
fn to_string() {
    assert_eq!(VisVersion::V3_9a.to_string(), "3-9a");
}

#[test]
fn from_str_known_versions() {
    assert_eq!(VisVersion::from_str("3-4a"), Ok(VisVersion::V3_4a));
    assert_eq!(VisVersion::from_str("3-9a"), Ok(VisVersion::V3_9a));
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

#[test]
fn from_str_to_string_roundtrip_all_versions() {
    for v in VisVersion::all() {
        let s = v.to_string();
        let parsed = VisVersion::from_str(&s);
        assert!(parsed.is_ok());
        assert_eq!(*v, parsed.unwrap());
    }
}

#[test]
fn all_returns_non_empty() {
    assert!(!VisVersion::all().is_empty());
}

#[test]
fn latest_is_last_in_all() {
    let versions = VisVersion::all();
    assert_eq!(VisVersion::latest(), *versions.last().unwrap());
}
