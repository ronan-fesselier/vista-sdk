use vista_sdk::*;

fn create_valid_fully_custom_dcl() -> DataChannelListPackage {
    let timestamp = "2016-01-01T00:00:00Z".parse::<DateTimeOffset>().unwrap();
    let dcl_id = ConfigurationReference::new("DataChannelList.xml", timestamp);

    let version_info = VersionInformation::with_fields("some_naming_rule", "2.0")
        .with_reference_url("http://somewhere.net");

    let ship_id = ShipId::from_string("IMO1234567").unwrap();
    let header = Header::new(&ship_id, &dcl_id)
        .with_version_information(&version_info)
        .with_author("Author1")
        .with_date_created("2015-12-01T00:00:00Z".parse::<DateTimeOffset>().unwrap());

    let mut dcl = DataChannelList::new();

    {
        let local_id = vista_sdk::core::local_id_builder::LocalIdBuilder::from_string(
            "/dnv-v2/vis-3-4a/411.1-1/C101.63/S206/meta/qty-temperature/cnt-cooling.air",
        )
        .unwrap();
        let name_object = NameObject::new().with_naming_rule("Naming_Rule");
        let channel_id = DataChannelId::new(&local_id)
            .with_short_id("0010")
            .with_name_object(&name_object);

        let dc_type = DataChannelType::new("Inst").with_update_cycle(1.0);

        let restriction = Restriction::new()
            .with_fraction_digits(1)
            .with_max_inclusive(200.0)
            .with_min_inclusive(-150.0);
        let format = Format::new("Decimal").with_restriction(&restriction);

        let range = Range::new(0.0, 150.0);
        let unit = Unit::new("°C").with_quantity_name("Temperature");

        let property = Property::new(&dc_type, &format)
            .with_range(&range)
            .with_unit(&unit)
            .with_quality_coding("OPC_QUALITY")
            .with_name("M/E #1 Air Cooler CFW OUT Temp")
            .with_remarks(" Location: ECR, Manufacturer: AAA Company, Type: TYPE-AAA ");

        dcl.add(&DataChannel::new(&channel_id, &property));
    }

    {
        let local_id = vista_sdk::core::local_id_builder::LocalIdBuilder::from_string(
            "/dnv-v2/vis-3-4a/511.15-1/E32/meta/qty-power",
        )
        .unwrap();
        let channel_id = DataChannelId::new(&local_id).with_short_id("0020");

        let dc_type = DataChannelType::new("Alert");

        let restriction = Restriction::new().with_max_length(100).with_min_length(0);
        let format = Format::new("String").with_restriction(&restriction);

        let property = Property::new(&dc_type, &format).with_alert_priority("Warning");

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
            let s = id_ref.short_id().or(None)?;
            TsdChannelId::from_string(&s)
        })
        .collect()
}

