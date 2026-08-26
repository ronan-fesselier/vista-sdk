use vista_sdk::transport::datachannel::data_channel::{
    ConfigurationReference as DclConfigRef, DataChannel, DataChannelId, DataChannelList,
    DataChannelListPackage, DataChannelType, Format, Header as DclHeader, NameObject, Package,
    Property, Range, Restriction, Unit, VersionInformation,
};
use vista_sdk::transport::ship_id::ShipId;
use vista_sdk::transport::timeseries::data_channel_id::TsdChannelId;
use vista_sdk::transport::timeseries::time_series_data::{
    EventData, EventDataSet, TabularData, TabularDataSet, TimeSeriesData, TimeSeriesDataPackage,
    TsdConfigRef, TsdHeader, TsdPackage, TsdTimeSpan, ValidateCallback,
};
use vista_sdk::types::date_time_offset::DateTimeOffset;

fn create_valid_fully_custom_dcl() -> DataChannelListPackage {
    let timestamp = DateTimeOffset::from_str("2016-01-01T00:00:00Z").unwrap();
    let dcl_id = DclConfigRef::new("DataChannelList.xml", timestamp);

    let mut version_info = VersionInformation::with_fields("some_naming_rule", "2.0");
    version_info.set_reference_url("http://somewhere.net");

    let ship_id = ShipId::from_string("IMO1234567").unwrap();
    let mut header = DclHeader::new(&ship_id, &dcl_id);
    header.set_version_information(&version_info);
    header.set_author("Author1");
    header.set_date_created(DateTimeOffset::from_str("2015-12-01T00:00:00Z").unwrap());

    let mut dcl = DataChannelList::new();

    {
        let local_id = vista_sdk::core::local_id_builder::OwnedLocalIdBuilder::from_string(
            "/dnv-v2/vis-3-4a/411.1-1/C101.63/S206/meta/qty-temperature/cnt-cooling.air",
        )
        .unwrap();
        let mut channel_id = DataChannelId::new(&local_id);
        channel_id.set_short_id("0010");
        let mut name_object = NameObject::new();
        name_object.set_naming_rule("Naming_Rule");
        channel_id.set_name_object(&name_object);

        let mut dc_type = DataChannelType::new("Inst");
        dc_type.set_update_cycle(1.0);

        let mut format = Format::new("Decimal");
        let mut restriction = Restriction::new();
        restriction.set_fraction_digits(1);
        restriction.set_max_inclusive(200.0);
        restriction.set_min_inclusive(-150.0);
        format.set_restriction(&restriction);

        let range = Range::new(0.0, 150.0);
        let mut unit = Unit::new("°C");
        unit.set_quantity_name("Temperature");

        let mut property = Property::new(&dc_type, &format);
        property.set_range(&range);
        property.set_unit(&unit);
        property.set_quality_coding("OPC_QUALITY");
        property.set_name("M/E #1 Air Cooler CFW OUT Temp");
        property.set_remarks(" Location: ECR, Manufacturer: AAA Company, Type: TYPE-AAA ");

        dcl.add(&DataChannel::new(&channel_id, &property));
    }

    {
        let local_id = vista_sdk::core::local_id_builder::OwnedLocalIdBuilder::from_string(
            "/dnv-v2/vis-3-4a/511.15-1/E32/meta/qty-power",
        )
        .unwrap();
        let mut channel_id = DataChannelId::new(&local_id);
        channel_id.set_short_id("0020");

        let dc_type = DataChannelType::new("Alert");

        let mut format = Format::new("String");
        let mut restriction = Restriction::new();
        restriction.set_max_length(100);
        restriction.set_min_length(0);
        format.set_restriction(&restriction);

        let mut property = Property::new(&dc_type, &format);
        property.set_alert_priority("Warning");

        dcl.add(&DataChannel::new(&channel_id, &property));
    }

    let package = Package::new(&header, &dcl);
    DataChannelListPackage::new(&package)
}

fn build_ts_channel_ids(dc_package: &DataChannelListPackage) -> Vec<TsdChannelId> {
    let list = dc_package.data_channel_list().unwrap();
    (0..list.len())
        .filter_map(|i| {
            let dc = list.at(i)?;
            let id_ref = dc.channel_id();
            let s = id_ref.short_id().or_else(|| None)?;
            TsdChannelId::from_string(&s)
        })
        .collect()
}

