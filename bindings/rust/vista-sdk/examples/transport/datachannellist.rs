use vista_sdk::core::codebook_name::CodebookName;
use vista_sdk::core::gmod_path::OwnedGmodPath;
use vista_sdk::core::imo_number::ImoNumber;
use vista_sdk::core::local_id_builder::OwnedLocalIdBuilder;
use vista_sdk::core::vis::Vis;
use vista_sdk::transport::datachannel::data_channel::{
    ConfigurationReference, DataChannel, DataChannelId, DataChannelList, DataChannelListPackage,
    DataChannelType, Format, Header, NameObject, Package, Property, Range, Unit,
    VersionInformation,
};
use vista_sdk::transport::datachannel::data_channel_json;
use vista_sdk::transport::serializable_document::SerializableDocument;
use vista_sdk::transport::ship_id::ShipId;
use vista_sdk::types::date_time_offset::DateTimeOffset;

fn main() {
    println!("=== vista-sdk DataChannelList Sample ===\n");

    {
        println!("1. Header: Creating DataChannelList headers");
        println!("----------------------------------------------");

        let imo = ImoNumber::from_str("9074729").expect("valid IMO");
        let ship_id = ShipId::from_imo_number(imo);

        let ts = DateTimeOffset::utc_now();
        let mut config_ref = ConfigurationReference::new("vessel-config-2026-v1", ts);
        config_ref.set_version("v1");

        let mut version_info = VersionInformation::new();
        version_info.set_naming_rule("dnv-v2");
        version_info.set_naming_scheme_version("3.0a");

        let mut header = Header::new(&ship_id, &config_ref);
        header.set_version_information(&version_info);
        header.set_author("Vista SDK Sample");
        header.set_date_created(DateTimeOffset::utc_now());

        let mut custom_headers = SerializableDocument::object();
        custom_headers.set(
            "vesselType",
            SerializableDocument::from_str("Container Ship"),
        );
        custom_headers.set("operator", SerializableDocument::from_str("DNV"));
        custom_headers.set(
            "lastModified",
            SerializableDocument::from_str("2026-08-25T14:30:00Z"),
        );
        header.set_custom_headers(custom_headers);

        println!("Created header:");
        println!("  Ship ID         : {}", ship_id);
        println!("  Config Reference: vessel-config-2026-v1");
        println!("  Version         : dnv-v2");
        println!("  Author          : Vista SDK Sample");
        println!(
            "  Date Created    : {}",
            header.date_created().expect("date set")
        );
        println!("  Custom Headers  : Present");
        println!();
    }

    {
        println!("2. DataChannelId: Creating channel identifiers");
        println!("-------------------------------------------------");

        let vis = Vis::instance();
        let gmod = vis.gmod(vis.latest()).expect("gmod");
        let locations = vis.locations(vis.latest()).expect("locations");
        let codebooks = vis.codebooks(vis.latest()).expect("codebooks");

        let primary_item =
            OwnedGmodPath::from_short_path("411.1/C101.63-1", gmod, locations).expect("valid path");
        let qty_tag = codebooks[CodebookName::Quantity]
            .create_tag("angle")
            .expect("valid tag");

        let local_id = OwnedLocalIdBuilder::create(vis.latest())
            .with_primary_item(&primary_item)
            .with_metadata_tag(&qty_tag)
            .build()
            .expect("valid local id");

        let mut name_object = NameObject::new();
        name_object.set_naming_rule("dnv-v2");

        let mut channel_id = DataChannelId::new(&*local_id);
        channel_id.set_short_id("GPSLatitude");
        channel_id.set_name_object(&name_object);

        println!("Created DataChannelId:");
        println!("  LocalId    : {}", local_id);
        println!("  Short ID   : GPSLatitude");
        println!("  Naming Rule: dnv-v2");
        println!();
    }

    {
        println!("3. Property: Creating properties with custom fields");
        println!("------------------------------------------------------");

        let dct = DataChannelType::new("Inst");
        let format = Format::new("Decimal");
        let unit = Unit::new("deg");
        let range = Range::new(-90.0, 90.0);

        let mut property = Property::new(&dct, &format);
        property.set_range(&range);
        property.set_unit(&unit);

        let mut custom_props = SerializableDocument::object();
        custom_props.set("coordinateSystem", SerializableDocument::from_str("WGS84"));
        custom_props.set("range", SerializableDocument::from_str("-90 to 90"));
        property.set_custom_properties(custom_props);

        println!("Created Property:");
        println!("  Type             : Inst");
        println!("  Format           : Decimal");
        println!("  Unit             : deg");
        println!("  Custom Properties: Present");
        println!();
    }

    {
        println!("4. DataChannel: Creating complete data channels");
        println!("--------------------------------------------------");

        let vis = Vis::instance();
        let gmod = vis.gmod(vis.latest()).expect("gmod");
        let locations = vis.locations(vis.latest()).expect("locations");
        let codebooks = vis.codebooks(vis.latest()).expect("codebooks");

        let primary_item =
            OwnedGmodPath::from_short_path("411.1/C101.63-1", gmod, locations).expect("valid path");
        let qty_tag = codebooks[CodebookName::Quantity]
            .create_tag("angle")
            .expect("valid tag");
        let local_id = OwnedLocalIdBuilder::create(vis.latest())
            .with_primary_item(&primary_item)
            .with_metadata_tag(&qty_tag)
            .build()
            .expect("valid local id");

        let mut name_object = NameObject::new();
        name_object.set_naming_rule("dnv-v2");

        let mut channel_id = DataChannelId::new(&*local_id);
        channel_id.set_short_id("GPSLatitude");
        channel_id.set_name_object(&name_object);

        let mut custom_props = SerializableDocument::object();
        custom_props.set("coordinateSystem", SerializableDocument::from_str("WGS84"));

        let range = Range::new(-90.0, 90.0);
        let unit = Unit::new("deg");
        let dct = DataChannelType::new("Inst");
        let format = Format::new("Decimal");
        let mut property = Property::new(&dct, &format);
        property.set_range(&range);
        property.set_unit(&unit);
        property.set_custom_properties(custom_props);

        drop(DataChannel::new(&channel_id, &property));

        println!("Created DataChannel:");
        println!("  Short ID: {}", channel_id.short_id().unwrap_or_default());
        println!("  LocalId : {}", local_id);
        println!("  Type    : Inst");
        println!("  Format  : Decimal");
        println!("  Unit    : deg");
        println!();
    }

    {
        println!("5. DataChannelList: Creating complete channel lists");
        println!("------------------------------------------------------");

        let vis = Vis::instance();
        let gmod = vis.gmod(vis.latest()).expect("gmod");
        let locations = vis.locations(vis.latest()).expect("locations");
        let codebooks = vis.codebooks(vis.latest()).expect("codebooks");

        let imo = ImoNumber::from_str("9074729").expect("valid IMO");
        let ship_id = ShipId::from_imo_number(imo);
        let ts = DateTimeOffset::utc_now();
        let mut config_ref = ConfigurationReference::new("vessel-config-2026-v1", ts);
        config_ref.set_version("v1");

        let mut custom_headers = SerializableDocument::object();
        custom_headers.set(
            "vesselType",
            SerializableDocument::from_str("Container Ship"),
        );

        let mut header = Header::new(&ship_id, &config_ref);
        let mut version_info_5 = VersionInformation::new();
        version_info_5.set_naming_rule("dnv-v2");
        version_info_5.set_naming_scheme_version("3.0a");
        header.set_version_information(&version_info_5);
        header.set_author("Vista SDK Sample");
        header.set_date_created(DateTimeOffset::utc_now());
        header.set_custom_headers(custom_headers);

        let mut list = DataChannelList::new();

        // Channel 1: GPS Latitude
        {
            let primary = OwnedGmodPath::from_short_path("411.1/C101.63-1", gmod, locations)
                .expect("valid path");
            let qty_tag = codebooks[CodebookName::Quantity]
                .create_tag("angle")
                .expect("valid tag");
            let local_id = OwnedLocalIdBuilder::create(vis.latest())
                .with_primary_item(&primary)
                .with_metadata_tag(&qty_tag)
                .build()
                .expect("valid local id");
            let mut name_object = NameObject::new();
            name_object.set_naming_rule("dnv-v2");
            let mut channel_id = DataChannelId::new(&*local_id);
            channel_id.set_short_id("GPSLatitude");
            channel_id.set_name_object(&name_object);
            let mut custom_props = SerializableDocument::object();
            custom_props.set("coordinateSystem", SerializableDocument::from_str("WGS84"));
            let range = Range::new(-90.0, 90.0);
            let unit = Unit::new("deg");
            let dct = DataChannelType::new("Inst");
            let format = Format::new("Decimal");
            let mut property = Property::new(&dct, &format);
            property.set_range(&range);
            property.set_unit(&unit);
            property.set_custom_properties(custom_props);
            list.add(&DataChannel::new(&channel_id, &property));
        }

        // Channel 2: GPS Longitude
        {
            let primary = OwnedGmodPath::from_short_path("411.1/C101.63-2", gmod, locations)
                .expect("valid path");
            let qty_tag = codebooks[CodebookName::Quantity]
                .create_tag("angle")
                .expect("valid tag");
            let local_id = OwnedLocalIdBuilder::create(vis.latest())
                .with_primary_item(&primary)
                .with_metadata_tag(&qty_tag)
                .build()
                .expect("valid local id");
            let mut name_object = NameObject::new();
            name_object.set_naming_rule("dnv-v2");
            let mut channel_id = DataChannelId::new(&*local_id);
            channel_id.set_short_id("GPSLongitude");
            channel_id.set_name_object(&name_object);
            let mut custom_props = SerializableDocument::object();
            custom_props.set("coordinateSystem", SerializableDocument::from_str("WGS84"));
            let range = Range::new(-180.0, 180.0);
            let unit = Unit::new("deg");
            let dct = DataChannelType::new("Inst");
            let format = Format::new("Decimal");
            let mut property = Property::new(&dct, &format);
            property.set_range(&range);
            property.set_unit(&unit);
            property.set_custom_properties(custom_props);
            list.add(&DataChannel::new(&channel_id, &property));
        }

        // Channel 3: Engine Temperature
        {
            let primary = OwnedGmodPath::from_short_path("411.1/C101.31-2", gmod, locations)
                .expect("valid path");
            let qty_tag = codebooks[CodebookName::Quantity]
                .create_tag("temperature")
                .expect("valid tag");
            let cnt_tag = codebooks[CodebookName::Content]
                .create_tag("exhaust.gas")
                .expect("valid tag");
            let local_id = OwnedLocalIdBuilder::create(vis.latest())
                .with_primary_item(&primary)
                .with_metadata_tag(&qty_tag)
                .with_metadata_tag(&cnt_tag)
                .build()
                .expect("valid local id");
            let mut name_object = NameObject::new();
            name_object.set_naming_rule("dnv-v2");
            let mut channel_id = DataChannelId::new(&*local_id);
            channel_id.set_short_id("EngineTemp");
            channel_id.set_name_object(&name_object);
            let mut custom_props = SerializableDocument::object();
            custom_props.set(
                "sensor",
                SerializableDocument::from_str("K-type thermocouple"),
            );
            let range = Range::new(-50.0, 1200.0);
            let unit = Unit::new("°C");
            let dct = DataChannelType::new("Average");
            let format = Format::new("Decimal");
            let mut property = Property::new(&dct, &format);
            property.set_range(&range);
            property.set_unit(&unit);
            property.set_custom_properties(custom_props);
            list.add(&DataChannel::new(&channel_id, &property));
        }

        // Channel 4: Fuel Level
        {
            let primary =
                OwnedGmodPath::from_short_path("621.21/S90", gmod, locations).expect("valid path");
            let qty_tag = codebooks[CodebookName::Quantity]
                .create_tag("volume")
                .expect("valid tag");
            let cnt_tag = codebooks[CodebookName::Content]
                .create_tag("fuel.oil")
                .expect("valid tag");
            let local_id = OwnedLocalIdBuilder::create(vis.latest())
                .with_primary_item(&primary)
                .with_metadata_tag(&qty_tag)
                .with_metadata_tag(&cnt_tag)
                .build()
                .expect("valid local id");
            let mut name_object = NameObject::new();
            name_object.set_naming_rule("dnv-v2");
            let mut channel_id = DataChannelId::new(&*local_id);
            channel_id.set_short_id("FuelLevel");
            channel_id.set_name_object(&name_object);
            let mut custom_props = SerializableDocument::object();
            custom_props.set("tankCapacity", SerializableDocument::from_str("5000.0"));
            custom_props.set("alarmLevel", SerializableDocument::from_str("500.0"));
            let range = Range::new(0.0, 5000.0);
            let unit = Unit::new("m3");
            let dct = DataChannelType::new("Inst");
            let format = Format::new("Decimal");
            let mut property = Property::new(&dct, &format);
            property.set_range(&range);
            property.set_unit(&unit);
            property.set_custom_properties(custom_props);
            list.add(&DataChannel::new(&channel_id, &property));
        }

        let package = Package::new(&header, &list);

        println!("Created Package with DataChannelList:");
        println!("  Ship ID      : {}", ship_id);
        println!("  Config Ref   : vessel-config-2026-v1");
        println!("  Channel Count: {}", list.len());
        println!();
        let channel_types = ["Inst", "Inst", "Average", "Inst"];
        let short_ids = ["GPSLatitude", "GPSLongitude", "EngineTemp", "FuelLevel"];
        println!("Channels:");
        for i in 0..list.len() {
            println!(
                "  [{}] {} ({}, Decimal)",
                i + 1,
                short_ids[i],
                channel_types[i]
            );
        }
        println!();

        let _ = package;
    }

    {
        println!("6. Value Types: Working with different data types");
        println!("----------------------------------------------------");

        use vista_sdk::transport::iso19848::Value;
        use vista_sdk::types::decimal::Decimal;

        let string_value = Value::String("test string".to_string());
        let int_value = Value::Integer(42);
        let decimal_value =
            Value::Decimal(Decimal::from_str("3.14159").expect("valid decimal string"));
        let bool_value = Value::Boolean(true);
        let date_time_value = Value::DateTime(DateTimeOffset::utc_now());

        println!("Value type checks:");
        println!(
            "  String   value is string  : {}",
            matches!(string_value, Value::String(_))
        );
        println!(
            "  Integer  value is integer : {}",
            matches!(int_value, Value::Integer(_))
        );
        println!(
            "  Decimal  value is decimal : {}",
            matches!(decimal_value, Value::Decimal(_))
        );
        println!(
            "  Boolean  value is boolean : {}",
            matches!(bool_value, Value::Boolean(_))
        );
        println!(
            "  DateTime value is dateTime: {}",
            matches!(date_time_value, Value::DateTime(_))
        );

        println!("\nExtracting values:");
        if let Value::String(s) = &string_value {
            println!("  String   : {}", s);
        }
        if let Value::Integer(i) = &int_value {
            println!("  Integer  : {}", i);
        }
        if let Value::Decimal(d) = &decimal_value {
            println!("  Decimal  : {}", d);
        }
        if let Value::Boolean(b) = &bool_value {
            println!("  Boolean  : {}", b);
        }
        if let Value::DateTime(dt) = &date_time_value {
            println!("  DateTime : {}", dt);
        }
        println!();
    }

    {
        println!("7. Validation: ISO 19848 field validation");
        println!("--------------------------------------------");

        let valid_type = DataChannelType::new("Inst");
        println!("[OK] Created valid DataChannelType: {}", valid_type.type_());

        let mut invalid_type = DataChannelType::new("Inst");
        if !invalid_type.set_type("InvalidType") {
            println!("[OK] Correctly rejected invalid DataChannelType: Invalid data channel type: InvalidType");
        } else {
            println!("[ERROR] Created invalid DataChannelType (should have failed)");
        }

        let valid_format = Format::new("Decimal");
        println!("[OK] Created valid Format: {}", valid_format.type_());

        let mut invalid_format = Format::new("Decimal");
        if !invalid_format.set_type("InvalidFormat") {
            println!("[OK] Correctly rejected invalid Format: Invalid format type: InvalidFormat");
        } else {
            println!("[ERROR] Created invalid Format (should have failed)");
        }

        println!();
    }

    {
        println!("8. JSON Serialization: Converting to/from JSON");
        println!("------------------------------------------------");

        let vis = Vis::instance();
        let gmod = vis.gmod(vis.latest()).expect("gmod");
        let locations = vis.locations(vis.latest()).expect("locations");
        let codebooks = vis.codebooks(vis.latest()).expect("codebooks");

        let ship_id = ShipId::from_string("IMO1234567")
            .unwrap_or_else(|| ShipId::from_other_id("IMO1234567").expect("non-empty"));
        let ts = DateTimeOffset::utc_now();
        let config_ref = ConfigurationReference::new("demo-config-v1", ts);
        let header = Header::new(&ship_id, &config_ref);

        let mut list = DataChannelList::new();

        // GPS Latitude
        {
            let primary = OwnedGmodPath::from_short_path("710.1/F211.11", gmod, locations)
                .expect("valid path");
            let qty_tag = codebooks[CodebookName::Quantity]
                .create_tag("latitude")
                .expect("valid tag");
            let local_id = OwnedLocalIdBuilder::create(vis.latest())
                .with_primary_item(&primary)
                .with_metadata_tag(&qty_tag)
                .build()
                .expect("valid local id");
            let mut name_object = NameObject::new();
            name_object.set_naming_rule("dnv-v2");
            let mut channel_id = DataChannelId::new(&*local_id);
            channel_id.set_short_id("GPSLatitude");
            channel_id.set_name_object(&name_object);
            let mut custom_props = SerializableDocument::object();
            custom_props.set("coordinateSystem", SerializableDocument::from_str("WGS84"));
            custom_props.set("geodeticDatum", SerializableDocument::from_str("WGS84"));
            let range = Range::new(-90.0, 90.0);
            let unit = Unit::new("deg");
            let dct = DataChannelType::new("Inst");
            let format = Format::new("Decimal");
            let mut property = Property::new(&dct, &format);
            property.set_range(&range);
            property.set_unit(&unit);
            property.set_name("GPS Latitude");
            property.set_remarks("Primary GPS latitude position");
            property.set_custom_properties(custom_props);
            list.add(&DataChannel::new(&channel_id, &property));
        }

        // GPS Longitude
        {
            let primary = OwnedGmodPath::from_short_path("710.1/F211.12", gmod, locations)
                .expect("valid path");
            let qty_tag = codebooks[CodebookName::Quantity]
                .create_tag("longitude")
                .expect("valid tag");
            let local_id = OwnedLocalIdBuilder::create(vis.latest())
                .with_primary_item(&primary)
                .with_metadata_tag(&qty_tag)
                .build()
                .expect("valid local id");
            let mut name_object = NameObject::new();
            name_object.set_naming_rule("dnv-v2");
            let mut channel_id = DataChannelId::new(&*local_id);
            channel_id.set_short_id("GPSLongitude");
            channel_id.set_name_object(&name_object);
            let mut custom_props = SerializableDocument::object();
            custom_props.set("coordinateSystem", SerializableDocument::from_str("WGS84"));
            custom_props.set("geodeticDatum", SerializableDocument::from_str("WGS84"));
            let range = Range::new(-180.0, 180.0);
            let unit = Unit::new("deg");
            let dct = DataChannelType::new("Inst");
            let format = Format::new("Decimal");
            let mut property = Property::new(&dct, &format);
            property.set_range(&range);
            property.set_unit(&unit);
            property.set_name("GPS Longitude");
            property.set_remarks("Primary GPS longitude position");
            property.set_custom_properties(custom_props);
            list.add(&DataChannel::new(&channel_id, &property));
        }

        let package = Package::new(&header, &list);
        let lp = DataChannelListPackage::new(&package);
        let json = data_channel_json::to_json(&lp, true);

        println!("Serialization result: Success");
        println!("\nJSON output (formatted):");
        println!("{}", json);
        println!();
    }

    {
        println!("9. Advanced: Main Engine Monitoring System");
        println!("--------------------------------------------");

        let vis = Vis::instance();
        let gmod = vis.gmod(vis.latest()).expect("gmod");
        let locations = vis.locations(vis.latest()).expect("locations");
        let codebooks = vis.codebooks(vis.latest()).expect("codebooks");

        let imo = ImoNumber::from_str("9074729").expect("valid IMO");
        let ship_id = ShipId::from_imo_number(imo);
        let ts = DateTimeOffset::utc_now();
        let mut config_ref = ConfigurationReference::new("main-engine-monitor-v3", ts);
        config_ref.set_version("3.0");

        let mut version_info = VersionInformation::new();
        version_info.set_naming_rule("dnv-v2");
        version_info.set_naming_scheme_version("3.10a");

        let mut custom_headers = SerializableDocument::object();
        custom_headers.set(
            "monitoringSystem",
            SerializableDocument::from_str("Vista Engine Monitor"),
        );
        custom_headers.set(
            "engineManufacturer",
            SerializableDocument::from_str("MAN Energy Solutions"),
        );
        custom_headers.set("engineModel", SerializableDocument::from_str("ME-C9.5-175"));
        custom_headers.set("samplingRate", SerializableDocument::from_str("10"));

        let mut header = Header::new(&ship_id, &config_ref);
        header.set_version_information(&version_info);
        header.set_author("Chief Engineer");
        header.set_date_created(DateTimeOffset::utc_now());
        header.set_custom_headers(custom_headers);

        let mut list = DataChannelList::new();

        // Channel 1: Engine Speed (RPM)
        {
            let primary = OwnedGmodPath::from_short_path("411.1/C101.41", gmod, locations)
                .expect("valid path");
            let qty_tag = codebooks[CodebookName::Quantity]
                .create_tag("rotational.speed")
                .expect("valid tag");
            let local_id = OwnedLocalIdBuilder::create(vis.latest())
                .with_primary_item(&primary)
                .with_metadata_tag(&qty_tag)
                .build()
                .expect("valid local id");

            let mut custom_name_objects = SerializableDocument::object();
            custom_name_objects.set(
                "sensorType",
                SerializableDocument::from_str("Magnetic pickup"),
            );
            custom_name_objects.set(
                "sensorManufacturer",
                SerializableDocument::from_str("Wärtsilä"),
            );
            custom_name_objects.set(
                "calibrationDate",
                SerializableDocument::from_str("2026-08-25T10:00:00Z"),
            );
            custom_name_objects.set("isRedundant", SerializableDocument::from_bool(true));

            let mut name_object = NameObject::new();
            name_object.set_naming_rule("dnv-v2");
            name_object.set_custom_name_objects(custom_name_objects);

            let mut channel_id = DataChannelId::new(&*local_id);
            channel_id.set_short_id("MainEngineRPM");
            channel_id.set_name_object(&name_object);

            let mut unit_custom = SerializableDocument::object();
            unit_custom.set("siEquivalent", SerializableDocument::from_str("hertz"));
            unit_custom.set("conversionFactor", SerializableDocument::from_str("60"));
            let mut unit = Unit::new("rpm");
            unit.set_quantity_name("rotational speed");
            unit.set_custom_elements(unit_custom);

            let mut custom_props = SerializableDocument::object();
            custom_props.set("nominalSpeed", SerializableDocument::from_str("150"));
            custom_props.set("maxSpeed", SerializableDocument::from_str("175"));
            custom_props.set("idleSpeed", SerializableDocument::from_str("60"));
            custom_props.set("criticalAlarm", SerializableDocument::from_bool(true));

            let range = Range::new(0.0, 200.0);
            let dct = DataChannelType::new("Inst");
            let format = Format::new("Decimal");
            let mut property = Property::new(&dct, &format);
            property.set_range(&range);
            property.set_unit(&unit);
            property.set_quality_coding("A");
            property.set_alert_priority("High");
            property.set_name("Main Engine Rotational Speed");
            property.set_remarks("Primary propulsion engine RPM");
            property.set_custom_properties(custom_props);

            list.add(&DataChannel::new(&channel_id, &property));
        }

        // Channel 2: Lube Oil Pressure
        {
            let primary = OwnedGmodPath::from_short_path("411.1/C101.663i", gmod, locations)
                .expect("valid path");
            let qty_tag = codebooks[CodebookName::Quantity]
                .create_tag("pressure")
                .expect("valid tag");
            let cnt_tag = codebooks[CodebookName::Content]
                .create_tag("lubricating.oil")
                .expect("valid tag");
            let pos_tag = codebooks[CodebookName::Position]
                .create_tag("inlet")
                .expect("valid tag");
            let local_id = OwnedLocalIdBuilder::create(vis.latest())
                .with_primary_item(&primary)
                .with_metadata_tag(&qty_tag)
                .with_metadata_tag(&cnt_tag)
                .with_metadata_tag(&pos_tag)
                .build()
                .expect("valid local id");

            let mut custom_name_objects = SerializableDocument::object();
            custom_name_objects.set(
                "sensorType",
                SerializableDocument::from_str("Pressure transducer"),
            );
            custom_name_objects.set("sensorManufacturer", SerializableDocument::from_str("WIKA"));
            custom_name_objects.set("sensorModel", SerializableDocument::from_str("A-10"));
            custom_name_objects.set("accuracy", SerializableDocument::from_str("0.5"));

            let mut name_object = NameObject::new();
            name_object.set_naming_rule("dnv-v2");
            name_object.set_custom_name_objects(custom_name_objects);

            let mut channel_id = DataChannelId::new(&*local_id);
            channel_id.set_short_id("LubeOilPressure");
            channel_id.set_name_object(&name_object);

            let mut unit_custom = SerializableDocument::object();
            unit_custom.set("pressureType", SerializableDocument::from_str("Gauge"));
            unit_custom.set("siEquivalent", SerializableDocument::from_str("pascal"));
            unit_custom.set("conversionFactor", SerializableDocument::from_str("100000"));
            let mut unit = Unit::new("bar");
            unit.set_quantity_name("pressure");
            unit.set_custom_elements(unit_custom);

            let mut custom_props = SerializableDocument::object();
            custom_props.set("normalPressure", SerializableDocument::from_str("4.2"));
            custom_props.set("minPressure", SerializableDocument::from_str("2.5"));
            custom_props.set("alarmThreshold", SerializableDocument::from_str("2"));
            custom_props.set("shutdownThreshold", SerializableDocument::from_str("1.5"));

            let range = Range::new(0.0, 10.0);
            let mut dct = DataChannelType::new("Average");
            dct.set_update_cycle(5.0);
            dct.set_calculation_period(1.0);
            let format = Format::new("Decimal");
            let mut property = Property::new(&dct, &format);
            property.set_range(&range);
            property.set_unit(&unit);
            property.set_quality_coding("A");
            property.set_alert_priority("High");
            property.set_name("Lubricating Oil Inlet Pressure");
            property.set_remarks("Main engine lube oil system inlet pressure");
            property.set_custom_properties(custom_props);

            list.add(&DataChannel::new(&channel_id, &property));
        }

        // Channel 3: Exhaust Gas Temperature
        {
            let primary = OwnedGmodPath::from_short_path("411.1/C101.31-2", gmod, locations)
                .expect("valid path");
            let qty_tag = codebooks[CodebookName::Quantity]
                .create_tag("temperature")
                .expect("valid tag");
            let cnt_tag = codebooks[CodebookName::Content]
                .create_tag("exhaust.gas")
                .expect("valid tag");
            let local_id = OwnedLocalIdBuilder::create(vis.latest())
                .with_primary_item(&primary)
                .with_metadata_tag(&qty_tag)
                .with_metadata_tag(&cnt_tag)
                .build()
                .expect("valid local id");

            let mut custom_name_objects = SerializableDocument::object();
            custom_name_objects.set(
                "sensorType",
                SerializableDocument::from_str("K-type thermocouple"),
            );
            custom_name_objects.set("cylinderNumber", SerializableDocument::from_i64(2));
            custom_name_objects.set("maxTemp", SerializableDocument::from_str("600"));
            custom_name_objects.set("isCritical", SerializableDocument::from_bool(true));

            let mut name_object = NameObject::new();
            name_object.set_naming_rule("dnv-v2");
            name_object.set_custom_name_objects(custom_name_objects);

            let mut channel_id = DataChannelId::new(&*local_id);
            channel_id.set_short_id("ExhaustTemp_Cyl2");
            channel_id.set_name_object(&name_object);

            let mut unit_custom = SerializableDocument::object();
            unit_custom.set("siEquivalent", SerializableDocument::from_str("kelvin"));
            unit_custom.set("conversionOffset", SerializableDocument::from_str("273.15"));
            let mut unit = Unit::new("degC");
            unit.set_quantity_name("temperature");
            unit.set_custom_elements(unit_custom);

            let mut custom_props = SerializableDocument::object();
            custom_props.set("normalTemp", SerializableDocument::from_str("380"));
            custom_props.set("maxTemp", SerializableDocument::from_str("450"));
            custom_props.set("alarmThreshold", SerializableDocument::from_str("420"));
            custom_props.set("deviationAlarm", SerializableDocument::from_str("30"));

            let range = Range::new(0.0, 600.0);
            let mut dct = DataChannelType::new("Average");
            dct.set_update_cycle(10.0);
            dct.set_calculation_period(10.0);
            let format = Format::new("Decimal");
            let mut property = Property::new(&dct, &format);
            property.set_range(&range);
            property.set_unit(&unit);
            property.set_quality_coding("A");
            property.set_alert_priority("High");
            property.set_name("Exhaust Gas Temperature Cylinder 2");
            property.set_remarks("Main engine cylinder 2 exhaust temperature");
            property.set_custom_properties(custom_props);

            list.add(&DataChannel::new(&channel_id, &property));
        }

        // Channel 4: Fuel Consumption
        {
            let primary =
                OwnedGmodPath::from_short_path("411.1/C101", gmod, locations).expect("valid path");
            let secondary = OwnedGmodPath::from_short_path("620.1/M201.32", gmod, locations)
                .expect("valid path");
            let qty_tag = codebooks[CodebookName::Quantity]
                .create_tag("volume.flow.rate")
                .expect("valid tag");
            let local_id = OwnedLocalIdBuilder::create(vis.latest())
                .with_primary_item(&primary)
                .with_secondary_item(&secondary)
                .with_metadata_tag(&qty_tag)
                .build()
                .expect("valid local id");

            let mut custom_name_objects = SerializableDocument::object();
            custom_name_objects.set(
                "meterType",
                SerializableDocument::from_str("Coriolis flow meter"),
            );
            custom_name_objects.set(
                "meterManufacturer",
                SerializableDocument::from_str("Endress+Hauser"),
            );
            custom_name_objects.set("meterModel", SerializableDocument::from_str("Promass 83F"));
            custom_name_objects.set("accuracy", SerializableDocument::from_str("0.1"));

            let mut name_object = NameObject::new();
            name_object.set_naming_rule("dnv-v2");
            name_object.set_custom_name_objects(custom_name_objects);

            let mut channel_id = DataChannelId::new(&*local_id);
            channel_id.set_short_id("FuelConsumption");
            channel_id.set_name_object(&name_object);

            let mut unit_custom = SerializableDocument::object();
            unit_custom.set("flowType", SerializableDocument::from_str("Volumetric"));
            unit_custom.set("fluidType", SerializableDocument::from_str("HFO380"));
            unit_custom.set("refTemperature", SerializableDocument::from_str("15"));
            let mut unit = Unit::new("l/h");
            unit.set_quantity_name("volume flow rate");
            unit.set_custom_elements(unit_custom);

            let mut custom_props = SerializableDocument::object();
            custom_props.set("fuelGrade", SerializableDocument::from_str("IFO380"));
            custom_props.set("fuelDensity", SerializableDocument::from_str("991"));
            custom_props.set("nominalConsumption", SerializableDocument::from_str("2800"));
            custom_props.set("co2Factor", SerializableDocument::from_str("3.114"));

            let range = Range::new(0.0, 5000.0);
            let mut dct = DataChannelType::new("Average");
            dct.set_update_cycle(60.0);
            dct.set_calculation_period(60.0);
            let format = Format::new("Decimal");
            let mut property = Property::new(&dct, &format);
            property.set_range(&range);
            property.set_unit(&unit);
            property.set_quality_coding("A");
            property.set_alert_priority("Normal");
            property.set_name("Main Engine Fuel Consumption");
            property.set_remarks("Hourly fuel consumption monitoring");
            property.set_custom_properties(custom_props);

            list.add(&DataChannel::new(&channel_id, &property));
        }

        let package = Package::new(&header, &list);
        let lp = DataChannelListPackage::new(&package);

        println!("Main Engine Monitoring System:");
        println!("  Engine       : MAN Energy Solutions ME-C9.5-175");
        println!("  Channels     : {}", list.len());
        println!("  Custom fields: Present");
        println!("\nMonitored parameters:");
        for i in 0..list.len() {
            let dc = list.at(i).expect("valid index");
            let short_id = dc.channel_id().short_id().unwrap_or_default();
            let name = dc.property().name().unwrap_or_default();
            println!("  [{}] {} - {}", i + 1, short_id, name);
        }

        let json = data_channel_json::to_json(&lp, true);
        println!("\nEngine Monitoring JSON:");
        println!("{}", json);
        println!();
    }
}