fn create_test_tsd_package() -> TimeSeriesDataPackage {
    let dc_package = create_valid_fully_custom_dcl();
    let all_ids = build_ts_channel_ids(&dc_package);

    let start = "2016-01-01T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let end = "2016-01-03T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let ts1 = "2016-01-01T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let ts2 = "2016-01-02T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let ts3 = "2016-01-03T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let event_ts = "2016-01-01T12:00:01Z".parse::<DateTimeOffset>().unwrap();
    let created_modified = "2016-01-03T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let config_ts = "2016-01-03T00:00:00Z".parse::<DateTimeOffset>().unwrap();

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
        "2016-01-01T00:00:00Z".parse::<DateTimeOffset>().unwrap(),
    );

    let sys_cfg_0 = TsdConfigRef::new("SystemConfiguration.xml", config_ts);
    let sys_cfg_1 = TsdConfigRef::new("SystemConfiguration.xml", config_ts);

    let mut custom1 = vista_sdk::transport::serializable_document::SerializableDocument::object();
    custom1.set(
        "dataQuality",
        vista_sdk::transport::serializable_document::SerializableDocument::from_string("high"),
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
        vista_sdk::transport::serializable_document::SerializableDocument::from_string(
            "2024-01-15T10:30:00Z",
        ),
    );

    let ts_data1 = TimeSeriesData::new()
        .with_data_configuration(&data_config)
        .with_tabular_data(&[&tabular1, &tabular2])
        .with_event_data(&event_data)
        .with_custom_data_kinds(custom1);

    let mut custom2 = vista_sdk::transport::serializable_document::SerializableDocument::object();
    custom2.set(
        "source",
        vista_sdk::transport::serializable_document::SerializableDocument::from_string(
            "sensor_array_1",
        ),
    );
    custom2.set(
        "version",
        vista_sdk::transport::serializable_document::SerializableDocument::from_i64(2),
    );

    let ts_data2 = TimeSeriesData::new()
        .with_data_configuration(&data_config)
        .with_tabular_data(&[&tabular1, &tabular2])
        .with_event_data(&event_data)
        .with_custom_data_kinds(custom2);

    let mut custom_headers =
        vista_sdk::transport::serializable_document::SerializableDocument::object();
    custom_headers.set(
        "temperatureUnit",
        vista_sdk::transport::serializable_document::SerializableDocument::from_string("Celsius"),
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
        vista_sdk::transport::serializable_document::SerializableDocument::from_string(
            "2024-01-15T00:00:00Z",
        ),
    );

    let ship_id = ShipId::from_string("IMO1234567").unwrap();
    let time_span = TsdTimeSpan::new(start, end).unwrap();
    let tsd_header = TsdHeader::new(&ship_id)
        .with_time_span(&time_span)
        .with_date_created(created_modified)
        .with_date_modified(created_modified)
        .with_author("Shipboard data server")
        .with_system_configuration(&[&sys_cfg_0, &sys_cfg_1])
        .with_custom_headers(custom_headers);

    let tsd_package = TsdPackage::new(Some(&tsd_header), &[&ts_data1, &ts_data2]);
    TimeSeriesDataPackage::new(&tsd_package)
}

fn ok_callback() -> Box<ValidateCallback> {
    Box::new(|_ts, _dc, _val, _qual| ValidateResult::Ok)
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

    let ts1 = "2016-01-01T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let ts2 = "2016-01-02T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let ts3 = "2016-01-03T12:00:00Z".parse::<DateTimeOffset>().unwrap();

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

    let event_ts = "2016-01-01T12:00:01Z".parse::<DateTimeOffset>().unwrap();
    let eds = EventDataSet::new(event_ts, &all_ids[1], "HIGH", Some("0"));
    let event_data = EventData::new(Some(&[&eds]));
    assert!(event_data.data_set_count() > 0);
}

#[test]
fn time_span_valid_construction_and_setters() {
    let start = "2016-01-01T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let end = "2016-01-03T12:00:00Z".parse::<DateTimeOffset>().unwrap();

    let time_span = TsdTimeSpan::new(start, end).unwrap();
    assert_eq!(time_span.start().to_string(), start.to_string());
    assert_eq!(time_span.end().to_string(), end.to_string());

    let new_start = "2016-01-02T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let time_span = time_span
        .with_start(new_start)
        .expect("new_start should be before end");
    assert_eq!(time_span.start().to_string(), new_start.to_string());

    let new_end = "2016-01-04T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let time_span = time_span
        .with_end(new_end)
        .expect("new_end should be after start");
    assert_eq!(time_span.end().to_string(), new_end.to_string());
}

#[test]
fn time_span_invalid_range_returns_err() {
    let start = "2016-01-03T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let end = "2016-01-01T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    assert!(TsdTimeSpan::new(start, end).is_err());

    let valid_start = "2016-01-01T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let valid_end = "2016-01-03T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let ts = TsdTimeSpan::new(valid_start, valid_end).unwrap();
    assert!(ts
        .with_start("2016-01-04T12:00:00Z".parse::<DateTimeOffset>().unwrap())
        .is_err());

    let ts = TsdTimeSpan::new(valid_start, valid_end).unwrap();
    assert!(ts
        .with_end("2015-12-31T12:00:00Z".parse::<DateTimeOffset>().unwrap())
        .is_err());
}