fn create_test_tsd_package() -> TimeSeriesDataPackage {
    let dc_package = create_valid_fully_custom_dcl();
    let all_ids = build_ts_channel_ids(&dc_package);

    let start = DateTimeOffset::from_str("2016-01-01T12:00:00Z").unwrap();
    let end = DateTimeOffset::from_str("2016-01-03T12:00:00Z").unwrap();
    let ts1 = DateTimeOffset::from_str("2016-01-01T12:00:00Z").unwrap();
    let ts2 = DateTimeOffset::from_str("2016-01-02T12:00:00Z").unwrap();
    let ts3 = DateTimeOffset::from_str("2016-01-03T12:00:00Z").unwrap();
    let event_ts = DateTimeOffset::from_str("2016-01-01T12:00:01Z").unwrap();
    let created_modified = DateTimeOffset::from_str("2016-01-03T12:00:00Z").unwrap();
    let config_ts = DateTimeOffset::from_str("2016-01-03T00:00:00Z").unwrap();

    let ds1_0 = TabularDataSet::new(ts1, &["100.0", "200.0"], Some(&["0", "0"]));
    let ds1_1 = TabularDataSet::new(ts2, &["105.0", "210.0"], Some(&["0", "0"]));
    let tabular1 = TabularData::new(&[&all_ids[0], &all_ids[1]], &[&ds1_0, &ds1_1]);

    let ds2_0 = TabularDataSet::new(ts1, &["100.0"], None);
    let ds2_1 = TabularDataSet::new(ts2, &["100.1"], None);
    let ds2_2 = TabularDataSet::new(ts3, &["100.2"], None);
    let tabular2 = TabularData::new(&[&all_ids[0]], &[&ds2_0, &ds2_1, &ds2_2]);

    let eds = EventDataSet::new(event_ts, &all_ids[1], "HIGH", Some("0"));
    let event_data = EventData::new(Some(&[&eds]));

    let data_config = TsdConfigRef::new(
        "DataChannelList.xml",
        DateTimeOffset::from_str("2016-01-01T00:00:00Z").unwrap(),
    );

    let sys_cfg_0 = TsdConfigRef::new("SystemConfiguration.xml", config_ts);
    let sys_cfg_1 = TsdConfigRef::new("SystemConfiguration.xml", config_ts);

    let mut custom1 = vista_sdk::transport::serializable_document::SerializableDocument::object();
    custom1.set(
        "dataQuality",
        vista_sdk::transport::serializable_document::SerializableDocument::from_str("high"),
    );
    custom1.set(
        "recordCount",
        vista_sdk::transport::serializable_document::SerializableDocument::from_i64(42),
    );
    custom1.set(
        "validated",
        vista_sdk::transport::serializable_document::SerializableDocument::from_bool(true),
    );
    custom1.set(
        "accuracy",
        vista_sdk::transport::serializable_document::SerializableDocument::from_f64(0.01),
    );
    custom1.set(
        "processingTime",
        vista_sdk::transport::serializable_document::SerializableDocument::from_str(
            "2024-01-15T10:30:00Z",
        ),
    );

    let mut ts_data1 = TimeSeriesData::new();
    ts_data1.set_data_configuration(&data_config);
    ts_data1.set_tabular_data(&[&tabular1, &tabular2]);
    ts_data1.set_event_data(&event_data);
    ts_data1.set_custom_data_kinds(custom1);

    let mut custom2 = vista_sdk::transport::serializable_document::SerializableDocument::object();
    custom2.set(
        "source",
        vista_sdk::transport::serializable_document::SerializableDocument::from_str(
            "sensor_array_1",
        ),
    );
    custom2.set(
        "version",
        vista_sdk::transport::serializable_document::SerializableDocument::from_i64(2),
    );

    let mut ts_data2 = TimeSeriesData::new();
    ts_data2.set_data_configuration(&data_config);
    ts_data2.set_tabular_data(&[&tabular1, &tabular2]);
    ts_data2.set_event_data(&event_data);
    ts_data2.set_custom_data_kinds(custom2);

    let mut custom_headers =
        vista_sdk::transport::serializable_document::SerializableDocument::object();
    custom_headers.set(
        "temperatureUnit",
        vista_sdk::transport::serializable_document::SerializableDocument::from_str("Celsius"),
    );
    custom_headers.set(
        "sampleRate",
        vista_sdk::transport::serializable_document::SerializableDocument::from_i64(1000),
    );
    custom_headers.set(
        "isCompressed",
        vista_sdk::transport::serializable_document::SerializableDocument::from_bool(true),
    );
    custom_headers.set(
        "precision",
        vista_sdk::transport::serializable_document::SerializableDocument::from_f64(0.001),
    );
    custom_headers.set(
        "calibrationDate",
        vista_sdk::transport::serializable_document::SerializableDocument::from_str(
            "2024-01-15T00:00:00Z",
        ),
    );

    let ship_id = ShipId::from_string("IMO1234567").unwrap();
    let time_span = TsdTimeSpan::new(start, end).unwrap();
    let mut tsd_header = TsdHeader::new(&ship_id);
    tsd_header.set_time_span(&time_span);
    tsd_header.set_date_created(created_modified);
    tsd_header.set_date_modified(created_modified);
    tsd_header.set_author("Shipboard data server");
    tsd_header.set_system_configuration(&[&sys_cfg_0, &sys_cfg_1]);
    tsd_header.set_custom_headers(custom_headers);

    let tsd_package = TsdPackage::new(Some(&tsd_header), &[&ts_data1, &ts_data2]);
    TimeSeriesDataPackage::new(&tsd_package)
}

