use vista_sdk::types::decimal::{Decimal, RoundingMode};

#[test]
fn zero() {
    let d = Decimal::zero();
    assert_eq!(d, Decimal::from_i64(0));
}

#[test]
fn from_str_valid() {
    let d = Decimal::from_str("123.45").expect("valid");
    assert_eq!(d.to_string(), "123.45");
}

#[test]
fn from_str_negative() {
    let d = Decimal::from_str("-0.001").expect("valid");
    assert_eq!(d.to_string(), "-0.001");
}

#[test]
fn from_str_invalid() {
    assert!(Decimal::from_str("not-a-number").is_none());
}

#[test]
fn from_i64() {
    let d = Decimal::from_i64(42);
    assert_eq!(d.to_string(), "42");
}

#[test]
fn from_u64() {
    let d = Decimal::from_u64(1_000_000);
    assert_eq!(d.to_string(), "1000000");
}

#[test]
fn from_f64_roundtrip() {
    let d = Decimal::from_f64(3.14);
    assert!((d.to_f64() - 3.14).abs() < 1e-10);
}

#[test]
fn equality() {
    assert_eq!(
        Decimal::from_str("1.5").unwrap(),
        Decimal::from_str("1.5").unwrap()
    );
    assert_ne!(
        Decimal::from_str("1.5").unwrap(),
        Decimal::from_str("2.0").unwrap()
    );
}

#[test]
fn ordering() {
    let a = Decimal::from_str("1.0").unwrap();
    let b = Decimal::from_str("2.0").unwrap();
    assert!(a < b);
    assert!(b > a);
    assert_eq!(a, Decimal::from_str("1.0").unwrap());
}

#[test]
fn add() {
    let a = Decimal::from_str("45.72").unwrap();
    let b = Decimal::from_str("12.00").unwrap();
    assert_eq!((a + b).to_string(), "57.72");
}

#[test]
fn sub() {
    let a = Decimal::from_str("87.3").unwrap();
    let b = Decimal::from_str("72.1").unwrap();
    assert_eq!((a - b).to_string(), "15.2");
}

#[test]
fn mul_exact() {
    let consumption = Decimal::from_str("45.72").unwrap();
    let days = Decimal::from_i64(12);
    assert_eq!((consumption * days).to_string(), "548.64");
}

#[test]
fn mul_vs_double() {
    // 0.047 * 1000: classic IEEE 754 accumulation artifact
    let increment = Decimal::from_str("0.047").unwrap();
    let mut total = Decimal::zero();
    for _ in 0..1000 {
        total += increment;
    }
    let double_total: f64 = (0..1000).fold(0.0_f64, |acc, _| acc + 0.047_f64);
    assert_eq!(total, Decimal::from_i64(47));
    assert_ne!(double_total, 47.0_f64);
}

#[test]
fn div() {
    let a = Decimal::from_str("10.0").unwrap();
    let b = Decimal::from_str("4.0").unwrap();
    assert_eq!((a / b).to_string(), "2.5");
}

#[test]
fn neg() {
    let d = Decimal::from_str("1.5").unwrap();
    assert_eq!((-d).to_string(), "-1.5");
}

#[test]
fn abs() {
    let d = Decimal::from_str("-3.14").unwrap();
    assert_eq!(d.abs().to_string(), "3.14");
}

#[test]
fn ceil() {
    let d = Decimal::from_str("97.4375").unwrap();
    assert_eq!(d.ceil().to_string(), "98");
}

#[test]
fn floor() {
    let d = Decimal::from_str("97.4375").unwrap();
    assert_eq!(d.floor().to_string(), "97");
}

#[test]
fn trunc() {
    let d = Decimal::from_str("97.4375").unwrap();
    assert_eq!(d.trunc().to_string(), "97");
}

#[test]
fn round_to_nearest() {
    let d = Decimal::from_str("97.4375").unwrap();
    assert_eq!(d.round(1, RoundingMode::ToNearest).to_string(), "97.4");
}

#[test]
fn round_ties_away() {
    let d = Decimal::from_str("97.4375").unwrap();
    assert_eq!(
        d.round(1, RoundingMode::ToNearestTiesAway).to_string(),
        "97.4"
    );
}

#[test]
fn scale() {
    let d = Decimal::from_str("0.8914").unwrap();
    assert_eq!(d.scale(), 4);
}

#[test]
fn add_assign() {
    let increment = Decimal::from_str("0.047").unwrap();
    let mut total = Decimal::zero();
    for _ in 0..1000 {
        total += increment;
    }
    assert_eq!(total, Decimal::from_i64(47));
}

#[test]
fn celsius_to_kelvin_exact() {
    let offset = Decimal::from_str("273.15").unwrap();
    assert_eq!(offset.to_string(), "273.15");
    // from_str gives exact representation
    // from_f64 on a non-representable float may differ
    let from_non_exact = Decimal::from_f64(0.1_f64 + 0.2_f64);
    let exact = Decimal::from_str("0.3").unwrap();
    assert_ne!(from_non_exact, exact);
}

#[test]
fn display() {
    let d = Decimal::from_str("1.23").unwrap();
    assert_eq!(format!("{d}"), "1.23");
}

#[test]
fn min_max_lowest() {
    assert!(Decimal::min() > Decimal::zero());
    assert!(Decimal::max() > Decimal::min());
    assert!(Decimal::lowest() < Decimal::zero());
}
