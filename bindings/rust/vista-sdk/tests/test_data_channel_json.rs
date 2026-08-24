use vista_sdk::core::error::last_error;
use vista_sdk::transport::datachannel::data_channel_json;

static VALID_JSON: &str =
    include_str!("../../../../cpp/tests/transport/_files/DataChannelList.json");

#[test]
fn list_package_from_json_parses_the_reference_file() {
    let package = data_channel_json::from_json(VALID_JSON).expect("parse should succeed");
    let list = package.data_channel_list().expect("list should be present");
    assert!(list.len() > 0);
    let dc = list.at(0).expect("first data channel should exist");
    let channel_id = dc.channel_id();
    assert_eq!(channel_id.short_id().as_deref(), Some("0010"));
    let inner = package.package().expect("package should be present");
    let header = inner.header().expect("header should be present");
    assert!(header.ship_id_is_some());
}

#[test]
fn list_package_from_json_empty_returns_err_and_sets_last_error() {
    let result = data_channel_json::from_json("");
    assert!(result.is_err());
    let err = last_error();
    assert!(!err.message.is_empty());
}

#[test]
fn list_package_from_json_malformed_returns_err_and_sets_last_error() {
    let result = data_channel_json::from_json("{ not valid json");
    assert!(result.is_err());
    let err = last_error();
    assert!(!err.message.is_empty());
}

#[test]
fn list_package_from_json_missing_mandatory_field_returns_err() {
    let missing_ship_id = r#"{
        "Package": {
            "Header": {
                "DataChannelListID": {
                    "ID": "DataChannelList.xml",
                    "TimeStamp": "2016-01-01T00:00:00Z"
                }
            },
            "DataChannelList": { "DataChannel": [] }
        }
    }"#;
    let result = data_channel_json::from_json(missing_ship_id);
    assert!(result.is_err());
}

#[test]
fn roundtrip_domain_to_json_to_domain_compact() {
    let package = data_channel_json::from_json(VALID_JSON).expect("parse should succeed");
    let json = data_channel_json::to_json(&package, false);
    let round_tripped =
        data_channel_json::from_json(&json).expect("roundtrip parse should succeed");
    let original_list = package.data_channel_list().expect("list should be present");
    let round_tripped_list = round_tripped
        .data_channel_list()
        .expect("list should be present");
    assert_eq!(original_list.len(), round_tripped_list.len());
}

#[test]
fn roundtrip_pretty_print_output_still_parses_back_correctly() {
    let package = data_channel_json::from_json(VALID_JSON).expect("parse should succeed");
    let pretty_json = data_channel_json::to_json(&package, true);
    assert!(pretty_json.contains('\n'));
    let round_tripped =
        data_channel_json::from_json(&pretty_json).expect("roundtrip parse should succeed");
    assert!(round_tripped.data_channel_list().is_some());
}