fn ok_callback() -> Box<ValidateCallback> {
    Box::new(|_ts| Ok(()))
}

#[test]
fn time_series_data_package_is_non_empty() {
    let message = create_test_tsd_package();
    assert!(!message.is_empty());
}

#[test]
fn time_series_data_structure_validation() {
    let dc_package = create_valid_fully_custom_dcl();
    let all_ids = build_ts_channel_ids(&dc_package);

    let ts1 = DateTimeOffset::from_str("2016-01-01T12:00:00Z").unwrap();
    let ts2 = DateTimeOffset::from_str("2016-01-02T12:00:00Z").unwrap();
    let ts3 = DateTimeOffset::from_str("2016-01-03T12:00:00Z").unwrap();

    let ds1_0 = TabularDataSet::new(ts1, &["100.0", "200.0"], Some(&["0", "0"]));
    let ds1_1 = TabularDataSet::new(ts2, &["105.0", "210.0"], Some(&["0", "0"]));
    let tabular1 = TabularData::new(&[&all_ids[0], &all_ids[1]], &[&ds1_0, &ds1_1]);
    assert!(tabular1.channel_id_count() != 0);
    assert!(tabular1.data_set_count() != 0);

    let ds2_0 = TabularDataSet::new(ts1, &["100.0"], None);
    let ds2_1 = TabularDataSet::new(ts2, &["100.1"], None);
    let ds2_2 = TabularDataSet::new(ts3, &["100.2"], None);
    let tabular2 = TabularData::new(&[&all_ids[0]], &[&ds2_0, &ds2_1, &ds2_2]);
    assert!(tabular2.channel_id_count() != 0);
    assert!(tabular2.data_set_count() != 0);

    let event_ts = DateTimeOffset::from_str("2016-01-01T12:00:01Z").unwrap();
    let eds = EventDataSet::new(event_ts, &all_ids[1], "HIGH", Some("0"));
    let event_data = EventData::new(Some(&[&eds]));
    assert!(event_data.data_set_count() > 0);
}

#[test]
fn time_span_valid_construction_and_setters() {
    let start = DateTimeOffset::from_str("2016-01-01T12:00:00Z").unwrap();
    let end = DateTimeOffset::from_str("2016-01-03T12:00:00Z").unwrap();

    let mut time_span = TsdTimeSpan::new(start, end).unwrap();
    assert_eq!(time_span.start().to_string(), start.to_string());
    assert_eq!(time_span.end().to_string(), end.to_string());

    let new_start = DateTimeOffset::from_str("2016-01-02T12:00:00Z").unwrap();
    time_span.set_start(new_start);
    assert_eq!(time_span.start().to_string(), new_start.to_string());

    let new_end = DateTimeOffset::from_str("2016-01-04T12:00:00Z").unwrap();
    time_span.set_end(new_end);
    assert_eq!(time_span.end().to_string(), new_end.to_string());
}

