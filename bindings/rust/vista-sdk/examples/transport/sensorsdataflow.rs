use std::collections::HashMap;

use vista_sdk::core::local_id_builder::OwnedLocalIdBuilder;
use vista_sdk::transport::datachannel::data_channel::{
    ConfigurationReference, DataChannel, DataChannelId as DclChannelId, DataChannelList,
    DataChannelListPackage, DataChannelType, Format, Header as DclHeader, Package as DclPackage,
    Property, Range, Restriction, Unit, VersionInformation,
};
use vista_sdk::transport::datachannel::data_channel_json;
use vista_sdk::transport::ship_id::ShipId;
use vista_sdk::transport::timeseries::data_channel_id::TsdChannelId;
use vista_sdk::transport::timeseries::time_series_data::{
    TabularData, TabularDataSet, TimeSeriesData, TimeSeriesDataPackage, TsdConfigRef, TsdHeader,
    TsdPackage, TsdTimeSpan,
};
use vista_sdk::transport::timeseries::time_series_data_json;
use vista_sdk::types::date_time_offset::DateTimeOffset;

struct SensorReading {
    system_id: String,
    value: f64,
    timestamp: DateTimeOffset,
    quality: String,
}

fn format_double(value: f64) -> String {
    format!("{}", value)
}

fn create_data_channel_list() -> DataChannelListPackage {
    let mut dcl = DataChannelList::new();

    let local_id1 = OwnedLocalIdBuilder::from_string(
        "/dnv-v2/vis-3-4a/411.1-1/C101.63/S206/meta/qty-temperature/cnt-cooling.air",
    )
    .expect("valid local id 1");

    let mut dc_id1 = DclChannelId::new(&local_id1);
    dc_id1.set_short_id("TEMP001");

    let mut restriction1 = Restriction::new();
    restriction1.set_fraction_digits(1);
    restriction1.set_max_inclusive(200.0);
    restriction1.set_min_inclusive(-50.0);

    let mut format1 = Format::new("Decimal");
    format1.set_restriction(&restriction1);

    let mut dct1 = DataChannelType::new("Inst");
    dct1.set_update_cycle(1.0);

    let range1 = Range::new(0.0, 150.0);
    let mut unit1 = Unit::new("°C");
    unit1.set_quantity_name("Temperature");

    let mut prop1 = Property::new(&dct1, &format1);
    prop1.set_range(&range1);
    prop1.set_unit(&unit1);
    prop1.set_quality_coding("OPC_QUALITY");
    prop1.set_name("Main Engine Air Cooler Temperature");

    dcl.add(&DataChannel::new(&dc_id1, &prop1));

    let local_id2 =
        OwnedLocalIdBuilder::from_string("/dnv-v2/vis-3-4a/511.15-1/E32/meta/qty-power")
            .expect("valid local id 2");

    let mut dc_id2 = DclChannelId::new(&local_id2);
    dc_id2.set_short_id("PWR001");

    let format2 = Format::new("Decimal");
    let mut dct2 = DataChannelType::new("Inst");
    dct2.set_update_cycle(1.0);
    let range2 = Range::new(0.0, 10000.0);
    let mut unit2 = Unit::new("kW");
    unit2.set_quantity_name("Power");

    let mut prop2 = Property::new(&dct2, &format2);
    prop2.set_range(&range2);
    prop2.set_unit(&unit2);
    prop2.set_name("Generator Power Output");

    dcl.add(&DataChannel::new(&dc_id2, &prop2));

    let local_id3 = OwnedLocalIdBuilder::from_string(
        "/dnv-v2/vis-3-4a/621.22i/S110/meta/cnt-diesel.oil/cmd-stop",
    )
    .expect("valid local id 3");

    let mut dc_id3 = DclChannelId::new(&local_id3);
    dc_id3.set_short_id("ALT001");

    let format3 = Format::new("Boolean");
    let dct3 = DataChannelType::new("Command");

    let mut prop3 = Property::new(&dct3, &format3);
    prop3.set_name("Diesel Oil Stop Command");

    dcl.add(&DataChannel::new(&dc_id3, &prop3));

    let ship_id = ShipId::from_string("IMO1234567").expect("valid ship id");
    let config_ref = ConfigurationReference::new(
        "DataChannelList-v1",
        DateTimeOffset::from_str("2026-08-28T00:00:00Z").unwrap(),
    );
    let mut version_info = VersionInformation::with_fields("dnv", "v2");
    version_info.set_reference_url("https://docs.vista.dnv.com");

    let mut header = DclHeader::new(&ship_id, &config_ref);
    header.set_version_information(&version_info);
    header.set_author("Vista SDK Sample");
    header.set_date_created(DateTimeOffset::from_str("2026-08-28T00:00:00Z").unwrap());
    let package = DclPackage::new(&header, &dcl);
    DataChannelListPackage::new(&package)
}

