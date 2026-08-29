use vista_sdk::*;

static VALID_JSON: &str =
    include_str!("../../../../cpp/tests/transport/_files/TimeSeriesData.json");

#[test]
fn from_json_parses_reference_file() {
    let dto = time_series_data_dto::from_json(VALID_JSON).expect("parse should succeed");
    let _ = dto;
}

#[test]
fn from_json_empty_returns_err() {
    let result = time_series_data_dto::from_json("");
    assert!(result.is_err());
}

#[test]
fn from_json_malformed_returns_err() {
    let result = time_series_data_dto::from_json("{ not valid json");
    assert!(result.is_err());
}

#[test]
fn to_json_roundtrip() {
    let dto = time_series_data_dto::from_json(VALID_JSON).expect("parse should succeed");
    let json = time_series_data_dto::to_json(&dto, false);
    assert!(!json.is_empty());
    let dto2 = time_series_data_dto::from_json(&json).expect("roundtrip parse should succeed");
    let json2 = time_series_data_dto::to_json(&dto2, false);
    assert_eq!(json, json2);
}

#[test]
fn dto_pkg_header_ship_id() {
    let mut dto = time_series_data_dto::from_json(VALID_JSON).expect("parse should succeed");
    let mut pkg = dto.pkg();
    let header = pkg.header().expect("header should be present");
    let ship_id = header.ship_id();
    assert!(!ship_id.is_empty());
}

#[test]
fn dto_header_set_author_roundtrip() {
    let mut dto = time_series_data_dto::from_json(VALID_JSON).expect("parse should succeed");
    let mut pkg = dto.pkg();
    let mut header = pkg.header().expect("header should be present");
    header.set_author("test-author");
    let got = header.author();
    assert_eq!(got.as_deref(), Some("test-author"));
    header.clear_author();
    assert!(header.author().is_none());
}

#[test]
fn dto_header_time_span_set_and_read() {
    let mut dto = time_series_data_dto::from_json(VALID_JSON).expect("parse should succeed");
    let mut pkg = dto.pkg();
    let mut header = pkg.header().expect("header should be present");
    header.ensure_time_span();
    assert!(header.has_time_span());
    {
        let mut ts = header
            .time_span()
            .expect("time_span should exist after ensure");
        ts.set_start("2024-01-01T00:00:00Z");
        ts.set_end("2024-01-01T01:00:00Z");
        assert_eq!(ts.start(), "2024-01-01T00:00:00Z");
        assert_eq!(ts.end(), "2024-01-01T01:00:00Z");
    }
    header.clear_time_span();
    assert!(!header.has_time_span());
}

#[test]
fn dto_header_system_cfg_push_and_remove() {
    let mut dto = time_series_data_dto::from_json(VALID_JSON).expect("parse should succeed");
    let mut pkg = dto.pkg();
    let mut header = pkg.header().expect("header should be present");
    let before = header.system_cfg_count();
    {
        let mut cfg = header.system_cfg_push();
        cfg.set_id("cfg-001");
        cfg.set_timestamp("2024-01-01T00:00:00Z");
        assert_eq!(cfg.id(), "cfg-001");
        assert_eq!(cfg.timestamp(), "2024-01-01T00:00:00Z");
    }
    assert_eq!(header.system_cfg_count(), before + 1);
    header.system_cfg_remove(before);
    assert_eq!(header.system_cfg_count(), before);
}

#[test]
fn dto_header_ensure_custom_headers_and_set() {
    let mut dto = time_series_data_dto::from_json(VALID_JSON).expect("parse should succeed");
    let mut pkg = dto.pkg();
    let mut header = pkg.header().expect("header should be present");
    {
        let mut custom = header.ensure_custom_headers();
        custom.set("exportedBy", SerializableDocument::from_string("rust-test"));
    }
    assert!(header.has_custom_headers());
    {
        let custom = header
            .custom_headers()
            .expect("custom headers should exist");
        assert!(custom.contains("exportedBy"));
    }
    header.clear_custom_headers();
    assert!(!header.has_custom_headers());
}

#[test]
fn dto_push_time_series_and_tabular() {
    let mut dto = time_series_data_dto::from_json(VALID_JSON).expect("parse should succeed");
    let mut pkg = dto.pkg();
    let before = pkg.tsd_count();
    {
        let mut tsd = pkg.tsd_push();
        tsd.ensure_data_cfg();
        assert!(tsd.has_data_cfg());
        {
            let mut cfg = tsd.data_cfg().expect("data_cfg should exist after ensure");
            cfg.set_id("dcl-001");
            cfg.set_timestamp("2024-01-01T00:00:00Z");
        }
        let _ = tsd.tabular_push();
        assert_eq!(tsd.tabular_count(), 1);
    }
    assert_eq!(pkg.tsd_count(), before + 1);
    pkg.tsd_remove(before);
    assert_eq!(pkg.tsd_count(), before);
}

