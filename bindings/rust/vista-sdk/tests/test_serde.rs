//! Integration tests for the optional `serde` feature.
//!
//! Only compiled when the feature is enabled (`cargo test --features serde`).

#![cfg(feature = "serde")]

use vista_sdk::*;

static VALID_JSON_RAW: &str =
    include_str!("../../../../cpp/tests/transport/_files/DataChannelList.json");

fn valid_json() -> &'static str {
    VALID_JSON_RAW
        .strip_prefix('\u{feff}')
        .unwrap_or(VALID_JSON_RAW)
}

#[test]
fn dto_package_serializes_to_the_same_json_as_the_sdk_writer() {
    let dto = data_channel_dto::from_json(valid_json()).expect("parse should succeed");

    let via_serde = serde_json::to_string(&dto).expect("serde serialization should succeed");
    let via_sdk = data_channel_dto::to_json(&dto, false);

    let from_serde: serde_json::Value = serde_json::from_str(&via_serde).expect("valid JSON");
    let from_sdk: serde_json::Value = serde_json::from_str(&via_sdk).expect("valid JSON");
    assert_eq!(
        from_serde, from_sdk,
        "serde serialization must agree with the SDK JSON writer"
    );
}

#[test]
fn dto_package_deserializes_from_json() {
    let dto = serde_json::from_str::<data_channel_dto::DtoPackage>(valid_json())
        .expect("serde deserialization should succeed");

    let mut reparsed = dto;
    let mut pkg = reparsed.pkg();
    assert!(!pkg.header().ship_id().is_empty());
}

#[test]
fn dto_package_serde_roundtrip() {
    let dto = data_channel_dto::from_json(valid_json()).expect("parse should succeed");
    let json = serde_json::to_string(&dto).expect("serialize");
    let back = serde_json::from_str::<data_channel_dto::DtoPackage>(&json).expect("deserialize");

    assert_eq!(
        data_channel_dto::to_json(&dto, false),
        data_channel_dto::to_json(&back, false)
    );
}

#[test]
fn domain_package_serializes_and_deserializes() {
    let package = data_channel_json::from_json(valid_json()).expect("parse should succeed");

    let json = serde_json::to_string(&package).expect("serialize");
    let back = serde_json::from_str::<DataChannelListPackage>(&json).expect("deserialize");

    assert_eq!(
        data_channel_json::to_json(&package, false),
        data_channel_json::to_json(&back, false)
    );
}

#[test]
fn serde_json_value_can_be_inserted_as_custom_header() {
    let domain = data_channel_json::from_json(valid_json()).expect("parse should succeed");
    let mut dto = data_channel_dto::to_dto(&domain).expect("to_dto should succeed");

    {
        let mut pkg = dto.pkg();
        let mut header = pkg.header();
        let mut custom = header.ensure_custom_headers();

        // Build a nested value with serde_json and hand it to the C++ document.
        let value = serde_json::json!({
            "exportedBy": "vista-sdk",
            "accuracy": 0.001,
            "validated": true,
            "steps": [1, 2, 3]
        });
        custom.set("exportPipeline", value.into());
    }

    let json = data_channel_dto::to_json(&dto, false);
    assert!(json.contains("exportPipeline"));
    assert!(json.contains("vista-sdk"));
    assert!(json.contains("accuracy"));
}

#[test]
fn custom_header_roundtrips_through_serde_json() {
    let domain = data_channel_json::from_json(valid_json()).expect("parse should succeed");
    let mut dto = data_channel_dto::to_dto(&domain).expect("to_dto should succeed");

    {
        let mut pkg = dto.pkg();
        let mut header = pkg.header();
        let mut custom = header.ensure_custom_headers();
        custom.set("origin", SerializableDocument::from_string("unit-test"));
    }

    let json = data_channel_dto::to_json(&dto, false);

    // Read it back through serde_json and check the extension survived.
    let value: serde_json::Value = serde_json::from_str(&json).expect("valid JSON");
    assert_eq!(
        value["Package"]["Header"]["origin"],
        serde_json::json!("unit-test")
    );
}
