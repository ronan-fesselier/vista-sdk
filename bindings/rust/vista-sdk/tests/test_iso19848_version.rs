use std::str::FromStr;
use vista_sdk::transport::iso19848_version::Iso19848Version;

#[test]
fn all_is_non_empty() {
    assert!(!Iso19848Version::all().is_empty());
}

#[test]
fn all_contains_known_versions() {
    let all = Iso19848Version::all();
    assert!(all.contains(&Iso19848Version::V2018));
    assert!(all.contains(&Iso19848Version::V2024));
}

#[test]
fn latest_is_last_in_all() {
    let all = Iso19848Version::all();
    assert_eq!(*all.last().unwrap(), Iso19848Version::latest());
}

#[test]
fn as_str_roundtrips() {
    for &v in Iso19848Version::all() {
        let s = v.as_str();
        assert_eq!(Iso19848Version::from_str(s).unwrap(), v);
    }
}

#[test]
fn display_matches_as_str() {
    for &v in Iso19848Version::all() {
        assert_eq!(format!("{v}"), v.as_str());
    }
}

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