#[test]
fn tabular_data_valid_validation() {
    let id1 = TsdChannelId::from_string("0010").unwrap();
    let id2 = TsdChannelId::from_string("0020").unwrap();
    let ts = "2016-01-01T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let ds = TabularDataSet::new(ts, &["100.0", "200.0"], Some(&["0", "0"]));
    let tabular = TabularData::new(&[&id1, &id2], &[&ds]);
    assert!(tabular.validate());
}

#[test]
fn tabular_data_dimension_mismatch_fails_validation() {
    let id1 = TsdChannelId::from_string("0010").unwrap();
    let id2 = TsdChannelId::from_string("0020").unwrap();
    let ts = "2016-01-01T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let ds = TabularDataSet::new(ts, &["100.0", "200.0", "300.0"], Some(&["0", "0", "0"]));
    let tabular = TabularData::new(&[&id1, &id2], &[&ds]);
    assert!(!tabular.validate());
}

#[test]
fn event_data_operations() {
    let event_data = EventData::new(None);
    assert_eq!(event_data.data_set_count(), 0);

    let ts = "2016-01-01T12:00:01Z".parse::<DateTimeOffset>().unwrap();
    let id = TsdChannelId::from_string("0010").unwrap();
    let eds1 = EventDataSet::new(ts, &id, "HIGH", Some("0"));
    let event_data = event_data.with_data_sets(&[&eds1]);
    assert_eq!(event_data.data_set_count(), 1);

    let eds2 = EventDataSet::new(ts, &id, "LOW", Some("0"));
    let event_data = event_data.with_data_sets(&[&eds1, &eds2]);
    assert_eq!(event_data.data_set_count(), 2);

    let event_data = event_data.without_data_sets();
    assert_eq!(event_data.data_set_count(), 0);
}

#[test]
fn config_ref_construction_and_setters() {
    let ts = "2016-01-01T00:00:00Z".parse::<DateTimeOffset>().unwrap();
    let config_ref = TsdConfigRef::new("TestConfig.xml", ts);
    assert_eq!(config_ref.id(), "TestConfig.xml");
    assert_eq!(config_ref.time_stamp().to_string(), ts.to_string());

    let config_ref = config_ref.with_id("NewConfig.xml");
    assert_eq!(config_ref.id(), "NewConfig.xml");

    let new_ts = "2016-01-02T00:00:00Z".parse::<DateTimeOffset>().unwrap();
    let config_ref = config_ref.with_time_stamp(new_ts);
    assert_eq!(config_ref.time_stamp().to_string(), new_ts.to_string());
}

#[test]
fn tabular_data_set_construction_with_and_without_quality() {
    let ts = "2016-01-01T12:00:00Z".parse::<DateTimeOffset>().unwrap();
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
    let ts = "2016-01-01T12:00:01Z".parse::<DateTimeOffset>().unwrap();
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

    let ts1 = "2016-01-01T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let ts2 = "2016-01-02T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let ds1 = TabularDataSet::new(ts1, &["100.0", "200.0"], Some(&["0", "0"]));
    let ds2 = TabularDataSet::new(ts2, &["105.0", "210.0"], Some(&["0", "0"]));
    let tabular = TabularData::new(&[&all_ids[0], &all_ids[1]], &[&ds1, &ds2]);

    let data_config = TsdConfigRef::new(
        "DataChannelList.xml",
        "2016-01-01T00:00:00Z".parse::<DateTimeOffset>().unwrap(),
    );
    let ts_data = TimeSeriesData::new()
        .with_data_configuration(&data_config)
        .with_tabular_data(&[&tabular]);

    let result = ts_data.validate(&dc_package, ok_callback().as_ref(), ok_callback().as_ref());
    assert!(result.is_valid());
}