#[test]
fn time_span_invalid_range_returns_err() {
    let start = DateTimeOffset::from_str("2016-01-03T12:00:00Z").unwrap();
    let end = DateTimeOffset::from_str("2016-01-01T12:00:00Z").unwrap();
    assert!(TsdTimeSpan::new(start, end).is_err());

    let valid_start = DateTimeOffset::from_str("2016-01-01T12:00:00Z").unwrap();
    let valid_end = DateTimeOffset::from_str("2016-01-03T12:00:00Z").unwrap();
    let mut ts = TsdTimeSpan::new(valid_start, valid_end).unwrap();
    ts.set_start(DateTimeOffset::from_str("2016-01-04T12:00:00Z").unwrap());
    assert!(ts.start().to_string() != "2016-01-04T12:00:00+00:00");
    ts.set_end(DateTimeOffset::from_str("2015-12-31T12:00:00Z").unwrap());
    assert!(ts.end().to_string() != "2015-12-31T12:00:00+00:00");
}

#[test]
fn tabular_data_valid_validation() {
    let id1 = TsdChannelId::from_string("0010").unwrap();
    let id2 = TsdChannelId::from_string("0020").unwrap();
    let ts = DateTimeOffset::from_str("2016-01-01T12:00:00Z").unwrap();
    let ds = TabularDataSet::new(ts, &["100.0", "200.0"], Some(&["0", "0"]));
    let tabular = TabularData::new(&[&id1, &id2], &[&ds]);
    assert!(tabular.validate());
}

#[test]
fn tabular_data_dimension_mismatch_fails_validation() {
    let id1 = TsdChannelId::from_string("0010").unwrap();
    let id2 = TsdChannelId::from_string("0020").unwrap();
    let ts = DateTimeOffset::from_str("2016-01-01T12:00:00Z").unwrap();
    let ds = TabularDataSet::new(ts, &["100.0", "200.0", "300.0"], Some(&["0", "0", "0"]));
    let tabular = TabularData::new(&[&id1, &id2], &[&ds]);
    assert!(!tabular.validate());
}

#[test]
fn event_data_operations() {
    let mut event_data = EventData::new(None);
    assert_eq!(event_data.data_set_count(), 0);

    let ts = DateTimeOffset::from_str("2016-01-01T12:00:01Z").unwrap();
    let id = TsdChannelId::from_string("0010").unwrap();
    let eds1 = EventDataSet::new(ts, &id, "HIGH", Some("0"));
    event_data.set_data_sets(&[&eds1]);
    assert_eq!(event_data.data_set_count(), 1);

    let eds2 = EventDataSet::new(ts, &id, "LOW", Some("0"));
    event_data.set_data_sets(&[&eds1, &eds2]);
    assert_eq!(event_data.data_set_count(), 2);

    event_data.clear_data_sets();
    assert_eq!(event_data.data_set_count(), 0);
}

#[test]
fn config_ref_construction_and_setters() {
    let ts = DateTimeOffset::from_str("2016-01-01T00:00:00Z").unwrap();
    let mut config_ref = TsdConfigRef::new("TestConfig.xml", ts);
    assert_eq!(config_ref.id(), "TestConfig.xml");
    assert_eq!(config_ref.time_stamp().to_string(), ts.to_string());

    config_ref.set_id("NewConfig.xml");
    assert_eq!(config_ref.id(), "NewConfig.xml");

    let new_ts = DateTimeOffset::from_str("2016-01-02T00:00:00Z").unwrap();
    config_ref.set_time_stamp(new_ts);
    assert_eq!(config_ref.time_stamp().to_string(), new_ts.to_string());
}

