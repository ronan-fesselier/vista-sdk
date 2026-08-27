use vista_sdk::core::codebook_name::CodebookName;
use vista_sdk::core::gmod_path::OwnedGmodPath;
use vista_sdk::core::local_id_builder::OwnedLocalIdBuilder;
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
    TsdConfigRef, TsdHeader, TsdPackage, TsdTimeSpan, ValidateCallback,
};
use vista_sdk::transport::timeseries::time_series_data_json;
use vista_sdk::types::date_time_offset::DateTimeOffset;

fn main() {
    println!("=== vista-sdk TimeSeriesData Sample ===\n");

    {
        println!("1. Header: Creating TimeSeriesData package headers");
        println!("-----------------------------------------------------");

        let ship_id = ShipId::from_string("IMO9074729").expect("valid ship ID");

        let start = DateTimeOffset::from_str("2026-08-27T10:00:00Z").unwrap();
        let end = DateTimeOffset::from_str("2026-08-27T14:00:00Z").unwrap();
        let time_span = TsdTimeSpan::new(start, end).expect("valid time span");

        let date_created = DateTimeOffset::from_str("2026-08-27T10:00:00Z").unwrap();
        let date_modified = DateTimeOffset::from_str("2026-08-27T14:30:00Z").unwrap();

        let sys_config = TsdConfigRef::new(
            "SystemConfiguration.xml",
            DateTimeOffset::from_str("2026-08-27T00:00:00Z").unwrap(),
        );

        let mut header = TsdHeader::new(&ship_id);
        header.set_time_span(&time_span);
        header.set_date_created(date_created);
        header.set_date_modified(date_modified);
        header.set_author("Vista SDK Sample");
        header.set_system_configuration(&[&sys_config]);

        println!("Created header:");
        println!(
            "  System Configs: {} entries",
            header.system_configuration_count()
        );
        println!("  Date Created  : {:?}", header.date_created());
        println!("  Author        : {:?}", header.author());
        println!();
    }

    {
        println!("2. TabularData: Creating tabular time series data");
        println!("----------------------------------------------------");

        let channel_id1 = TsdChannelId::from_string("Temperature").expect("valid channel ID");
        let channel_id2 = TsdChannelId::from_string("Pressure").expect("valid channel ID");

        let ts1 = DateTimeOffset::from_str("2026-08-27T10:00:00Z").unwrap();
        let ts2 = DateTimeOffset::from_str("2026-08-27T11:00:00Z").unwrap();
        let ts3 = DateTimeOffset::from_str("2026-08-27T12:00:00Z").unwrap();

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
        let ts1 = DateTimeOffset::from_str("2026-08-27T10:30:00Z").unwrap();
        let ts2 = DateTimeOffset::from_str("2026-08-27T11:45:00Z").unwrap();

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
            DateTimeOffset::from_str("2026-08-27T00:00:00Z").unwrap(),
        );

        let ch1 = TsdChannelId::from_string("Temp1").expect("valid channel ID");
        let ch2 = TsdChannelId::from_string("Press1").expect("valid channel ID");
        let ts = DateTimeOffset::from_str("2026-08-27T10:00:00Z").unwrap();
        let ds = TabularDataSet::new(ts, &["100.0", "200.0"], None);
        let tabular_data = TabularData::new(&[&ch1, &ch2], &[&ds]);

        let alarm_ch = TsdChannelId::from_string("Alarm1").expect("valid channel ID");
        let event_ts = DateTimeOffset::from_str("2026-08-27T10:30:00Z").unwrap();
        let eds = EventDataSet::new(event_ts, &alarm_ch, "HIGH", Some("0"));
        let event_data = EventData::new(Some(&[&eds]));

        let mut ts_data = TimeSeriesData::new();
        ts_data.set_data_configuration(&data_config);
        ts_data.set_tabular_data(&[&tabular_data]);
        ts_data.set_event_data(&event_data);

        println!("Created TimeSeriesData:");
        println!("  Data Configuration: {}", data_config.id());
        println!("  Tabular Data      : 1 table");
        println!(
            "  Event Data        : {} events",
            event_data.data_set_count()
        );
        println!();
    }

    {
        println!("5. Package: Creating complete TimeSeriesData package");
        println!("-------------------------------------------------------");

        let ship_id = ShipId::from_string("IMO9074729").expect("valid ship ID");
        let start = DateTimeOffset::from_str("2026-08-27T10:00:00Z").unwrap();
        let end = DateTimeOffset::from_str("2026-08-27T14:00:00Z").unwrap();
        let time_span = TsdTimeSpan::new(start, end).expect("valid time span");

        let mut header = TsdHeader::new(&ship_id);
        header.set_time_span(&time_span);
        header.set_date_created(DateTimeOffset::from_str("2026-08-27T10:00:00Z").unwrap());
        header.set_date_modified(DateTimeOffset::from_str("2026-08-27T14:30:00Z").unwrap());
        header.set_author("Vista SDK");

        let data_config = TsdConfigRef::new(
            "DataChannelList.xml",
            DateTimeOffset::from_str("2026-08-27T00:00:00Z").unwrap(),
        );
        let ch = TsdChannelId::from_string("Sensor1").expect("valid channel ID");
        let ts = DateTimeOffset::from_str("2026-08-27T10:00:00Z").unwrap();
        let ds = TabularDataSet::new(ts, &["100.0"], None);
        let tabular_data = TabularData::new(&[&ch], &[&ds]);

        let mut ts_data = TimeSeriesData::new();
        ts_data.set_data_configuration(&data_config);
        ts_data.set_tabular_data(&[&tabular_data]);

        let package = TsdPackage::new(Some(&header), &[&ts_data]);
        let tsd_package = TimeSeriesDataPackage::new(&package);

        println!("Created TimeSeriesDataPackage:");
        println!("  Header         : {}", package.has_header());
        println!(
            "  TimeSeriesData : {} entries",
            tsd_package.time_series_data_count()
        );
        println!();
    }

    {
        println!("6. JSON Serialization: Round-trip serialization");
        println!("--------------------------------------------------");

        let ship_id = ShipId::from_string("IMO1234567").expect("valid ship ID");
        let mut header = TsdHeader::new(&ship_id);
        header.set_date_created(DateTimeOffset::from_str("2026-08-27T10:00:00Z").unwrap());

        let data_config = TsdConfigRef::new(
            "DataChannelList.xml",
            DateTimeOffset::from_str("2026-08-27T00:00:00Z").unwrap(),
        );
        let ch = TsdChannelId::from_string("Ch1").expect("valid channel ID");
        let ts = DateTimeOffset::from_str("2026-08-27T10:00:00Z").unwrap();
        let ds = TabularDataSet::new(ts, &["42.0"], None);
        let tabular_data = TabularData::new(&[&ch], &[&ds]);

        let mut ts_data = TimeSeriesData::new();
        ts_data.set_data_configuration(&data_config);
        ts_data.set_tabular_data(&[&tabular_data]);

        let package = TsdPackage::new(Some(&header), &[&ts_data]);
        let original_package = TimeSeriesDataPackage::new(&package);

        let json_str = time_series_data_json::to_json(&original_package, false);
        println!("Serialized to JSON ({} bytes)", json_str.len());

        let deserialized =
            time_series_data_json::from_json(&json_str).expect("roundtrip must parse");
        println!("Deserialized successfully:");
        println!(
            "  TimeSeriesData: {} entries",
            deserialized.time_series_data_count()
        );
        println!();
    }

    {
        println!("7. JSON Serialization: TabularData + EventData combined");
        println!("--------------------------------------------------");

        let ship_id = ShipId::from_string("IMO1234567").expect("valid ship ID");
        let mut header = TsdHeader::new(&ship_id);
        header.set_date_created(DateTimeOffset::from_str("2026-08-27T10:00:00Z").unwrap());

        let data_config = TsdConfigRef::new(
            "DataChannelList.xml",
            DateTimeOffset::from_str("2026-08-27T00:00:00Z").unwrap(),
        );
        let ch = TsdChannelId::from_string("Ch1").expect("valid channel ID");
        let ts = DateTimeOffset::from_str("2026-08-27T10:00:00Z").unwrap();
        let ds = TabularDataSet::new(ts, &["42.0"], None);
        let tabular_data = TabularData::new(&[&ch], &[&ds]);

        let alarm_ch = TsdChannelId::from_string("Alarm1").expect("valid channel ID");
        let event_ts = DateTimeOffset::from_str("2026-08-27T10:00:42Z").unwrap();
        let eds = EventDataSet::new(event_ts, &alarm_ch, "alarm.active", Some("0"));
        let event_data = EventData::new(Some(&[&eds]));

        let mut ts_data = TimeSeriesData::new();
        ts_data.set_data_configuration(&data_config);
        ts_data.set_tabular_data(&[&tabular_data]);
        ts_data.set_event_data(&event_data);

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
            DateTimeOffset::from_str("2026-08-27T00:00:00Z").unwrap(),
        );

        let primary_item = OwnedGmodPath::from_short_path("411.1/C101.31-2", &gmod, &locations)
            .expect("valid path");
        let qty_tag = codebooks[CodebookName::Quantity]
            .create_tag("temperature")
            .expect("valid tag");

        let local_id = OwnedLocalIdBuilder::create(vis.latest())
            .with_primary_item(&primary_item)
            .with_metadata_tag(&qty_tag)
            .build()
            .expect("valid local id");

        let mut dcl_channel_id = DclChannelId::new(&local_id);
        dcl_channel_id.set_short_id("TempSensor");

        let mut restriction = Restriction::new();
        restriction.set_fraction_digits(1);
        let mut format = Format::new("Decimal");
        format.set_restriction(&restriction);
        let dc_type = DataChannelType::new("Inst");
        let range = Range::new(0.0, 200.0);
        let unit = Unit::new("°C");
        let mut property = Property::new(&dc_type, &format);
        property.set_range(&range);
        property.set_unit(&unit);

        let data_channel = DataChannel::new(&dcl_channel_id, &property);
        let mut dcl = DataChannelList::new();
        dcl.add(&data_channel);

        let dcl_header = DclHeader::new(&ship_id, &config_ref);
        let dcl_pkg = DclPackage::new(&dcl_header, &dcl);
        let dcl_package = DataChannelListPackage::new(&dcl_pkg);

        let ts_config = TsdConfigRef::new(
            "DataChannelList.xml",
            DateTimeOffset::from_str("2026-08-27T00:00:00Z").unwrap(),
        );
        let ts_ch = TsdChannelId::from_string("TempSensor").expect("valid channel ID");
        let ts_event_ts = DateTimeOffset::from_str("2026-08-27T10:15:00Z").unwrap();
        let ts = DateTimeOffset::from_str("2026-08-27T10:00:00Z").unwrap();
        let ds = TabularDataSet::new(ts, &["100.5"], None);
        let tabular_data = TabularData::new(&[&ts_ch], &[&ds]);
        let eds = EventDataSet::new(ts_event_ts, &ts_ch, "105.2", Some("0"));
        let event_data = EventData::new(Some(&[&eds]));

        let mut ts_data = TimeSeriesData::new();
        ts_data.set_data_configuration(&ts_config);
        ts_data.set_tabular_data(&[&tabular_data]);
        ts_data.set_event_data(&event_data);

        let on_tabular: &ValidateCallback = &|ts| {
            println!("  Validating tabular data at {}", ts);
            Ok(())
        };
        let on_event: &ValidateCallback = &|ts| {
            println!("  Validating event data at {}", ts);
            Ok(())
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
            DateTimeOffset::from_str("2026-08-27T00:00:00Z").unwrap(),
        );

        let primary_item =
            OwnedGmodPath::from_short_path("411.1/C101", &gmod, &locations).expect("valid path");
        let qty_tag = codebooks[CodebookName::Quantity]
            .create_tag("temperature")
            .expect("valid tag");

        let local_id = OwnedLocalIdBuilder::create(vis.latest())
            .with_primary_item(&primary_item)
            .with_metadata_tag(&qty_tag)
            .build()
            .expect("valid local id");

        let mut dcl_channel_id = DclChannelId::new(&local_id);
        dcl_channel_id.set_short_id("ExhaustTemp");

        let dc_type = DataChannelType::new("Inst");
        let format = Format::new("Decimal");
        let range = Range::new(0.0, 450.0);
        let unit = Unit::new("deg C");
        let mut property = Property::new(&dc_type, &format);
        property.set_range(&range);
        property.set_unit(&unit);
        property.set_alert_priority("high-temperature");

        let data_channel = DataChannel::new(&dcl_channel_id, &property);
        let mut dcl = DataChannelList::new();
        dcl.add(&data_channel);

        let dcl_header = DclHeader::new(&ship_id, &config_ref);
        let dcl_pkg = DclPackage::new(&dcl_header, &dcl);
        let dcl_package = DataChannelListPackage::new(&dcl_pkg);

        let ts_config = TsdConfigRef::new(
            "DataChannelList.xml",
            DateTimeOffset::from_str("2026-08-27T00:00:00Z").unwrap(),
        );
        let ts_ch = TsdChannelId::from_string("ExhaustTemp").expect("valid channel ID");
        let ts = DateTimeOffset::from_str("2026-08-27T10:00:00Z").unwrap();
        let ds = TabularDataSet::new(ts, &["420.0"], None);
        let tabular_data = TabularData::new(&[&ts_ch], &[&ds]);

        let event_ts = DateTimeOffset::from_str("2026-08-27T11:00:00Z").unwrap();
        let eds = EventDataSet::new(event_ts, &ts_ch, "380.0", Some("0"));
        let event_data = EventData::new(Some(&[&eds]));

        let mut ts_data = TimeSeriesData::new();
        ts_data.set_data_configuration(&ts_config);
        ts_data.set_tabular_data(&[&tabular_data]);
        ts_data.set_event_data(&event_data);

        let on_tabular: &ValidateCallback = &|ts| {
            println!("  Exhaust temp tabular data at {}", ts);
            Err("Value 420.0 triggered alarm 'high-temperature': approaching upper limit of 450 deg C".to_string())
        };
        let on_event: &ValidateCallback = &|ts| {
            println!("  Exhaust temp event data at {}", ts);
            Ok(())
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
        custom_headers.set("version", SerializableDocument::from_str("1.2.3"));
        custom_headers.set("recordCount", SerializableDocument::from_i64(42));
        custom_headers.set("isCompressed", SerializableDocument::from_bool(true));
        custom_headers.set("temperature", SerializableDocument::from_f64(98.6));
        custom_headers.set(
            "createdAt",
            SerializableDocument::from_str("2026-08-27T10:00:00Z"),
        );

        let ship_id = ShipId::from_string("IMO1234567").expect("valid ship ID");
        let mut header = TsdHeader::new(&ship_id);
        header.set_date_created(DateTimeOffset::from_str("2026-08-27T10:00:00Z").unwrap());
        header.set_author("CustomData Demo");
        header.set_custom_headers(custom_headers);

        let mut custom_data_kinds = SerializableDocument::object();
        custom_data_kinds.set(
            "sensorModel",
            SerializableDocument::from_str("TempSensor-XYZ-2000"),
        );
        custom_data_kinds.set(
            "calibrationDate",
            SerializableDocument::from_str("2026-08-27T00:00:00Z"),
        );
        custom_data_kinds.set("sampleRate", SerializableDocument::from_i64(1000));
        custom_data_kinds.set("accuracy", SerializableDocument::from_f64(0.001));
        custom_data_kinds.set("validated", SerializableDocument::from_bool(true));

        let data_config = TsdConfigRef::new(
            "DataChannelList.xml",
            DateTimeOffset::from_str("2026-08-27T00:00:00Z").unwrap(),
        );
        let ch = TsdChannelId::from_string("Sensor1").expect("valid channel ID");
        let ts = DateTimeOffset::from_str("2026-08-27T10:00:00Z").unwrap();
        let ds = TabularDataSet::new(ts, &["100.0"], None);
        let tabular_data = TabularData::new(&[&ch], &[&ds]);

        let mut ts_data = TimeSeriesData::new();
        ts_data.set_data_configuration(&data_config);
        ts_data.set_tabular_data(&[&tabular_data]);
        ts_data.set_custom_data_kinds(custom_data_kinds);

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

        let primary_item =
            OwnedGmodPath::from_short_path("411.1", &gmod, &locations).expect("valid path");
        let qty_tag = codebooks[CodebookName::Quantity]
            .create_tag("temperature")
            .expect("valid tag");

        let local_id = OwnedLocalIdBuilder::create(vis.latest())
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

        // Section 12 (DTO-level manipulation) is not available via the C API --
        // toDto/toDomain are internal C++ implementation details with no C binding.

        println!();
    }
}
