use vista_sdk::transport::timeseries::data_channel_id::TsdChannelId;

const LOCAL_ID_STR: &str =
    "/dnv-v2/vis-3-4a/411.1/C101.62/S205/meta/qty-temperature/cnt-lubricating.oil/pos-inlet";

#[test]
fn construction_from_local_id_string() {
    let id = TsdChannelId::from_string(LOCAL_ID_STR);
    assert!(id.is_some());
    let id = id.unwrap();
    assert!(id.is_local_id());
    assert!(!id.is_short_id());
    assert!(id.local_id().is_some());
    assert!(id.short_id().is_none());
}

#[test]
fn construction_from_short_id_string() {
    let id = TsdChannelId::from_string("CH001");
    assert!(id.is_some());
    let id = id.unwrap();
    assert!(!id.is_local_id());
    assert!(id.is_short_id());
    assert_eq!(id.short_id(), Some("CH001"));
    assert!(id.local_id().is_none());
}

#[test]
fn equality_same_local_ids() {
    let id1 = TsdChannelId::from_string(LOCAL_ID_STR).unwrap();
    let id2 = TsdChannelId::from_string(LOCAL_ID_STR).unwrap();
    assert_eq!(id1, id2);
}

#[test]
fn equality_same_short_ids() {
    let id1 = TsdChannelId::from_string("CH001").unwrap();
    let id2 = TsdChannelId::from_string("CH001").unwrap();
    assert_eq!(id1, id2);
}

#[test]
fn inequality_different_types() {
    let local_id =
        TsdChannelId::from_string("/dnv-v2/vis-3-4a/411.1/C101.44i-5A/C261/meta/qty-temperature")
            .unwrap();
    let short_id = TsdChannelId::from_string("CH001").unwrap();
    assert_ne!(local_id, short_id);
}

#[test]
fn inequality_different_values() {
    let id1 = TsdChannelId::from_string("CH001").unwrap();
    let id2 = TsdChannelId::from_string("CH002").unwrap();
    assert_ne!(id1, id2);
}

#[test]
fn to_string_local_id() {
    let s = "/dnv-v2/vis-3-4a/411.1/C101.44i-6A/C261/meta/qty-temperature";
    let id = TsdChannelId::from_string(s).unwrap();
    assert_eq!(id.to_string_repr(), s);
}

#[test]
fn to_string_short_id() {
    let id = TsdChannelId::from_string("CH001").unwrap();
    assert_eq!(id.to_string_repr(), "CH001");
}

#[test]
fn from_string_invalid_local_id_becomes_short_id() {
    let id = TsdChannelId::from_string("/invalid/local/id").unwrap();
    assert!(!id.is_local_id());
    assert!(id.is_short_id());
    assert_eq!(id.short_id(), Some("/invalid/local/id"));
}

#[test]
fn from_string_empty_returns_none() {
    assert!(TsdChannelId::from_string("").is_none());
}

#[test]
fn move_short_id_preserves_value() {
    let original = TsdChannelId::from_string("CH001").unwrap();
    let moved = original;
    assert!(moved.is_short_id());
    assert_eq!(moved.short_id(), Some("CH001"));
}

#[test]
fn move_local_id_preserves_value() {
    let s = "/dnv-v2/vis-3-4a/511.15-1/E32/meta/qty-power";
    let original = TsdChannelId::from_string(s).unwrap();
    let moved = original;
    assert!(moved.is_local_id());
    assert_eq!(moved.local_id().map(|l| l.to_string()), Some(s.to_string()));
}