fn main() {
    println!("=== vista-sdk SensorDataFlow Sample ===\n");

    {
        println!("1. DataChannelList: Creating sample data channel list");
        println!("--------------------------------------------------------");

        let dcl_package = create_data_channel_list();

        let dcl_ref = dcl_package.data_channel_list().expect("data channel list");
        println!("Created DataChannelList with {} channels:", dcl_ref.len());

        for i in 0..dcl_ref.len() {
            let dc = dcl_ref.at(i).unwrap();
            let cycle = dc.property().data_channel_type().update_cycle();
            let cycle_str = cycle
                .map(|c| format!("{}s", format_double(c)))
                .unwrap_or_else(|| "None".to_string());
            let short_id = dc
                .channel_id()
                .short_id()
                .unwrap_or_else(|| "<no-short-id>".to_string());
            println!("  {} (updateCycle={})", short_id, cycle_str);
        }

        {
            println!();
            let json_str = data_channel_json::to_json(&dcl_package, true);
            println!("Serialized to JSON: {} bytes", json_str.len());
            println!("Package:\n{}", json_str);
        }
        println!();

        println!("2. Proprietary sensor data: Simulating sensor readings");
        println!("---------------------------------------------------------");

        let base_time = DateTimeOffset::from_str("2026-08-28T10:00:00Z").unwrap();
        let base_plus_5 = DateTimeOffset::from_str("2026-08-28T10:05:00Z").unwrap();

        let readings: Vec<SensorReading> = vec![
            SensorReading {
                system_id: "TEMP001".to_string(),
                value: 45.2,
                timestamp: base_time,
                quality: "Good".to_string(),
            },
            SensorReading {
                system_id: "TEMP001".to_string(),
                value: 46.1,
                timestamp: base_plus_5,
                quality: "Good".to_string(),
            },
            SensorReading {
                system_id: "PWR001".to_string(),
                value: 2500.0,
                timestamp: base_time,
                quality: "Good".to_string(),
            },
            SensorReading {
                system_id: "PWR001".to_string(),
                value: 2650.5,
                timestamp: base_plus_5,
                quality: "Good".to_string(),
            },
            SensorReading {
                system_id: "ALT001".to_string(),
                value: 1.0,
                timestamp: base_time,
                quality: "Good".to_string(),
            },
            SensorReading {
                system_id: "UNKNOWN".to_string(),
                value: 123.4,
                timestamp: base_time,
                quality: "Good".to_string(),
            },
        ];

        println!(
            "Created {} sensor readings from proprietary system",
            readings.len()
        );
        println!("Sample readings:");
        for r in &readings {
            println!(
                "  SystemID={}, Value={}, Timestamp={}",
                r.system_id, r.value, r.timestamp
            );
        }
        println!();

        println!("3. Validation: Filter readings against DataChannelList");
        println!("--------------------------------------------------------");

        let mut valid_readings: HashMap<String, Vec<&SensorReading>> = HashMap::new();

        for reading in &readings {
            let dcl_ref = dcl_package.data_channel_list().expect("data channel list");
            if dcl_ref.from_short_id(&reading.system_id).is_some() {
                valid_readings
                    .entry(reading.system_id.clone())
                    .or_default()
                    .push(reading);
                println!("  Accepted: {} = {}", reading.system_id, reading.value);
            } else {
                println!("  Rejected: {} (unknown channel)", reading.system_id);
            }
        }

        println!(
            "\nAccepted {} unique channels with valid data",
            valid_readings.len()
        );
        println!();

        println!("4. Grouping: Group channels by updateCycle (ISO19848 optimization)");
        println!("---------------------------------------------------------------------");

        let mut channels_by_cycle: HashMap<String, Vec<(String, String)>> = HashMap::new();

        for system_id in valid_readings.keys() {
            let dcl_ref = dcl_package.data_channel_list().expect("data channel list");
            if let Some(dc) = dcl_ref.from_short_id(system_id) {
                let update_cycle = dc.property().data_channel_type().update_cycle();
                let cycle_key = update_cycle
                    .map(|c| format_double(c))
                    .unwrap_or_else(|| "none".to_string());
                let local_id_str = dc.channel_id().local_id_string().unwrap_or_default();
                channels_by_cycle
                    .entry(cycle_key)
                    .or_default()
                    .push((local_id_str, system_id.clone()));
            }
        }

        println!(
            "Grouped channels into {} updateCycle groups:",
            channels_by_cycle.len()
        );
        for (cycle_key, channels) in &channels_by_cycle {
            let cycle_str = if cycle_key == "none" {
                "None".to_string()
            } else {
                format!("{}s", cycle_key)
            };
            println!("  updateCycle={}: {} channels", cycle_str, channels.len());
        }
        println!();

        println!("5. TimeSeriesData: Build TabularData for each group");
        println!("------------------------------------------------------");

        let mut tabular_data_list: Vec<TabularData> = Vec::new();

        for (cycle_key, channel_pairs) in &channels_by_cycle {
            let mut all_timestamps: Vec<DateTimeOffset> = Vec::new();
            for (_, system_id) in channel_pairs {
                if let Some(rs) = valid_readings.get(system_id) {
                    for r in rs {
                        if !all_timestamps
                            .iter()
                            .any(|t| t.ticks() == r.timestamp.ticks())
                        {
                            all_timestamps.push(r.timestamp);
                        }
                    }
                }
            }
            all_timestamps.sort_by_key(|t| t.ticks());

            let mut data_channel_ids: Vec<TsdChannelId> = Vec::new();
            for (local_id_str, _) in channel_pairs {
                if let Some(id) = TsdChannelId::from_string(local_id_str) {
                    data_channel_ids.push(id);
                }
            }

            let mut data_sets: Vec<TabularDataSet> = Vec::new();
            for &ts in &all_timestamps {
                let mut values: Vec<String> = Vec::new();
                let mut qualities: Vec<String> = Vec::new();

                for (_, system_id) in channel_pairs {
                    let dcl_ref = dcl_package.data_channel_list().expect("data channel list");
                    let format_type = dcl_ref
                        .from_short_id(system_id)
                        .map(|dc| dc.property().format().type_())
                        .unwrap_or_default();

                    if let Some(rs) = valid_readings.get(system_id) {
                        if let Some(r) = rs.iter().find(|r| r.timestamp.ticks() == ts.ticks()) {
                            if format_type == "Boolean" {
                                values.push(if r.value != 0.0 {
                                    "True".to_string()
                                } else {
                                    "False".to_string()
                                });
                            } else {
                                values.push(format_double(r.value));
                            }
                            qualities.push(r.quality.clone());
                        } else {
                            values.push(String::new());
                            qualities.push("Bad".to_string());
                        }
                    }
                }

                data_sets.push(TabularDataSet::new(
                    ts,
                    &values.iter().map(|s| s.as_str()).collect::<Vec<_>>(),
                    Some(&qualities.iter().map(|s| s.as_str()).collect::<Vec<_>>()),
                ));
            }

            let cycle_str = if cycle_key == "none" {
                "None".to_string()
            } else {
                format!("{}s", cycle_key)
            };
            println!(
                "  TabularData: {} channels, {} rows (updateCycle={})",
                channel_pairs.len(),
                data_sets.len(),
                cycle_str
            );

            tabular_data_list.push(TabularData::new(
                &data_channel_ids.iter().collect::<Vec<_>>(),
                &data_sets.iter().collect::<Vec<_>>(),
            ));
        }
        println!();

        println!("6. Package: Create TimeSeriesDataPackage with header");
        println!("-------------------------------------------------------");

        let mut all_timestamps_list: Vec<DateTimeOffset> = Vec::new();
        for rs in valid_readings.values() {
            for r in rs {
                all_timestamps_list.push(r.timestamp);
            }
        }

        if all_timestamps_list.is_empty() {
            println!("No valid readings to build a TimeSeriesData package from.");
            return;
        }

        let min_ts = *all_timestamps_list
            .iter()
            .min_by_key(|t| t.ticks())
            .unwrap();
        let max_ts = *all_timestamps_list
            .iter()
            .max_by_key(|t| t.ticks())
            .unwrap();

        let ship_id = ShipId::from_string("IMO1234567").expect("valid ship id");

        let pkg_ref = dcl_package.package().expect("package");
        let hdr_ref = pkg_ref.header().expect("header");
        let dcl_id = hdr_ref.data_channel_list_id();
        let configuration = TsdConfigRef::new(&dcl_id.id(), dcl_id.timestamp());

        let time_span = TsdTimeSpan::new(min_ts, max_ts).expect("valid time span");
        let mut ts_header = TsdHeader::new(&ship_id);
        ts_header.set_time_span(&time_span);
        ts_header.set_author("Vista SDK Sample");

        let mut ts_data = TimeSeriesData::new();
        ts_data.set_data_configuration(&configuration);
        ts_data.set_tabular_data(&tabular_data_list.iter().collect::<Vec<_>>());

        let package = TsdPackage::new(Some(&ts_header), &[&ts_data]);
        let tsd_package = TimeSeriesDataPackage::new(&package);

        println!("Created TimeSeriesDataPackage:");
        println!("  Ship ID  : IMO1234567");
        println!("  TimeSpan : {} to {}", min_ts, max_ts);
        println!("  TabularData blocks: {}", tabular_data_list.len());
        println!();

        println!("7. Serialization: Convert to ISO19848 JSON format");
        println!("----------------------------------------------------");

        let json_str = time_series_data_json::to_json(&tsd_package, true);
        println!("Serialized to JSON: {} bytes", json_str.len());
        println!("Package:\n{}", json_str);
        println!();

        println!("8. Validation: Cross-check TimeSeriesData against DataChannelList");
        println!("-----------------------------------------------------------------------");

        let validation = ts_data.validate(&dcl_package, &|_ts| Ok(()), &|_ts| Ok(()));

        println!("  Is valid: {}", validation.is_valid());
        if !validation.is_valid() {
            println!("  Errors:");
            for error in validation.errors() {
                println!("    - {}", error);
            }
        }
        println!();

        println!("9. Validation failure: Out-of-range sensor value rejected");
        println!("---------------------------------------------------------------");

        let dcl_ref = dcl_package.data_channel_list().expect("data channel list");
        let temp001_local_id = dcl_ref
            .from_short_id("TEMP001")
            .expect("TEMP001 channel")
            .channel_id()
            .local_id_string()
            .expect("local id");

        let faulty_channel_id =
            TsdChannelId::from_string(&temp001_local_id).expect("valid channel id");

        let faulty_ds = TabularDataSet::new(base_time, &["999.9"], Some(&["Good"]));
        let faulty_tabular = TabularData::new(&[&faulty_channel_id], &[&faulty_ds]);

        let mut faulty_ts_data = TimeSeriesData::new();
        faulty_ts_data.set_data_configuration(&configuration);
        faulty_ts_data.set_tabular_data(&[&faulty_tabular]);

        println!("  Reporting TEMP001 = 999.9 degC (Restriction.MaxInclusive is 200)");

        let faulty_validation = faulty_ts_data.validate(&dcl_package, &|_ts| Ok(()), &|_ts| Ok(()));

        println!("  Is valid: {}", faulty_validation.is_valid());
        if !faulty_validation.is_valid() {
            println!("  Errors:");
            for error in faulty_validation.errors() {
                println!("    - {}", error);
            }
        }
        println!();
    }
}
