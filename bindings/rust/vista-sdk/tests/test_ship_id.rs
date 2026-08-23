use vista_sdk::core::imo_number::ImoNumber;
use vista_sdk::transport::ship_id::ShipId;

#[test]
fn construction_from_imo_number() {
    let imo = ImoNumber::from_str("9074729").expect("9074729 should be a valid IMO number");
    let ship_id = ShipId::from_imo_number(imo);

    assert!(ship_id.is_imo_number());
    assert!(!ship_id.is_other_id());
}

#[test]
fn construction_from_alternative_id() {
    let ship_id = ShipId::from_other_id("VESSEL-123").expect("non-empty string should succeed");

    assert!(!ship_id.is_imo_number());
    assert!(ship_id.is_other_id());
}

#[test]
fn construction_empty_alternative_id_returns_err() {
    assert!(ShipId::from_other_id("").is_err());
}

#[test]
fn imo_number_accessor() {
    let imo = ImoNumber::from_str("9074729").expect("9074729 should be a valid IMO number");
    let ship_id = ShipId::from_imo_number(imo);

    let retrieved = ship_id.imo_number();
    assert!(retrieved.is_some());
    assert_eq!(retrieved.unwrap().to_string(), "IMO9074729");
    assert!(ship_id.other_id().is_none());
}

#[test]
fn other_id_accessor() {
    let ship_id = ShipId::from_other_id("VESSEL-ABC-456").expect("non-empty string should succeed");

    let other_id = ship_id.other_id();
    assert!(other_id.is_some());
    assert_eq!(other_id.unwrap(), "VESSEL-ABC-456");
    assert!(ship_id.imo_number().is_none());
}

#[test]
fn to_string_imo_number() {
    let imo = ImoNumber::from_str("9074729").expect("9074729 should be a valid IMO number");
    let ship_id = ShipId::from_imo_number(imo);

    assert_eq!(ship_id.to_string(), "IMO9074729");
}

#[test]
fn to_string_alternative_id() {
    let ship_id = ShipId::from_other_id("CUSTOM-SHIP-ID").expect("non-empty string should succeed");

    assert_eq!(ship_id.to_string(), "CUSTOM-SHIP-ID");
}

#[test]
fn from_string_imo_with_prefix() {
    let ship_id = ShipId::from_string("IMO9074729").expect("IMO9074729 should parse");

    assert!(ship_id.is_imo_number());
    assert_eq!(ship_id.imo_number().unwrap().to_string(), "IMO9074729");
}

#[test]
fn from_string_imo_with_prefix_case_insensitive() {
    let ship_id1 = ShipId::from_string("imo9074729").expect("imo9074729 should parse");
    assert!(ship_id1.is_imo_number());

    let ship_id2 = ShipId::from_string("ImO9074729").expect("ImO9074729 should parse");
    assert!(ship_id2.is_imo_number());
}

#[test]
fn from_string_invalid_imo_falls_back_to_alternative_id() {
    let ship_id = ShipId::from_string("IMO1234568").expect("IMO1234568 should parse as other");

    assert!(ship_id.is_other_id());
    assert_eq!(ship_id.other_id().unwrap(), "IMO1234568");
}

#[test]
fn from_string_alternative_id() {
    let ship_id = ShipId::from_string("VESSEL-XYZ-789").expect("VESSEL-XYZ-789 should parse");

    assert!(ship_id.is_other_id());
    assert_eq!(ship_id.other_id().unwrap(), "VESSEL-XYZ-789");
}

#[test]
fn from_string_empty_returns_none() {
    assert!(ShipId::from_string("").is_none());
}

#[test]
fn from_string_whitespace_only_returns_none() {
    assert!(ShipId::from_string("   ").is_none());
}

#[test]
fn equality_imo_numbers() {
    let imo1 = ImoNumber::from_str("9074729").unwrap();
    let imo2 = ImoNumber::from_str("9074729").unwrap();
    let imo3 = ImoNumber::from_str("1234567").unwrap();

    let ship_id1 = ShipId::from_imo_number(imo1);
    let ship_id2 = ShipId::from_imo_number(imo2);
    let ship_id3 = ShipId::from_imo_number(imo3);

    assert_eq!(ship_id1, ship_id2);
    assert_ne!(ship_id1, ship_id3);
}

#[test]
fn equality_alternative_ids() {
    let ship_id1 = ShipId::from_other_id("VESSEL-A").unwrap();
    let ship_id2 = ShipId::from_other_id("VESSEL-A").unwrap();
    let ship_id3 = ShipId::from_other_id("VESSEL-B").unwrap();

    assert_eq!(ship_id1, ship_id2);
    assert_ne!(ship_id1, ship_id3);
}

#[test]
fn equality_imo_vs_alternative() {
    let imo = ImoNumber::from_str("9074729").unwrap();
    let ship_id1 = ShipId::from_imo_number(imo);
    let ship_id2 = ShipId::from_other_id("9074729").unwrap();

    assert_ne!(ship_id1, ship_id2);
}

#[test]
fn round_trip_imo_number() {
    let imo = ImoNumber::from_str("9074729").unwrap();
    let ship_id1 = ShipId::from_imo_number(imo);

    let s = ship_id1.to_string();
    let ship_id2 = ShipId::from_string(&s).expect("round-trip should succeed");

    assert_eq!(ship_id1, ship_id2);
    assert!(ship_id2.is_imo_number());
}

#[test]
fn round_trip_alternative_id() {
    let ship_id1 = ShipId::from_other_id("VESSEL-ROUND-TRIP").unwrap();

    let s = ship_id1.to_string();
    let ship_id2 = ShipId::from_string(&s).expect("round-trip should succeed");

    assert_eq!(ship_id1, ship_id2);
    assert!(ship_id2.is_other_id());
}