#[test]
fn dto_tabular_channel_ids_and_datasets() {
    let mut dto = time_series_data_dto::from_json(VALID_JSON).expect("parse should succeed");
    let mut pkg = dto.pkg();
    let tsd_count = pkg.tsd_count();
    assert!(
        tsd_count > 0,
        "reference file must have at least one TSD entry"
    );
    let mut tsd = pkg.tsd_at(0).expect("first tsd should exist");
    let tab_count = tsd.tabular_count();
    assert!(
        tab_count > 0,
        "first TSD entry must have at least one tabular block"
    );
    let tab = tsd.tabular_at(0).expect("first tabular should exist");
    let ch_count = tab.channel_id_count();
    assert!(ch_count > 0, "tabular must have at least one channel ID");
    let first_id = tab
        .channel_id_at(0)
        .expect("first channel ID should be readable");
    assert!(!first_id.is_empty());
}

#[test]
fn dto_tabular_set_channel_ids_and_dataset_values() {
    let mut dto = time_series_data_dto::from_json(VALID_JSON).expect("parse should succeed");
    let mut pkg = dto.pkg();
    let mut tsd = pkg.tsd_push();
    let mut tab = tsd.tabular_push();
    tab.set_channel_ids(&[
        "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature",
        "/dnv-v2/vis-3-4a/411.1/meta/qty-pressure",
    ]);
    assert_eq!(tab.channel_id_count(), 2);
    {
        let mut ds = tab.data_set_push();
        ds.set_timestamp("2024-01-01T00:00:01Z");
        ds.set_values(&["22.5", "101.3"]);
        ds.set_quality(&["Good", "Good"]);
        assert_eq!(ds.timestamp(), "2024-01-01T00:00:01Z");
        assert_eq!(ds.value_count(), 2);
        assert_eq!(ds.value_at(0).as_deref(), Some("22.5"));
        assert_eq!(ds.quality_count(), 2);
        assert_eq!(ds.quality_at(0).as_deref(), Some("Good"));
    }
    assert_eq!(tab.data_set_count(), 1);
    tab.data_set_remove(0);
    assert_eq!(tab.data_set_count(), 0);
    tab.clear_channel_ids();
    assert_eq!(tab.channel_id_count(), 0);
}

#[test]
fn dto_event_data_push_and_read() {
    let mut dto = time_series_data_dto::from_json(VALID_JSON).expect("parse should succeed");
    let mut pkg = dto.pkg();
    let mut tsd = pkg.tsd_push();
    tsd.ensure_event();
    assert!(tsd.has_event());
    {
        let mut ev = tsd.event().expect("event should exist after ensure");
        let mut ds = ev.data_set_push();
        ds.set_timestamp("2024-01-01T00:01:00Z");
        ds.set_data_channel_id("/dnv-v2/vis-3-4a/411.1/meta/qty-temperature");
        ds.set_value("23.0");
        assert_eq!(ds.timestamp(), "2024-01-01T00:01:00Z");
        assert_eq!(
            ds.data_channel_id(),
            "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature"
        );
        assert_eq!(ds.value(), "23.0");
        assert!(!ds.has_quality());
        ds.set_quality("Good");
        assert!(ds.has_quality());
        assert_eq!(ds.quality().as_deref(), Some("Good"));
        ds.clear_quality();
        assert!(!ds.has_quality());
        assert_eq!(ev.data_set_count(), 1);
        ev.data_set_remove(0);
        assert_eq!(ev.data_set_count(), 0);
    }
    tsd.clear_event();
    assert!(!tsd.has_event());
}

#[test]
fn to_dto_and_back_to_domain() {
    let domain = time_series_data_json::from_json(VALID_JSON).expect("domain parse should succeed");
    let dto = time_series_data_dto::to_dto(&domain).expect("to_dto should succeed");
    let domain2 = time_series_data_dto::to_domain(&dto).expect("to_domain should succeed");
    let json1 = time_series_data_json::to_json(&domain, false);
    let json2 = time_series_data_json::to_json(&domain2, false);
    assert_eq!(json1, json2);
}

#[test]
fn dto_patch_before_serialization_is_visible_in_json() {
    let domain = time_series_data_json::from_json(VALID_JSON).expect("domain parse should succeed");
    let mut dto = time_series_data_dto::to_dto(&domain).expect("to_dto should succeed");
    {
        let mut pkg = dto.pkg();
        let mut header = pkg.header().expect("header should be present after to_dto");
        header.set_author("export-pipeline");
        let mut custom = header.ensure_custom_headers();
        custom.set(
            "exportedBy",
            SerializableDocument::from_string("vista-sdk-test"),
        );
    }
    let json = time_series_data_dto::to_json(&dto, false);
    assert!(
        json.contains("export-pipeline"),
        "patched author must appear"
    );
    assert!(
        json.contains("exportedBy"),
        "patched custom header must appear"
    );
    assert!(json.contains("vista-sdk-test"), "patched value must appear");
}