#[test]
fn invalid_data_channel_id_not_found_in_dcl() {
    let dc_package = create_valid_fully_custom_dcl();

    let ts = "2016-01-01T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let invalid_id = TsdChannelId::from_string("9999").unwrap();
    let ds = TabularDataSet::new(ts, &["100.0"], None);
    let tabular = TabularData::new(&[&invalid_id], &[&ds]);

    let data_config = TsdConfigRef::new(
        "DataChannelList.xml",
        "2016-01-01T00:00:00Z".parse::<DateTimeOffset>().unwrap(),
    );
    let ts_data = TimeSeriesData::new()
        .with_data_configuration(&data_config)
        .with_tabular_data(&[&tabular]);

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

    let ts = "2016-01-01T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let ds = TabularDataSet::new(ts, &["invalid_not_a_number"], None);
    let tabular = TabularData::new(&[&all_ids[0]], &[&ds]);

    let data_config = TsdConfigRef::new(
        "DataChannelList.xml",
        "2016-01-01T00:00:00Z".parse::<DateTimeOffset>().unwrap(),
    );
    let ts_data = TimeSeriesData::new()
        .with_data_configuration(&data_config)
        .with_tabular_data(&[&tabular]);

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

    let ts1 = "2016-01-01T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let ds = TabularDataSet::new(ts1, &["100.0", "200.0"], Some(&["0", "0"]));
    let tabular = TabularData::new(&[&all_ids[0], &all_ids[1]], &[&ds]);

    let wrong_config = TsdConfigRef::new(
        "WrongDataChannelList.xml",
        "2016-01-01T00:00:00Z".parse::<DateTimeOffset>().unwrap(),
    );
    let ts_data = TimeSeriesData::new()
        .with_data_configuration(&wrong_config)
        .with_tabular_data(&[&tabular]);

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
        "2016-01-01T00:00:00Z".parse::<DateTimeOffset>().unwrap(),
    );
    let ts_data = TimeSeriesData::new().with_data_configuration(&data_config);

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

    let ts1 = "2016-01-01T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let ts2 = "2016-01-02T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let ds1 = TabularDataSet::new(ts1, &["100.0", "200.0"], Some(&["0", "0"]));
    let ds2 = TabularDataSet::new(ts2, &["105.0", "210.0"], Some(&["0", "0"]));
    let tabular = TabularData::new(&[&all_ids[0], &all_ids[1]], &[&ds1, &ds2]);

    let data_config = TsdConfigRef::new(
        "DataChannelList.xml",
        "2016-01-01T00:00:00Z".parse::<DateTimeOffset>().unwrap(),
    );
    let ts_data = TimeSeriesData::new()
        .with_data_configuration(&data_config)
        .with_tabular_data(&[&tabular]);

    let on_tabular: &ValidateCallback = &|_ts, _dc, _val, _qual| {
        ValidateResult::Invalid(vec!["Custom business rule violation".to_string()])
    };
    let result = ts_data.validate(&dc_package, on_tabular, ok_callback().as_ref());
    assert!(!result.is_valid());
    assert!(!result.errors().is_empty());
    assert!(
        result.errors()[0].contains("Custom business rule violation"),
        "expected 'Custom business rule violation' in: {}",
        result.errors()[0]
    );
}

#[test]
fn custom_callback_multi_message_rejection() {
    let dc_package = create_valid_fully_custom_dcl();
    let all_ids = build_ts_channel_ids(&dc_package);

    let ts1 = "2016-01-01T12:00:00Z".parse::<DateTimeOffset>().unwrap();
    let ds1 = TabularDataSet::new(ts1, &["100.0"], Some(&["0"]));
    let tabular = TabularData::new(&[&all_ids[0]], &[&ds1]);

    let data_config = TsdConfigRef::new(
        "DataChannelList.xml",
        "2016-01-01T00:00:00Z".parse::<DateTimeOffset>().unwrap(),
    );
    let ts_data = TimeSeriesData::new()
        .with_data_configuration(&data_config)
        .with_tabular_data(&[&tabular]);

    let on_tabular: &ValidateCallback = &|_ts, _dc, _val, _qual| {
        ValidateResult::Invalid(vec![
            "first business rule violation".to_string(),
            "second business rule violation".to_string(),
        ])
    };
    let result = ts_data.validate(&dc_package, on_tabular, ok_callback().as_ref());
    assert!(!result.is_valid());
    assert_eq!(
        result.errors(),
        &[
            "first business rule violation",
            "second business rule violation"
        ]
    );
}

