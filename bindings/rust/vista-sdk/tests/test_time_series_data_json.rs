use vista_sdk::transport::timeseries::time_series_data_json;

static TIME_SERIES_DATA_JSON: &str =
    include_str!("../../../../cpp/tests/transport/_files/TimeSeriesData.json");

#[test]
fn from_json_string_malformed_returns_err() {
    let missing_ship_id = r#"{
        "Package": {
            "Header": {
                "TimeSpan": { "Start": "2016-01-01T00:00:00Z", "End": "2016-01-01T01:00:00Z" }
            }
        }
    }"#;
    assert!(time_series_data_json::from_json(missing_ship_id).is_err());
}

#[test]
fn from_json_string_parses_reference_file() {
    let package =
        time_series_data_json::from_json(TIME_SERIES_DATA_JSON).expect("reference file must parse");

    assert!(!package.is_empty());
    assert_eq!(package.time_series_data_count(), 2);
}

#[test]
fn roundtrip_domain_to_json_to_domain_compact() {
    let package =
        time_series_data_json::from_json(TIME_SERIES_DATA_JSON).expect("reference file must parse");

    let json2 = time_series_data_json::to_json(&package, false);
    assert!(!json2.is_empty());

    let package2 = time_series_data_json::from_json(&json2).expect("roundtrip must parse");
    assert_eq!(
        package.time_series_data_count(),
        package2.time_series_data_count()
    );
}

#[test]
fn roundtrip_pretty_print_output_still_parses_back_correctly() {
    let package =
        time_series_data_json::from_json(TIME_SERIES_DATA_JSON).expect("reference file must parse");

    let pretty = time_series_data_json::to_json(&package, true);
    assert!(pretty.contains('\n'));

    let package2 = time_series_data_json::from_json(&pretty).expect("pretty roundtrip must parse");
    assert_eq!(
        package.time_series_data_count(),
        package2.time_series_data_count()
    );
}

#[test]
fn from_json_string_empty_ship_id_returns_err() {
    let json = r#"{
        "Package": {
            "Header": {
                "ShipID": "   "
            },
            "TimeSeriesData": []
        }
    }"#;
    assert!(time_series_data_json::from_json(json).is_err());
}

#[test]
fn from_json_string_empty_returns_err() {
    assert!(time_series_data_json::from_json("").is_err());
}

#[test]
fn from_json_string_malformed_json_returns_err() {
    assert!(time_series_data_json::from_json("{not valid json}").is_err());
}