#[test]
fn tabular_data_set_construction_with_and_without_quality() {
    let ts = DateTimeOffset::from_str("2016-01-01T12:00:00Z").unwrap();
    let ds = TabularDataSet::new(ts, &["100.0", "200.0"], Some(&["0", "0"]));
    assert_eq!(ds.time_stamp().to_string(), ts.to_string());
    let vals = ds.values();
    assert_eq!(vals.len(), 2);
    assert_eq!(vals[0], "100.0");
    assert_eq!(vals[1], "200.0");
    let q = ds.quality().unwrap();
    assert_eq!(q.len(), 2);
    assert_eq!(q[0], "0");

    let ds_nq = TabularDataSet::new(ts, &["100.0", "200.0"], None);
    assert!(ds_nq.quality().is_none());
}

#[test]
fn event_data_set_construction_with_and_without_quality() {
    let ts = DateTimeOffset::from_str("2016-01-01T12:00:01Z").unwrap();
    let id = TsdChannelId::from_string("0010").unwrap();

    let eds = EventDataSet::new(ts, &id, "HIGH", Some("0"));
    assert_eq!(eds.time_stamp().to_string(), ts.to_string());
    assert_eq!(eds.value(), "HIGH");
    assert_eq!(eds.quality().unwrap(), "0");

    let eds_nq = EventDataSet::new(ts, &id, "LOW", None);
    assert!(eds_nq.quality().is_none());
}

#[test]
fn package_empty_time_series_data_is_allowed() {
    let ship_id = ShipId::from_string("IMO1234567").unwrap();
    let header = TsdHeader::new(&ship_id);
    let package = TsdPackage::new(Some(&header), &[]);
    assert!(package.time_series_data_is_empty());
    assert!(package.has_header());
}

#[test]
fn valid_time_series_data_against_data_channel_list() {
    let dc_package = create_valid_fully_custom_dcl();
    let all_ids = build_ts_channel_ids(&dc_package);

    let ts1 = DateTimeOffset::from_str("2016-01-01T12:00:00Z").unwrap();
    let ts2 = DateTimeOffset::from_str("2016-01-02T12:00:00Z").unwrap();
    let ds1 = TabularDataSet::new(ts1, &["100.0", "200.0"], Some(&["0", "0"]));
    let ds2 = TabularDataSet::new(ts2, &["105.0", "210.0"], Some(&["0", "0"]));
    let tabular = TabularData::new(&[&all_ids[0], &all_ids[1]], &[&ds1, &ds2]);

    let data_config = TsdConfigRef::new(
        "DataChannelList.xml",
        DateTimeOffset::from_str("2016-01-01T00:00:00Z").unwrap(),
    );
    let mut ts_data = TimeSeriesData::new();
    ts_data.set_data_configuration(&data_config);
    ts_data.set_tabular_data(&[&tabular]);

    let result = ts_data.validate(&dc_package, ok_callback().as_ref(), ok_callback().as_ref());
    assert!(result.is_valid());
}

#[test]
fn invalid_data_channel_id_not_found_in_dcl() {
    let dc_package = create_valid_fully_custom_dcl();

    let ts = DateTimeOffset::from_str("2016-01-01T12:00:00Z").unwrap();
    let invalid_id = TsdChannelId::from_string("9999").unwrap();
    let ds = TabularDataSet::new(ts, &["100.0"], None);
    let tabular = TabularData::new(&[&invalid_id], &[&ds]);

    let data_config = TsdConfigRef::new(
        "DataChannelList.xml",
        DateTimeOffset::from_str("2016-01-01T00:00:00Z").unwrap(),
    );
    let mut ts_data = TimeSeriesData::new();
    ts_data.set_data_configuration(&data_config);
    ts_data.set_tabular_data(&[&tabular]);

    let result = ts_data.validate(&dc_package, ok_callback().as_ref(), ok_callback().as_ref());
    assert!(!result.is_valid());
    assert!(!result.errors().is_empty());
    assert!(
        result.errors()[0].contains("not found"),
        "expected 'not found' in: {}",
        result.errors()[0]
    );
}