#[test]
fn tsd_package_with_header() {
    let ship_id = ShipId::from_string("IMO1234567").unwrap();
    let header = TsdHeader::new(&ship_id);
    let pkg = TsdPackage::new(None, &[]);
    assert!(!pkg.has_header());
    let pkg2 = pkg.with_header(&header);
    assert!(pkg2.has_header());
}

#[test]
fn tsd_header_date_created_and_modified() {
    let ship_id = ShipId::from_string("IMO1234567").unwrap();
    let ts = DateTimeOffset::utc_now();
    let h = TsdHeader::new(&ship_id)
        .with_date_created(ts)
        .with_date_modified(ts);
    assert!(h.date_created().is_some());
    assert!(h.date_modified().is_some());
    let h2 = h.without_date_created().without_date_modified();
    assert!(h2.date_created().is_none());
    assert!(h2.date_modified().is_none());
}

#[test]
fn tsd_header_without_author() {
    let ship_id = ShipId::from_string("IMO1234567").unwrap();
    let h = TsdHeader::new(&ship_id).with_author("Alice");
    assert_eq!(h.author().as_deref(), Some("Alice"));
    let h2 = h.without_author();
    assert!(h2.author().is_none());
}

#[test]
fn tsd_header_system_configuration() {
    let ship_id = ShipId::from_string("IMO1234567").unwrap();
    let ts = "2024-01-01T00:00:00Z".parse::<DateTimeOffset>().unwrap();
    let cfg = TsdConfigRef::new("cfg-1", ts);
    let h = TsdHeader::new(&ship_id).with_system_configuration(&[&cfg]);
    assert_eq!(h.system_configuration_count(), 1);
    assert!(h.system_configuration_at(0).is_some());
    assert!(h.system_configuration_at(1).is_none());
    let h2 = h.without_system_configuration();
    assert_eq!(h2.system_configuration_count(), 0);
}

#[test]
fn tsd_header_custom_headers() {
    let ship_id = ShipId::from_string("IMO1234567").unwrap();
    let h = TsdHeader::new(&ship_id).with_custom_headers(SerializableDocument::object());
    assert!(h.custom_headers().is_some());
    let h2 = h.without_custom_headers();
    assert!(h2.custom_headers().is_none());
}

#[test]
fn time_series_data_without_tabular_and_data_configuration() {
    let ts = "2024-01-01T00:00:00Z".parse::<DateTimeOffset>().unwrap();
    let cfg = TsdConfigRef::new("cfg-1", ts);
    let ch = TsdChannelId::from_string(
        "/dnv-v2/vis-3-4a/411.1-1/C101.63/S206/meta/qty-temperature/cnt-cooling.air",
    )
    .unwrap();
    let ds = TabularDataSet::new(DateTimeOffset::utc_now(), &["1.0"], None);
    let td = TabularData::new(&[&ch], &[&ds]);
    let tsd = TimeSeriesData::new()
        .with_data_configuration(&cfg)
        .with_tabular_data(&[&td]);
    assert!(tsd.data_configuration().is_some());
    assert_eq!(tsd.tabular_data_count(), 1);
    let tsd2 = tsd.without_data_configuration().without_tabular_data();
    assert!(tsd2.data_configuration().is_none());
    assert_eq!(tsd2.tabular_data_count(), 0);
}

