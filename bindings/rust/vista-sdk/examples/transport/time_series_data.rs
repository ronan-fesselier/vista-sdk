use vista_sdk::core::codebook_name::CodebookName;
use vista_sdk::core::gmod_path::GmodPath;
use vista_sdk::core::local_id_builder::LocalIdBuilder;
use vista_sdk::core::vis::Vis;
use vista_sdk::transport::datachannel::data_channel::{
    ConfigurationReference as DclConfigRef, DataChannel, DataChannelId as DclChannelId,
    DataChannelList, DataChannelListPackage, DataChannelType, Format, Header as DclHeader,
    Package as DclPackage, Property, Range, Restriction, Unit,
};
use vista_sdk::transport::serializable_document::SerializableDocument;
use vista_sdk::transport::ship_id::ShipId;
use vista_sdk::transport::timeseries::data_channel_id::TsdChannelId;
use vista_sdk::transport::timeseries::time_series_data::{
    EventData, EventDataSet, TabularData, TabularDataSet, TimeSeriesData, TimeSeriesDataPackage,
    TsdConfigRef, TsdHeader, TsdPackage, TsdTimeSpan, ValidateCallback, ValidateResult,
};
use vista_sdk::transport::timeseries::time_series_data_dto;
use vista_sdk::transport::timeseries::time_series_data_json;
use vista_sdk::types::date_time_offset::DateTimeOffset;

