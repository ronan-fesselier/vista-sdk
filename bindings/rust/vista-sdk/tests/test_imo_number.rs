use vista_sdk::core::imo_number::ImoNumber;

#[test]
fn is_valid_accepts_valid_numbers() {
    assert!(ImoNumber::is_valid(9074729));
    assert!(ImoNumber::is_valid(9785811));
    assert!(ImoNumber::is_valid(9704611));
}

#[test]
fn is_valid_rejects_invalid_numbers() {
    assert!(!ImoNumber::is_valid(-1));
    assert!(!ImoNumber::is_valid(0));
    assert!(!ImoNumber::is_valid(1));
    assert!(!ImoNumber::is_valid(1234507));
}

#[test]
fn create_valid_value_matches_from_str() {
    let from_int = ImoNumber::create(9074729).expect("9074729 should be a valid IMO number");
    let from_str = ImoNumber::from_str("9074729").expect("9074729 should be a valid IMO number");

    assert_eq!(from_int, from_str);
}

#[test]
fn create_invalid_checksum_returns_err() {
    let result = ImoNumber::create(1234507);
    assert!(result.is_err());
}

#[test]
fn from_str_valid_string_with_and_without_prefix() {
    let without_prefix =
        ImoNumber::from_str("9074729").expect("9074729 should be a valid IMO number");
    let with_prefix =
        ImoNumber::from_str("IMO9074729").expect("IMO9074729 should be a valid IMO number");

    assert_eq!(without_prefix, with_prefix);
}

#[test]
fn from_str_invalid_string_returns_none() {
    assert!(ImoNumber::from_str("not-an-imo").is_none());
}

#[test]
fn to_string_formats_with_imo_prefix() {
    let imo_number = ImoNumber::from_str("9074729").expect("9074729 should be a valid IMO number");

    assert_eq!(imo_number.to_string(), "IMO9074729");
}

#[test]
fn equals_different_numbers_are_not_equal() {
    let a = ImoNumber::from_str("9074729").expect("9074729 should be a valid IMO number");
    let b = ImoNumber::from_str("9785811").expect("9785811 should be a valid IMO number");

    assert_ne!(a, b);
}

#[test]
fn value_returns_raw_integer() {
    let imo = ImoNumber::create(9074729).expect("9074729 should be a valid IMO number");
    assert_eq!(imo.value(), 9074729);

    let imo2 = ImoNumber::create(9785811).expect("9785811 should be a valid IMO number");
    assert_eq!(imo2.value(), 9785811);
}