#[test]
fn invalid_value_format_mismatch() {
    let dc_package = create_valid_fully_custom_dcl();
    let all_ids = build_ts_channel_ids(&dc_package);

    let ts = DateTimeOffset::from_str("2016-01-01T12:00:00Z").unwrap();
    let ds = TabularDataSet::new(ts, &["invalid_not_a_number"], None);
    let tabular = TabularData::new(&[&all_ids[0]], &[&ds]);

    let data_config = TsdConfigRef::new(
        "DataChannelList.xml",
        DateTimeOffset::from_str("2016-01-01T00:00:00Z").unwrap(),
    );
    let mut ts_data = TimeSeriesData::new();
    ts_data.set_data_configuration(&data_config);
    ts_data.set_tabular_data(&[&tabular]);

    let result = ts_data.validate(&dc_package, ok_callback().as_ref(), ok_callback().as_ref());
    assert!(!result.is_valid());
    assert!(!result.errors().is_empty());
    assert!(
        result.errors()[0].contains("invalid value"),
        "expected 'invalid value' in: {}",
        result.errors()[0]
    );
}

#[test]
fn invalid_data_config_id_mismatch() {
    let dc_package = create_valid_fully_custom_dcl();
    let all_ids = build_ts_channel_ids(&dc_package);

    let ts1 = DateTimeOffset::from_str("2016-01-01T12:00:00Z").unwrap();
    let ds = TabularDataSet::new(ts1, &["100.0", "200.0"], Some(&["0", "0"]));
    let tabular = TabularData::new(&[&all_ids[0], &all_ids[1]], &[&ds]);

    let wrong_config = TsdConfigRef::new(
        "WrongDataChannelList.xml",
        DateTimeOffset::from_str("2016-01-01T00:00:00Z").unwrap(),
    );
    let mut ts_data = TimeSeriesData::new();
    ts_data.set_data_configuration(&wrong_config);
    ts_data.set_tabular_data(&[&tabular]);

    let result = ts_data.validate(&dc_package, ok_callback().as_ref(), ok_callback().as_ref());
    assert!(!result.is_valid());
    assert!(!result.errors().is_empty());
    assert!(
        result.errors()[0].contains("does not match"),
        "expected 'does not match' in: {}",
        result.errors()[0]
    );
}

#[test]
fn invalid_empty_time_series_data() {
    let dc_package = create_valid_fully_custom_dcl();

    let data_config = TsdConfigRef::new(
        "DataChannelList.xml",
        DateTimeOffset::from_str("2016-01-01T00:00:00Z").unwrap(),
    );
    let mut ts_data = TimeSeriesData::new();
    ts_data.set_data_configuration(&data_config);

    let result = ts_data.validate(&dc_package, ok_callback().as_ref(), ok_callback().as_ref());
    assert!(!result.is_valid());
    assert!(!result.errors().is_empty());
    assert!(
        result.errors()[0].contains("without data"),
        "expected 'without data' in: {}",
        result.errors()[0]
    );
}

#[test]
fn custom_callback_rejection() {
    let dc_package = create_valid_fully_custom_dcl();
    let all_ids = build_ts_channel_ids(&dc_package);

    let ts1 = DateTimeOffset::from_str("2016-01-01T12:00:00Z").unwrap();
    let ts2 = DateTimeOffset::from_str("2016-01-02T12:00:00Z").unwrap();
    let ds1 = TabularDataSet::new(ts1, &["100.0", "200.0"], Some(&["0", "0"]));
    let ds2 = TabularDataSet::new(ts2, &["105.0", "210.0"], Some(&["0", "0"]));
    let tabular = TabularData::new(&[&all_ids[0], &all_ids[1]], &[&ds1, &ds2]);

    let data_config = TsdConfigRef::new(
        "DataChannelList.xml",
        DateTimeOffset::from_str("2016-01-01T00:00:00Z").unwrap(),
    );
    let mut ts_data = TimeSeriesData::new();
    ts_data.set_data_configuration(&data_config);
    ts_data.set_tabular_data(&[&tabular]);

    let on_tabular: &ValidateCallback = &|_ts| Err("Custom business rule violation".to_string());
    let result = ts_data.validate(&dc_package, on_tabular, ok_callback().as_ref());
    assert!(!result.is_valid());
    assert!(!result.errors().is_empty());
    assert!(
        result.errors()[0].contains("Custom business rule violation"),
        "expected 'Custom business rule violation' in: {}",
        result.errors()[0]
    );
}