fn main() {
    println!("=== vista-sdk TimeSeriesData Sample ===\n");

    {
        println!("1. Header: Creating TimeSeriesData package headers");
        println!("-----------------------------------------------------");

        let ship_id = ShipId::from_string("IMO9074729").expect("valid ship ID");

        let start = "2026-08-27T10:00:00Z".parse::<DateTimeOffset>().unwrap();
        let end = "2026-08-27T14:00:00Z".parse::<DateTimeOffset>().unwrap();
        let time_span = TsdTimeSpan::new(start, end).expect("valid time span");

        let date_created = "2026-08-27T10:00:00Z".parse::<DateTimeOffset>().unwrap();
        let date_modified = "2026-08-27T14:30:00Z".parse::<DateTimeOffset>().unwrap();

        let sys_config = TsdConfigRef::new(
            "SystemConfiguration.xml",
            "2026-08-27T00:00:00Z".parse::<DateTimeOffset>().unwrap(),
        );

        let header = TsdHeader::new(&ship_id)
            .with_time_span(&time_span)
            .with_date_created(date_created)
            .with_date_modified(date_modified)
            .with_author("Vista SDK Sample")
            .with_system_configuration(&[&sys_config]);

        println!("Created header:");
        println!("  Ship ID       : {}", ship_id);
        if let Some(ts) = header.time_span() {
            println!("  Time Span     : {} - {}", ts.start(), ts.end());
        }
        println!("  Date Created  : {}", header.date_created().unwrap());
        println!("  Date Modified : {}", header.date_modified().unwrap());
        println!("  Author        : {}", header.author().unwrap());
        println!(
            "  System Configs: {} entries",
            header.system_configuration_count()
        );
        println!();
    }

    {
        println!("2. TabularData: Creating tabular time series data");
        println!("----------------------------------------------------");

        let channel_id1 = TsdChannelId::from_string("Temperature").expect("valid channel ID");
        let channel_id2 = TsdChannelId::from_string("Pressure").expect("valid channel ID");

        let ts1 = "2026-08-27T10:00:00Z".parse::<DateTimeOffset>().unwrap();
        let ts2 = "2026-08-27T11:00:00Z".parse::<DateTimeOffset>().unwrap();
        let ts3 = "2026-08-27T12:00:00Z".parse::<DateTimeOffset>().unwrap();

        let ds1 = TabularDataSet::new(ts1, &["100.5", "200.0"], Some(&["0", "0"]));
        let ds2 = TabularDataSet::new(ts2, &["105.2", "205.5"], Some(&["0", "0"]));
        let ds3 = TabularDataSet::new(ts3, &["110.0", "210.0"], Some(&["0", "0"]));

        let tabular_data = TabularData::new(&[&channel_id1, &channel_id2], &[&ds1, &ds2, &ds3]);

        println!("Created TabularData:");
        println!("  Data Channels: {}", tabular_data.channel_id_count());
        println!("  Data Sets    : {}", tabular_data.data_set_count());
        println!("  Validation   : {}", tabular_data.validate());
        println!();
    }

    {
        println!("3. EventData: Creating event-based time series data");
        println!("------------------------------------------------------");

        let channel_id = TsdChannelId::from_string("AlarmStatus").expect("valid channel ID");
        let ts1 = "2026-08-27T10:30:00Z".parse::<DateTimeOffset>().unwrap();
        let ts2 = "2026-08-27T11:45:00Z".parse::<DateTimeOffset>().unwrap();

        let eds1 = EventDataSet::new(ts1, &channel_id, "HIGH", Some("0"));
        let eds2 = EventDataSet::new(ts2, &channel_id, "NORMAL", Some("0"));
        let event_data = EventData::new(Some(&[&eds1, &eds2]));

        println!("Created EventData:");
        println!("  Event Data Sets: {}", event_data.data_set_count());
        println!();
    }

    {
        println!("4. TimeSeriesData: Combining tabular and event data");
        println!("------------------------------------------------------");

        let data_config = TsdConfigRef::new(
            "DataChannelList.xml",
            "2026-08-27T00:00:00Z".parse::<DateTimeOffset>().unwrap(),
        );

        let ch1 = TsdChannelId::from_string("Temp1").expect("valid channel ID");
        let ch2 = TsdChannelId::from_string("Press1").expect("valid channel ID");
        let ts = "2026-08-27T10:00:00Z".parse::<DateTimeOffset>().unwrap();
        let ds = TabularDataSet::new(ts, &["100.0", "200.0"], None);
        let tabular_data = TabularData::new(&[&ch1, &ch2], &[&ds]);

        let alarm_ch = TsdChannelId::from_string("Alarm1").expect("valid channel ID");
        let event_ts = "2026-08-27T10:30:00Z".parse::<DateTimeOffset>().unwrap();
        let eds = EventDataSet::new(event_ts, &alarm_ch, "HIGH", Some("0"));
        let event_data = EventData::new(Some(&[&eds]));

        let ts_data = TimeSeriesData::new()
            .with_data_configuration(&data_config)
            .with_tabular_data(&[&tabular_data])
            .with_event_data(&event_data);

        println!("Created TimeSeriesData:");
        if let Some(cfg) = ts_data.data_configuration() {
            println!("  DataConfig  : {} @ {}", cfg.id(), cfg.time_stamp());
        }
        for i in 0..ts_data.tabular_data_count() {
            let tab = ts_data.tabular_data_at(i).unwrap();
            println!(
                "  TabularData [{}]: {} channel(s), {} data set(s)",
                i,
                tab.channel_id_count(),
                tab.data_set_count()
            );
            let channel_labels: Vec<String> = (0..tab.channel_id_count())
                .filter_map(|j| tab.channel_id_at(j))
                .map(|id| id.to_string())
                .collect();
            println!("    channels: {}", channel_labels.join(", "));
            for j in 0..tab.data_set_count() {
                let ds = tab.data_set_at(j).unwrap();
                let quality_str = ds
                    .quality()
                    .map(|q| q.join(", "))
                    .unwrap_or_else(|| "-".to_owned());
                println!(
                    "    [{}] ts={} values=[{}] quality={}",
                    j,
                    ds.time_stamp(),
                    ds.values().join(", "),
                    quality_str
                );
            }
        }
        if let Some(ev) = ts_data.event_data() {
            println!("  EventData: {} event(s)", ev.data_set_count());
            for i in 0..ev.data_set_count() {
                let eds = ev.data_set_at(i).unwrap();
                println!(
                    "    [{}] ts={} channel={} value={} quality={}",
                    i,
                    eds.time_stamp(),
                    eds.channel_id(),
                    eds.value(),
                    eds.quality().as_deref().unwrap_or("-")
                );
            }
        }
        println!();
    }

    {
        println!("5. Package: Creating complete TimeSeriesData package");
        println!("-------------------------------------------------------");

        let ship_id = ShipId::from_string("IMO9074729").expect("valid ship ID");
        let start = "2026-08-27T10:00:00Z".parse::<DateTimeOffset>().unwrap();
        let end = "2026-08-27T14:00:00Z".parse::<DateTimeOffset>().unwrap();
        let time_span = TsdTimeSpan::new(start, end).expect("valid time span");

        let header = TsdHeader::new(&ship_id)
            .with_time_span(&time_span)
            .with_date_created("2026-08-27T10:00:00Z".parse::<DateTimeOffset>().unwrap())
            .with_date_modified("2026-08-27T14:30:00Z".parse::<DateTimeOffset>().unwrap())
            .with_author("Vista SDK");

        let data_config = TsdConfigRef::new(
            "DataChannelList.xml",
            "2026-08-27T00:00:00Z".parse::<DateTimeOffset>().unwrap(),
        );
        let ch = TsdChannelId::from_string("Sensor1").expect("valid channel ID");
        let ts = "2026-08-27T10:00:00Z".parse::<DateTimeOffset>().unwrap();
        let ds = TabularDataSet::new(ts, &["100.0"], None);
        let tabular_data = TabularData::new(&[&ch], &[&ds]);

        let ts_data = TimeSeriesData::new()
            .with_data_configuration(&data_config)
            .with_tabular_data(&[&tabular_data]);

        let package = TsdPackage::new(Some(&header), &[&ts_data]);

        println!("Created TimeSeriesDataPackage:");
        println!("  Ship ID        : {}", ship_id);
        if let Some(ts) = header.time_span() {
            println!("  Time Span      : {} - {}", ts.start(), ts.end());
        }
        println!(
            "  Date Created   : {}",
            header
                .date_created()
                .map(|d| d.to_string())
                .unwrap_or_else(|| "-".to_owned())
        );
        println!(
            "  Author         : {}",
            header.author().unwrap_or_else(|| "-".to_owned())
        );
        println!(
            "  TimeSeriesData : {} block(s)",
            package.time_series_data_count()
        );
        for i in 0..package.time_series_data_count() {
            let tsd = package.time_series_data_at(i).unwrap();
            if let Some(cfg) = tsd.data_configuration() {
                println!("    [{}] DataConfig   : {}", i, cfg.id());
            }
            println!(
                "    [{}] TabularData  : {} block(s)",
                i,
                tsd.tabular_data_count()
            );
            if let Some(ev) = tsd.event_data() {
                println!(
                    "    [{}] EventData    : {} event(s)",
                    i,
                    ev.data_set_count()
                );
            }
        }
        println!();
    }

    {
        println!("6. JSON Serialization: Round-trip serialization");
        println!("--------------------------------------------------");

        let ship_id = ShipId::from_string("IMO1234567").expect("valid ship ID");
        let header = TsdHeader::new(&ship_id)
            .with_date_created("2026-08-27T10:00:00Z".parse::<DateTimeOffset>().unwrap());

        let data_config = TsdConfigRef::new(
            "DataChannelList.xml",
            "2026-08-27T00:00:00Z".parse::<DateTimeOffset>().unwrap(),
        );
        let ch = TsdChannelId::from_string("Ch1").expect("valid channel ID");
        let ts = "2026-08-27T10:00:00Z".parse::<DateTimeOffset>().unwrap();
        let ds = TabularDataSet::new(ts, &["42.0"], None);
        let tabular_data = TabularData::new(&[&ch], &[&ds]);

        let ts_data = TimeSeriesData::new()
            .with_data_configuration(&data_config)
            .with_tabular_data(&[&tabular_data]);

        let package = TsdPackage::new(Some(&header), &[&ts_data]);
        let original_package = TimeSeriesDataPackage::new(&package);

        let json_str = time_series_data_json::to_json(&original_package, true);
        println!("Serialized ({} bytes):", json_str.len());
        println!("{}", json_str);

        let deserialized =
            time_series_data_json::from_json(&json_str).expect("roundtrip must parse");
        println!("Round-trip check:");
        let n = deserialized.time_series_data_count();
        println!(
            "  TimeSeriesData blocks: {} (original: {})",
            n,
            package.time_series_data_count()
        );
        for i in 0..n {
            if let Some(tsd) = package.time_series_data_at(i) {
                if let Some(cfg) = tsd.data_configuration() {
                    println!("  [{}] DataConfig : {}", i, cfg.id());
                }
                for j in 0..tsd.tabular_data_count() {
                    let tab = tsd.tabular_data_at(j).unwrap();
                    let channels: Vec<String> = (0..tab.channel_id_count())
                        .filter_map(|k| tab.channel_id_at(k))
                        .map(|id| id.to_string())
                        .collect();
                    for k in 0..tab.data_set_count() {
                        let ds = tab.data_set_at(k).unwrap();
                        println!(
                            "  [{}] channels=[{}] ts={} values=[{}]",
                            j,
                            channels.join(", "),
                            ds.time_stamp(),
                            ds.values().join(", ")
                        );
                    }
                }
            }
        }
        println!();
    }

    {
        println!("7. JSON Serialization: TabularData + EventData combined");
        println!("--------------------------------------------------");

        let ship_id = ShipId::from_string("IMO1234567").expect("valid ship ID");
        let header = TsdHeader::new(&ship_id)
            .with_date_created("2026-08-27T10:00:00Z".parse::<DateTimeOffset>().unwrap());

        let data_config = TsdConfigRef::new(
            "DataChannelList.xml",
            "2026-08-27T00:00:00Z".parse::<DateTimeOffset>().unwrap(),
        );
        let ch = TsdChannelId::from_string("Ch1").expect("valid channel ID");
        let ts = "2026-08-27T10:00:00Z".parse::<DateTimeOffset>().unwrap();
        let ds = TabularDataSet::new(ts, &["42.0"], None);
        let tabular_data = TabularData::new(&[&ch], &[&ds]);

        let alarm_ch = TsdChannelId::from_string("Alarm1").expect("valid channel ID");
        let event_ts = "2026-08-27T10:00:42Z".parse::<DateTimeOffset>().unwrap();
        let eds = EventDataSet::new(event_ts, &alarm_ch, "alarm.active", Some("0"));
        let event_data = EventData::new(Some(&[&eds]));

        let ts_data = TimeSeriesData::new()
            .with_data_configuration(&data_config)
            .with_tabular_data(&[&tabular_data])
            .with_event_data(&event_data);

        let package = TsdPackage::new(Some(&header), &[&ts_data]);
        let original_package = TimeSeriesDataPackage::new(&package);

        let json_str = time_series_data_json::to_json(&original_package, true);
        println!("Serialized to JSON ({} bytes)", json_str.len());
        println!("{}", json_str);
        println!();
    }

    {
        println!("8. Cross-Validation: Validating against DataChannelList");
        println!("----------------------------------------------------------");

        let vis = Vis::instance();
        let gmod = vis.gmod(vis.latest()).expect("valid gmod");
        let locations = vis.locations(vis.latest()).expect("valid locations");
        let codebooks = vis.codebooks(vis.latest()).expect("valid codebooks");

        let ship_id = ShipId::from_string("IMO9074729").expect("valid ship ID");
        let config_ref = DclConfigRef::new(
            "DataChannelList.xml",
            "2026-08-27T00:00:00Z".parse::<DateTimeOffset>().unwrap(),
        );

        let primary_item =
            GmodPath::from_short_path("411.1/C101.31-2", gmod, locations).expect("valid path");
        let qty_tag = codebooks[CodebookName::Quantity]
            .create_tag("temperature")
            .expect("valid tag");

        let local_id = LocalIdBuilder::create(vis.latest())
            .with_primary_item(&primary_item)
            .with_metadata_tag(&qty_tag)
            .build()
            .expect("valid local id");

        let dcl_channel_id = DclChannelId::new(&local_id).with_short_id("TempSensor");

        let restriction = Restriction::new().with_fraction_digits(1);
        let format = Format::new("Decimal").with_restriction(&restriction);
        let dc_type = DataChannelType::new("Inst");
        let range = Range::new(0.0, 200.0);
        let unit = Unit::new("°C");
        let property = Property::new(&dc_type, &format)
            .with_range(&range)
            .with_unit(&unit);

        let data_channel = DataChannel::new(&dcl_channel_id, &property);
        let mut dcl = DataChannelList::new();
        dcl.add(&data_channel);

        let dcl_header = DclHeader::new(&ship_id, &config_ref);
        let dcl_pkg = DclPackage::new(&dcl_header, &dcl);
        let dcl_package = DataChannelListPackage::new(&dcl_pkg);

        let ts_config = TsdConfigRef::new(
            "DataChannelList.xml",
            "2026-08-27T00:00:00Z".parse::<DateTimeOffset>().unwrap(),
        );
        let ts_ch = TsdChannelId::from_string("TempSensor").expect("valid channel ID");
        let ts_event_ts = "2026-08-27T10:15:00Z".parse::<DateTimeOffset>().unwrap();
        let ts = "2026-08-27T10:00:00Z".parse::<DateTimeOffset>().unwrap();
        let ds = TabularDataSet::new(ts, &["100.5"], None);
        let tabular_data = TabularData::new(&[&ts_ch], &[&ds]);
        let eds = EventDataSet::new(ts_event_ts, &ts_ch, "105.2", Some("0"));
        let event_data = EventData::new(Some(&[&eds]));

        let ts_data = TimeSeriesData::new()
            .with_data_configuration(&ts_config)
            .with_tabular_data(&[&tabular_data])
            .with_event_data(&event_data);

        let on_tabular: &ValidateCallback = &|ts, dc, value, quality| {
            println!(
                "  Tabular data at {} | channel: {} | value: {} | quality: {}",
                ts,
                dc.channel_id().short_id().as_deref().unwrap_or("?"),
                value,
                quality.unwrap_or("-")
            );
            ValidateResult::Ok
        };
        let on_event: &ValidateCallback = &|ts, dc, value, quality| {
            println!(
                "  Event data at {}   | channel: {} | value: {} | quality: {}",
                ts,
                dc.channel_id().short_id().as_deref().unwrap_or("?"),
                value,
                quality.unwrap_or("-")
            );
            ValidateResult::Ok
        };

        let result = ts_data.validate(&dcl_package, on_tabular, on_event);
        println!("Cross-validation result:");
        println!("  Is valid: {}", result.is_valid());
        if !result.is_valid() {
            for err in result.errors() {
                println!("    - {}", err);
            }
        }
        println!();
    }

    {
        println!("9. Custom Validation: Business rule callbacks");
        println!("------------------------------------------------");

        let vis = Vis::instance();
        let gmod = vis.gmod(vis.latest()).expect("valid gmod");
        let locations = vis.locations(vis.latest()).expect("valid locations");
        let codebooks = vis.codebooks(vis.latest()).expect("valid codebooks");

        let ship_id = ShipId::from_string("IMO1234567").expect("valid ship ID");
        let config_ref = DclConfigRef::new(
            "DataChannelList.xml",
            "2026-08-27T00:00:00Z".parse::<DateTimeOffset>().unwrap(),
        );

        let primary_item =
            GmodPath::from_short_path("411.1/C101", gmod, locations).expect("valid path");
        let qty_tag = codebooks[CodebookName::Quantity]
            .create_tag("temperature")
            .expect("valid tag");

        let local_id = LocalIdBuilder::create(vis.latest())
            .with_primary_item(&primary_item)
            .with_metadata_tag(&qty_tag)
            .build()
            .expect("valid local id");

        let dcl_channel_id = DclChannelId::new(&local_id).with_short_id("ExhaustTemp");

        let dc_type = DataChannelType::new("Inst");
        let format = Format::new("Decimal");
        let range = Range::new(0.0, 450.0);
        let unit = Unit::new("deg C");
        let property = Property::new(&dc_type, &format)
            .with_range(&range)
            .with_unit(&unit)
            .with_alert_priority("high-temperature");

        let data_channel = DataChannel::new(&dcl_channel_id, &property);
        let mut dcl = DataChannelList::new();
        dcl.add(&data_channel);

        let dcl_header = DclHeader::new(&ship_id, &config_ref);
        let dcl_pkg = DclPackage::new(&dcl_header, &dcl);
        let dcl_package = DataChannelListPackage::new(&dcl_pkg);

        let ts_config = TsdConfigRef::new(
            "DataChannelList.xml",
            "2026-08-27T00:00:00Z".parse::<DateTimeOffset>().unwrap(),
        );
        let ts_ch = TsdChannelId::from_string("ExhaustTemp").expect("valid channel ID");
        let ts = "2026-08-27T10:00:00Z".parse::<DateTimeOffset>().unwrap();
        let ds = TabularDataSet::new(ts, &["420.0"], None);
        let tabular_data = TabularData::new(&[&ts_ch], &[&ds]);

        let event_ts = "2026-08-27T11:00:00Z".parse::<DateTimeOffset>().unwrap();
        let eds = EventDataSet::new(event_ts, &ts_ch, "380.0", Some("0"));
        let event_data = EventData::new(Some(&[&eds]));

        let ts_data = TimeSeriesData::new()
            .with_data_configuration(&ts_config)
            .with_tabular_data(&[&tabular_data])
            .with_event_data(&event_data);

        let on_tabular: &ValidateCallback = &|ts, dc, value, _quality| {
            println!(
                "  Exhaust temp tabular at {} | channel: {} | value: {}",
                ts,
                dc.channel_id().short_id().as_deref().unwrap_or("?"),
                value
            );
            ValidateResult::Invalid(vec![
                "Value 420.0 triggered alarm 'high-temperature': approaching upper limit of 450 deg C"
                    .to_string(),
                "Value 420.0 exceeds recommended cruise threshold of 400 deg C".to_string(),
            ])
        };
        let on_event: &ValidateCallback = &|ts, dc, value, _quality| {
            println!(
                "  Exhaust temp event at {}   | channel: {} | value: {}",
                ts,
                dc.channel_id().short_id().as_deref().unwrap_or("?"),
                value
            );
            ValidateResult::Ok
        };

        let result = ts_data.validate(&dcl_package, on_tabular, on_event);
        println!("Custom validation result:");
        println!("  Is valid: {}", result.is_valid());
        if !result.is_valid() {
            for err in result.errors() {
                println!("    - {}", err);
            }
        }
        println!();
    }

    {
        println!("10. CustomHeaders & CustomDataKinds: Extension data support");
        println!("-------------------------------------------------------------");

        let mut custom_headers = SerializableDocument::object();
        custom_headers.set("version", SerializableDocument::from_string("1.2.3"));
        custom_headers.set("recordCount", SerializableDocument::from_i64(42));
        custom_headers.set("isCompressed", SerializableDocument::from_bool(true));
        custom_headers.set("temperature", SerializableDocument::from_f64(98.6));
        custom_headers.set(
            "createdAt",
            SerializableDocument::from_string("2026-08-27T10:00:00Z"),
        );

        let ship_id = ShipId::from_string("IMO1234567").expect("valid ship ID");
        let header = TsdHeader::new(&ship_id)
            .with_date_created("2026-08-27T10:00:00Z".parse::<DateTimeOffset>().unwrap())
            .with_author("CustomData Demo")
            .with_custom_headers(custom_headers);

        let mut custom_data_kinds = SerializableDocument::object();
        custom_data_kinds.set(
            "sensorModel",
            SerializableDocument::from_string("TempSensor-XYZ-2000"),
        );
        custom_data_kinds.set(
            "calibrationDate",
            SerializableDocument::from_string("2026-08-27T00:00:00Z"),
        );
        custom_data_kinds.set("sampleRate", SerializableDocument::from_i64(1000));
        custom_data_kinds.set("accuracy", SerializableDocument::from_f64(0.001));
        custom_data_kinds.set("validated", SerializableDocument::from_bool(true));

        let data_config = TsdConfigRef::new(
            "DataChannelList.xml",
            "2026-08-27T00:00:00Z".parse::<DateTimeOffset>().unwrap(),
        );
        let ch = TsdChannelId::from_string("Sensor1").expect("valid channel ID");
        let ts = "2026-08-27T10:00:00Z".parse::<DateTimeOffset>().unwrap();
        let ds = TabularDataSet::new(ts, &["100.0"], None);
        let tabular_data = TabularData::new(&[&ch], &[&ds]);

        let ts_data = TimeSeriesData::new()
            .with_data_configuration(&data_config)
            .with_tabular_data(&[&tabular_data])
            .with_custom_data_kinds(custom_data_kinds);

        let package = TsdPackage::new(Some(&header), &[&ts_data]);
        let ts_package = TimeSeriesDataPackage::new(&package);

        println!("Created package with custom extension data");

        let json_str = time_series_data_json::to_json(&ts_package, true);
        println!("\nSerialized JSON with custom data:");
        println!("{}", json_str);
        println!();
    }

    {
        println!("11. DataChannelId: dispatching on LocalId vs ShortId");
        println!("--------------------------------------------------------");

        let vis = Vis::instance();
        let gmod = vis.gmod(vis.latest()).expect("valid gmod");
        let locations = vis.locations(vis.latest()).expect("valid locations");
        let codebooks = vis.codebooks(vis.latest()).expect("valid codebooks");

        let primary_item = GmodPath::from_short_path("411.1", gmod, locations).expect("valid path");
        let qty_tag = codebooks[CodebookName::Quantity]
            .create_tag("temperature")
            .expect("valid tag");

        let local_id = LocalIdBuilder::create(vis.latest())
            .with_primary_item(&primary_item)
            .with_metadata_tag(&qty_tag)
            .build()
            .expect("valid local id");

        let channel_ids = vec![
            TsdChannelId::from_string(&local_id.to_string()).expect("valid local id channel"),
            TsdChannelId::from_string("Sensor42").expect("valid short id channel"),
        ];

        for channel_id in &channel_ids {
            if channel_id.is_local_id() {
                println!("  LocalId channel : {}", channel_id.local_id().unwrap());
            } else {
                println!(
                    "  ShortId channel : '{}'",
                    channel_id.short_id().unwrap_or_default()
                );
            }
        }

        println!();
    }

    {
        println!("12. Advanced: DTO-level manipulation before serialization");
        println!("---------------------------------------------------------");

        let ship_id = ShipId::from_string("IMO8027781").expect("valid IMO");
        let header = TsdHeader::new(&ship_id);
        let package = TsdPackage::new(Some(&header), &[]);
        let domain_package = TimeSeriesDataPackage::new(&package);

        // Convert to the DTO (the serialization-facing representation) so the package can be
        // patched and serialized directly, without rebuilding a validated domain package
        let mut dto = time_series_data_dto::to_dto(&domain_package).expect("to_dto");
        {
            let mut pkg = dto.pkg();
            let mut dto_header = pkg.header().expect("header should be present");
            dto_header.set_author("export-pipeline");
            dto_header.set_date_modified(&DateTimeOffset::utc_now().to_string());

            let mut custom_headers = dto_header.ensure_custom_headers();
            custom_headers.set(
                "exportedBy",
                SerializableDocument::from_string("vista-sdk-sample"),
            );
        }

        let patched_json = time_series_data_dto::to_json(&dto, true);

        println!("DTO patched with author, dateModified and exportedBy custom header:");
        println!("{}", patched_json);
        println!();
    }
}