#[test]
fn time_series_data_with_and_without_event_data() {
    let ch = TsdChannelId::from_string(
        "/dnv-v2/vis-3-4a/411.1-1/C101.63/S206/meta/qty-temperature/cnt-cooling.air",
    )
    .unwrap();
    let ts = DateTimeOffset::utc_now();
    let eds = EventDataSet::new(ts, &ch, "42.0", None);
    let ed = EventData::new(Some(&[&eds]));
    let tsd = TimeSeriesData::new().with_event_data(&ed);
    assert!(tsd.event_data().is_some());
    let tsd2 = tsd.without_event_data();
    assert!(tsd2.event_data().is_none());
}

#[test]
fn tabular_data_accessors() {
    let ch = TsdChannelId::from_string(
        "/dnv-v2/vis-3-4a/411.1-1/C101.63/S206/meta/qty-temperature/cnt-cooling.air",
    )
    .unwrap();
    let ds = TabularDataSet::new(DateTimeOffset::utc_now(), &["1.0"], None);
    let td = TabularData::new(&[&ch], &[&ds]);
    assert_eq!(td.channel_id_count(), 1);
    assert_eq!(td.data_set_count(), 1);
    assert!(td.channel_id_at(0).is_some());
    assert!(td.channel_id_at(1).is_none());
    assert!(td.data_set_at(0).is_some());
    assert!(td.data_set_at(1).is_none());
    assert!(td.validate());
}

#[test]
fn tabular_data_set_values_and_quality() {
    let ts = DateTimeOffset::utc_now();
    let ds = TabularDataSet::new(ts, &["1.0", "2.0"], Some(&["Good", "Bad"]));
    assert_eq!(ds.time_stamp().ticks(), ts.ticks());
    let vals = ds.values();
    assert_eq!(vals.len(), 2);
    assert_eq!(vals[0], "1.0");
    let qual = ds.quality().expect("quality set");
    assert_eq!(qual[0], "Good");
    let ds2 = ds.without_quality();
    assert!(ds2.quality().is_none());
}

#[test]
fn event_data_set_accessors() {
    let ch = TsdChannelId::from_string(
        "/dnv-v2/vis-3-4a/411.1-1/C101.63/S206/meta/qty-temperature/cnt-cooling.air",
    )
    .unwrap();
    let ts = DateTimeOffset::utc_now();
    let eds = EventDataSet::new(ts, &ch, "42.0", Some("Good"));
    assert_eq!(eds.time_stamp().ticks(), ts.ticks());
    assert_eq!(eds.value(), "42.0");
    assert_eq!(eds.quality().as_deref(), Some("Good"));
    let eds2 = eds.without_quality();
    assert!(eds2.quality().is_none());
}

#[test]
fn event_data_accessors() {
    let ch = TsdChannelId::from_string(
        "/dnv-v2/vis-3-4a/411.1-1/C101.63/S206/meta/qty-temperature/cnt-cooling.air",
    )
    .unwrap();
    let ts = DateTimeOffset::utc_now();
    let eds = EventDataSet::new(ts, &ch, "1.0", None);
    let ed = EventData::new(Some(&[&eds]));
    assert_eq!(ed.data_set_count(), 1);
    assert!(ed.data_set_at(0).is_some());
    assert!(ed.data_set_at(1).is_none());
    let ed2 = ed.without_data_sets();
    assert_eq!(ed2.data_set_count(), 0);
}

#[test]
fn time_series_data_package_is_empty() {
    let pkg = TsdPackage::new(None, &[]);
    let tsdp = TimeSeriesDataPackage::new(&pkg);
    assert!(tsdp.is_empty());
    assert_eq!(tsdp.time_series_data_count(), 0);
}

#[test]
fn time_series_data_with_custom_data_kinds() {
    let doc = SerializableDocument::object();
    let tsd = TimeSeriesData::new().with_custom_data_kinds(doc);
    let tsd2 = tsd.without_custom_data_kinds();
    let _ = tsd2;
}
