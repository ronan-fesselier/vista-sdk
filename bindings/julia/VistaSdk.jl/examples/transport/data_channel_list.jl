using VistaSdk

println("=== vista-sdk DataChannelList Sample ===\n")

let
    println("1. Header: Creating DataChannelList headers")
    println("----------------------------------------------")

    imo = parse(ImoNumber, "9074729")
    ship_id = from_imo_number(imo)

    ts = now(DateTimeOffset)
    config_ref = DclConfigurationReference("vessel-config-2026-v1", ts)
    set_version!(config_ref, "v1")

    vi = DclVersionInformation()
    set_naming_rule!(vi, "dnv-v2")
    set_naming_scheme_version!(vi, "3.0a")

    custom_hdrs = sd_object()
    set!(custom_hdrs, "vesselType", SerializableDocument("Container Ship"))
    set!(custom_hdrs, "operator", SerializableDocument("DNV"))
    set!(custom_hdrs, "lastModified", SerializableDocument("2026-08-25T14:30:00Z"))

    h = DclHeader(ship_id, config_ref)
    set_version_information!(h, vi)
    set_author!(h, "Vista SDK Sample")
    set_date_created!(h, now(DateTimeOffset))
    set_custom_headers!(h, custom_hdrs)

    println("Created header:")
    println("  Ship ID         : $ship_id")
    println("  Config Reference: vessel-config-2026-v1")
    println("  Version         : dnv-v2")
    println("  Author          : Vista SDK Sample")
    println("  Date Created    : $(date_created(h))")
    println("  Custom Headers  : Present")
    println()
end

let
    println("2. DataChannelId: Creating channel identifiers")
    println("-------------------------------------------------")

    v = vis()
    gmod_ = gmod(v, latest(v))
    locs = locations(v, latest(v))
    cbs = codebooks(v, latest(v))

    primary_item = from_short_path("411.1/C101.63-1", gmod_, locs)
    qty_tag = create_tag(cbs[VistaSdk.Quantity], "angle")

    local_id = build(
        with_metadata_tag(
            with_primary_item(create(LocalIdBuilder, latest(v)), primary_item),
            qty_tag,
        ),
    )

    no = DclNameObject()
    set_naming_rule!(no, "dnv-v2")
    cid = DclDataChannelId(local_id)
    set_short_id!(cid, "GPSLatitude")
    set_name_object!(cid, no)

    println("Created DataChannelId:")
    println("  LocalId    : $local_id")
    println("  Short ID   : $(short_id(cid))")
    println("  Naming Rule: $(naming_rule(name_object(cid)))")
    println()
end

let
    println("3. Property: Creating properties with custom fields")
    println("------------------------------------------------------")

    dct = DclDataChannelType("Inst")
    fmt = DclFormat("Decimal")
    u = DclUnit("deg")
    r = DclRange(-90.0, 90.0)

    custom_props = sd_object()
    set!(custom_props, "coordinateSystem", SerializableDocument("WGS84"))
    set!(custom_props, "range", SerializableDocument("-90 to 90"))

    p = DclProperty(dct, fmt)
    set_range!(p, r)
    set_unit!(p, u)
    set_custom_properties!(p, custom_props)

    println("Created Property:")
    println("  Type             : $(type_(data_channel_type(p)))")
    println("  Format           : $(type_(format(p)))")
    println("  Unit             : $(unit_symbol(unit(p)))")
    println("  Custom Properties: Present")
    println()
end

let
    println("4. DataChannel: Creating complete data channels")
    println("--------------------------------------------------")

    v = vis()
    gmod_ = gmod(v, latest(v))
    locs = locations(v, latest(v))
    cbs = codebooks(v, latest(v))

    primary_item = from_short_path("411.1/C101.63-1", gmod_, locs)
    qty_tag = create_tag(cbs[VistaSdk.Quantity], "angle")
    local_id = build(
        with_metadata_tag(
            with_primary_item(create(LocalIdBuilder, latest(v)), primary_item),
            qty_tag,
        ),
    )

    no = DclNameObject()
    set_naming_rule!(no, "dnv-v2")
    cid = DclDataChannelId(local_id)
    set_short_id!(cid, "GPSLatitude")
    set_name_object!(cid, no)

    custom_props = sd_object()
    set!(custom_props, "coordinateSystem", SerializableDocument("WGS84"))
    set!(custom_props, "precision", SerializableDocument(Decimal(0.000001)))

    dct = DclDataChannelType("Inst")
    fmt = DclFormat("Decimal")
    p = DclProperty(dct, fmt)
    set_range!(p, DclRange(-90.0, 90.0))
    set_unit!(p, DclUnit("deg"))
    set_custom_properties!(p, custom_props)

    dc = DclDataChannel(cid, p)

    println("Created DataChannel:")
    println("  Short ID: $(short_id(channel_id(dc)))")
    println("  LocalId : $(local_id)")
    println("  Type    : $(type_(data_channel_type(property(dc))))")
    println("  Format  : $(type_(format(property(dc))))")
    println("  Unit    : $(unit_symbol(unit(property(dc))))")
    println()
end

let
    println("5. DataChannelList: Creating complete channel lists")
    println("------------------------------------------------------")

    v = vis()
    gmod_ = gmod(v, latest(v))
    locs = locations(v, latest(v))
    cbs = codebooks(v, latest(v))

    imo = parse(ImoNumber, "9074729")
    ship_id = from_imo_number(imo)
    ts = now(DateTimeOffset)
    config_ref = DclConfigurationReference("vessel-config-2026-v1", ts)
    set_version!(config_ref, "v1")

    custom_hdrs = sd_object()
    set!(custom_hdrs, "vesselType", SerializableDocument("Container Ship"))

    h = DclHeader(ship_id, config_ref)
    set_version_information!(h, let vi = DclVersionInformation()
        set_naming_rule!(vi, "dnv-v2")
        set_naming_scheme_version!(vi, "3.0a")
        vi
    end)
    set_author!(h, "Vista SDK Sample")
    set_date_created!(h, now(DateTimeOffset))
    set_custom_headers!(h, custom_hdrs)

    l = DclDataChannelList()

    # Channel 1: GPS Latitude
    let
        primary = from_short_path("411.1/C101.63-1", gmod_, locs)
        qty_tag = create_tag(cbs[VistaSdk.Quantity], "angle")
        local_id = build(
            with_metadata_tag(
                with_primary_item(create(LocalIdBuilder, latest(v)), primary),
                qty_tag,
            ),
        )
        no = DclNameObject()
        set_naming_rule!(no, "dnv-v2")
        cid = DclDataChannelId(local_id)
        set_short_id!(cid, "GPSLatitude")
        set_name_object!(cid, no)
        custom_props = sd_object()
        set!(custom_props, "coordinateSystem", SerializableDocument("WGS84"))
        p = DclProperty(DclDataChannelType("Inst"), DclFormat("Decimal"))
        set_range!(p, DclRange(-90.0, 90.0))
        set_unit!(p, DclUnit("deg"))
        set_custom_properties!(p, custom_props)
        add!(l, DclDataChannel(cid, p))
    end

    # Channel 2: GPS Longitude
    let
        primary = from_short_path("411.1/C101.63-2", gmod_, locs)
        qty_tag = create_tag(cbs[VistaSdk.Quantity], "angle")
        local_id = build(
            with_metadata_tag(
                with_primary_item(create(LocalIdBuilder, latest(v)), primary),
                qty_tag,
            ),
        )
        no = DclNameObject()
        set_naming_rule!(no, "dnv-v2")
        cid = DclDataChannelId(local_id)
        set_short_id!(cid, "GPSLongitude")
        set_name_object!(cid, no)
        custom_props = sd_object()
        set!(custom_props, "coordinateSystem", SerializableDocument("WGS84"))
        p = DclProperty(DclDataChannelType("Inst"), DclFormat("Decimal"))
        set_range!(p, DclRange(-180.0, 180.0))
        set_unit!(p, DclUnit("deg"))
        set_custom_properties!(p, custom_props)
        add!(l, DclDataChannel(cid, p))
    end

    # Channel 3: Engine Temperature
    let
        primary = from_short_path("411.1/C101.31-2", gmod_, locs)
        qty_tag = create_tag(cbs[VistaSdk.Quantity], "temperature")
        cnt_tag = create_tag(cbs[VistaSdk.Content], "exhaust.gas")
        local_id = build(
            with_metadata_tag(
                with_metadata_tag(
                    with_primary_item(create(LocalIdBuilder, latest(v)), primary),
                    qty_tag,
                ),
                cnt_tag,
            ),
        )
        no = DclNameObject()
        set_naming_rule!(no, "dnv-v2")
        cid = DclDataChannelId(local_id)
        set_short_id!(cid, "EngineTemp")
        set_name_object!(cid, no)
        custom_props = sd_object()
        set!(custom_props, "sensor", SerializableDocument("K-type thermocouple"))
        set!(custom_props, "maxValue", SerializableDocument(Decimal(1200.0)))
        p = DclProperty(DclDataChannelType("Average"), DclFormat("Decimal"))
        set_range!(p, DclRange(-50.0, 1200.0))
        set_unit!(p, DclUnit("degC"))
        set_custom_properties!(p, custom_props)
        add!(l, DclDataChannel(cid, p))
    end

    # Channel 4: Fuel Level
    let
        primary = from_short_path("621.21/S90", gmod_, locs)
        qty_tag = create_tag(cbs[VistaSdk.Quantity], "volume")
        cnt_tag = create_tag(cbs[VistaSdk.Content], "fuel.oil")
        local_id = build(
            with_metadata_tag(
                with_metadata_tag(
                    with_primary_item(create(LocalIdBuilder, latest(v)), primary),
                    qty_tag,
                ),
                cnt_tag,
            ),
        )
        no = DclNameObject()
        set_naming_rule!(no, "dnv-v2")
        cid = DclDataChannelId(local_id)
        set_short_id!(cid, "FuelLevel")
        set_name_object!(cid, no)
        custom_props = sd_object()
        set!(custom_props, "tankCapacity", SerializableDocument(Decimal(5000.0)))
        set!(custom_props, "alarmLevel", SerializableDocument(Decimal(500.0)))
        p = DclProperty(DclDataChannelType("Inst"), DclFormat("Decimal"))
        set_range!(p, DclRange(0.0, 5000.0))
        set_unit!(p, DclUnit("m3"))
        set_custom_properties!(p, custom_props)
        add!(l, DclDataChannel(cid, p))
    end

    pkg = DclPackage(h, l)
    println("Created Package with DataChannelList:")
    println("  Ship ID      : $ship_id")
    println("  Config Ref   : vessel-config-2026-v1")
    println("  Channel Count: $(length(l))")
    println()
    println("Channels:")
    for i = 1:length(l)
        dc = l[i]
        println(
            "  [$i] $(short_id(channel_id(dc))) ($(type_(data_channel_type(property(dc)))), Decimal)",
        )
    end
    println()
end

let
    println("6. Value Types: Working with different data types")
    println("----------------------------------------------------")

    sv = iso19848_value_from_string("test string")
    iv = iso19848_value_from_integer(42)
    dv = iso19848_value_from_decimal(Decimal(3.14159))
    bv = iso19848_value_from_boolean(true)
    dtv = iso19848_value_from_date_time(now(DateTimeOffset))

    println("Value type checks:")
    println("  String   value is string  : $(iso19848_value_to_string(sv))")
    println("  Integer  value is integer : $(iso19848_value_to_string(iv))")
    println("  Decimal  value is decimal : $(iso19848_value_to_string(dv))")
    println("  Boolean  value is boolean : $(iso19848_value_to_string(bv))")
    println("  DateTime value is dateTime: $(iso19848_value_to_string(dtv))")

    println("\nExtracting values:")
    println("  String  : $(iso19848_value_to_string(sv))")
    println("  Integer : $(iso19848_value_to_string(iv))")
    println("  Decimal : $(iso19848_value_to_string(dv))")
    println("  Boolean : $(iso19848_value_to_string(bv))")
    println("  DateTime: $(iso19848_value_to_string(dtv))")
    println()
end

let
    println("7. Validation: ISO 19848 field validation")
    println("--------------------------------------------")

    valid_dct = DclDataChannelType("Inst")
    println("[OK] Created valid DataChannelType: $(type_(valid_dct))")

    try
        dct2 = DclDataChannelType("Inst")
        set_type_!(dct2, "InvalidType")
        println("[ERROR] Created invalid DataChannelType (should have failed)")
    catch
        println("[OK] Correctly rejected invalid DataChannelType")
    end

    valid_fmt = DclFormat("Decimal")
    println("[OK] Created valid Format: $(type_(valid_fmt))")

    try
        fmt2 = DclFormat("Decimal")
        set_type_!(fmt2, "InvalidFormat")
        println("[ERROR] Created invalid Format (should have failed)")
    catch
        println("[OK] Correctly rejected invalid Format")
    end
    println()
end

let
    println("8. JSON Serialization: Converting to/from JSON")
    println("------------------------------------------------")

    v = vis()
    gmod_ = gmod(v, latest(v))
    locs = locations(v, latest(v))
    cbs = codebooks(v, latest(v))

    ship_id = from_string(ShipId, "IMO1234567")
    ts = now(DateTimeOffset)
    config_ref = DclConfigurationReference("demo-config-v1", ts)
    h = DclHeader(ship_id, config_ref)

    l = DclDataChannelList()

    for (sid, path, qty, rng, nm, rmk) in [
        (
            "GPSLatitude",
            "710.1/F211.11",
            "latitude",
            (-90.0, 90.0),
            "GPS Latitude",
            "Primary GPS latitude position",
        ),
        (
            "GPSLongitude",
            "710.1/F211.12",
            "longitude",
            (-180.0, 180.0),
            "GPS Longitude",
            "Primary GPS longitude position",
        ),
    ]
        primary = from_short_path(path, gmod_, locs)
        qty_tag = create_tag(cbs[VistaSdk.Quantity], qty)
        local_id = build(
            with_metadata_tag(
                with_primary_item(create(LocalIdBuilder, latest(v)), primary),
                qty_tag,
            ),
        )
        no = DclNameObject()
        set_naming_rule!(no, "dnv-v2")
        cid = DclDataChannelId(local_id)
        set_short_id!(cid, sid)
        set_name_object!(cid, no)
        custom_props = sd_object()
        set!(custom_props, "coordinateSystem", SerializableDocument("WGS84"))
        set!(custom_props, "geodeticDatum", SerializableDocument("WGS84"))
        p = DclProperty(DclDataChannelType("Inst"), DclFormat("Decimal"))
        set_range!(p, DclRange(rng[1], rng[2]))
        set_unit!(p, DclUnit("deg"))
        set_name!(p, nm)
        set_remarks!(p, rmk)
        set_custom_properties!(p, custom_props)
        add!(l, DclDataChannel(cid, p))
    end

    pkg = DclPackage(h, l)
    lp = DclListPackage(pkg)
    json = dcl_to_json(lp; pretty = true)

    println("Serialization result: Success")
    println("\nJSON output (formatted):")
    println(json)
    println()
end

let
    println("9. Advanced: Main Engine Monitoring System")
    println("--------------------------------------------")

    v = vis()
    gmod_ = gmod(v, latest(v))
    locs = locations(v, latest(v))
    cbs = codebooks(v, latest(v))

    imo = parse(ImoNumber, "9074729")
    ship_id = from_imo_number(imo)
    config_ref = DclConfigurationReference("main-engine-monitor-v3", now(DateTimeOffset))
    set_version!(config_ref, "3.0")

    vi = DclVersionInformation()
    set_naming_rule!(vi, "dnv-v2")
    set_naming_scheme_version!(vi, "3.10a")

    custom_hdrs = sd_object()
    set!(custom_hdrs, "monitoringSystem", SerializableDocument("Vista Engine Monitor"))
    set!(custom_hdrs, "engineManufacturer", SerializableDocument("MAN Energy Solutions"))
    set!(custom_hdrs, "engineModel", SerializableDocument("ME-C9.5-175"))
    set!(custom_hdrs, "samplingRate", SerializableDocument(Decimal(10.0)))

    h = DclHeader(ship_id, config_ref)
    set_version_information!(h, vi)
    set_author!(h, "Chief Engineer")
    set_date_created!(h, now(DateTimeOffset))
    set_custom_headers!(h, custom_hdrs)

    l = DclDataChannelList()

    # Channel 1: Engine Speed (RPM)
    let
        primary = from_short_path("411.1/C101.41", gmod_, locs)
        qty_tag = create_tag(cbs[VistaSdk.Quantity], "rotational.speed")
        local_id = build(
            with_metadata_tag(
                with_primary_item(create(LocalIdBuilder, latest(v)), primary),
                qty_tag,
            ),
        )
        custom_no = sd_object()
        set!(custom_no, "sensorType", SerializableDocument("Magnetic pickup"))
        set!(custom_no, "sensorManufacturer", SerializableDocument("Wartsila"))
        set!(custom_no, "calibrationDate", SerializableDocument("2026-07-16T10:00:00Z"))
        set!(custom_no, "isRedundant", SerializableDocument(true))
        no = DclNameObject()
        set_naming_rule!(no, "dnv-v2")
        set_custom_name_objects!(no, custom_no)
        cid = DclDataChannelId(local_id)
        set_short_id!(cid, "MainEngineRPM")
        set_name_object!(cid, no)
        custom_unit_elems = sd_object()
        set!(custom_unit_elems, "siEquivalent", SerializableDocument("hertz"))
        set!(custom_unit_elems, "conversionFactor", SerializableDocument(Decimal(60.0)))
        u = DclUnit("rpm")
        set_quantity_name!(u, "rotational speed")
        set_custom_elements!(u, custom_unit_elems)
        custom_props = sd_object()
        set!(custom_props, "nominalSpeed", SerializableDocument(Decimal(150.0)))
        set!(custom_props, "maxSpeed", SerializableDocument(Decimal(175.0)))
        set!(custom_props, "idleSpeed", SerializableDocument(Decimal(60.0)))
        set!(custom_props, "criticalAlarm", SerializableDocument(true))
        dct = DclDataChannelType("Inst")
        p = DclProperty(dct, DclFormat("Decimal"))
        set_range!(p, DclRange(0.0, 200.0))
        set_unit!(p, u)
        set_quality_coding!(p, "A")
        set_alert_priority!(p, "High")
        set_name!(p, "Main Engine Rotational Speed")
        set_remarks!(p, "Primary propulsion engine RPM")
        set_custom_properties!(p, custom_props)
        add!(l, DclDataChannel(cid, p))
    end

    # Channel 2: Lube Oil Pressure
    let
        primary = from_short_path("411.1/C101.663i", gmod_, locs)
        qty_tag = create_tag(cbs[VistaSdk.Quantity], "pressure")
        cnt_tag = create_tag(cbs[VistaSdk.Content], "lubricating.oil")
        pos_tag = create_tag(cbs[VistaSdk.Position], "inlet")
        local_id = build(
            with_metadata_tag(
                with_metadata_tag(
                    with_metadata_tag(
                        with_primary_item(create(LocalIdBuilder, latest(v)), primary),
                        qty_tag,
                    ),
                    cnt_tag,
                ),
                pos_tag,
            ),
        )
        custom_no = sd_object()
        set!(custom_no, "sensorType", SerializableDocument("Pressure transducer"))
        set!(custom_no, "sensorManufacturer", SerializableDocument("WIKA"))
        set!(custom_no, "sensorModel", SerializableDocument("A-10"))
        set!(custom_no, "accuracy", SerializableDocument(Decimal(0.5)))
        no = DclNameObject()
        set_naming_rule!(no, "dnv-v2")
        set_custom_name_objects!(no, custom_no)
        cid = DclDataChannelId(local_id)
        set_short_id!(cid, "LubeOilPressure")
        set_name_object!(cid, no)
        custom_unit_elems = sd_object()
        set!(custom_unit_elems, "pressureType", SerializableDocument("Gauge"))
        set!(custom_unit_elems, "siEquivalent", SerializableDocument("pascal"))
        set!(custom_unit_elems, "conversionFactor", SerializableDocument(Decimal(100000.0)))
        u = DclUnit("bar")
        set_quantity_name!(u, "pressure")
        set_custom_elements!(u, custom_unit_elems)
        custom_props = sd_object()
        set!(custom_props, "normalPressure", SerializableDocument(Decimal(4.2)))
        set!(custom_props, "minPressure", SerializableDocument(Decimal(2.5)))
        set!(custom_props, "alarmThreshold", SerializableDocument(Decimal(2.0)))
        set!(custom_props, "shutdownThreshold", SerializableDocument(Decimal(1.5)))
        dct = DclDataChannelType("Average")
        set_update_cycle!(dct, 5.0)
        set_calculation_period!(dct, 1.0)
        p = DclProperty(dct, DclFormat("Decimal"))
        set_range!(p, DclRange(0.0, 10.0))
        set_unit!(p, u)
        set_quality_coding!(p, "A")
        set_alert_priority!(p, "High")
        set_name!(p, "Lubricating Oil Inlet Pressure")
        set_remarks!(p, "Main engine lube oil system inlet pressure")
        set_custom_properties!(p, custom_props)
        add!(l, DclDataChannel(cid, p))
    end

    # Channel 3: Exhaust Gas Temperature
    let
        primary = from_short_path("411.1/C101.31-2", gmod_, locs)
        qty_tag = create_tag(cbs[VistaSdk.Quantity], "temperature")
        cnt_tag = create_tag(cbs[VistaSdk.Content], "exhaust.gas")
        local_id = build(
            with_metadata_tag(
                with_metadata_tag(
                    with_primary_item(create(LocalIdBuilder, latest(v)), primary),
                    qty_tag,
                ),
                cnt_tag,
            ),
        )
        custom_no = sd_object()
        set!(custom_no, "sensorType", SerializableDocument("K-type thermocouple"))
        set!(custom_no, "cylinderNumber", SerializableDocument(2))
        set!(custom_no, "maxTemp", SerializableDocument(Decimal(600.0)))
        set!(custom_no, "isCritical", SerializableDocument(true))
        no = DclNameObject()
        set_naming_rule!(no, "dnv-v2")
        set_custom_name_objects!(no, custom_no)
        cid = DclDataChannelId(local_id)
        set_short_id!(cid, "ExhaustTemp_Cyl2")
        set_name_object!(cid, no)
        custom_unit_elems = sd_object()
        set!(custom_unit_elems, "siEquivalent", SerializableDocument("kelvin"))
        set!(custom_unit_elems, "conversionOffset", SerializableDocument(Decimal(273.15)))
        u = DclUnit("degC")
        set_quantity_name!(u, "temperature")
        set_custom_elements!(u, custom_unit_elems)
        custom_props = sd_object()
        set!(custom_props, "normalTemp", SerializableDocument(Decimal(380.0)))
        set!(custom_props, "maxTemp", SerializableDocument(Decimal(450.0)))
        set!(custom_props, "alarmThreshold", SerializableDocument(Decimal(420.0)))
        set!(custom_props, "deviationAlarm", SerializableDocument(Decimal(30.0)))
        dct = DclDataChannelType("Average")
        set_update_cycle!(dct, 10.0)
        set_calculation_period!(dct, 10.0)
        p = DclProperty(dct, DclFormat("Decimal"))
        set_range!(p, DclRange(0.0, 600.0))
        set_unit!(p, u)
        set_quality_coding!(p, "A")
        set_alert_priority!(p, "High")
        set_name!(p, "Exhaust Gas Temperature Cylinder 2")
        set_remarks!(p, "Main engine cylinder 2 exhaust temperature")
        set_custom_properties!(p, custom_props)
        add!(l, DclDataChannel(cid, p))
    end

    # Channel 4: Fuel Consumption
    let
        primary = from_short_path("411.1/C101", gmod_, locs)
        sec = from_short_path("620.1/M201.32", gmod_, locs)
        qty_tag = create_tag(cbs[VistaSdk.Quantity], "volume.flow.rate")
        local_id = build(
            with_metadata_tag(
                with_secondary_item(
                    with_primary_item(create(LocalIdBuilder, latest(v)), primary),
                    sec,
                ),
                qty_tag,
            ),
        )
        custom_no = sd_object()
        set!(custom_no, "meterType", SerializableDocument("Coriolis flow meter"))
        set!(custom_no, "meterManufacturer", SerializableDocument("Endress+Hauser"))
        set!(custom_no, "meterModel", SerializableDocument("Promass 83F"))
        set!(custom_no, "accuracy", SerializableDocument(Decimal(0.1)))
        no = DclNameObject()
        set_naming_rule!(no, "dnv-v2")
        set_custom_name_objects!(no, custom_no)
        cid = DclDataChannelId(local_id)
        set_short_id!(cid, "FuelConsumption")
        set_name_object!(cid, no)
        custom_unit_elems = sd_object()
        set!(custom_unit_elems, "flowType", SerializableDocument("Volumetric"))
        set!(custom_unit_elems, "fluidType", SerializableDocument("HFO380"))
        set!(custom_unit_elems, "refTemperature", SerializableDocument(Decimal(15.0)))
        u = DclUnit("l/h")
        set_quantity_name!(u, "volume flow rate")
        set_custom_elements!(u, custom_unit_elems)
        custom_props = sd_object()
        set!(custom_props, "fuelGrade", SerializableDocument("IFO380"))
        set!(custom_props, "fuelDensity", SerializableDocument(Decimal(991.0)))
        set!(custom_props, "nominalConsumption", SerializableDocument(Decimal(2800.0)))
        set!(custom_props, "co2Factor", SerializableDocument(Decimal(3.114)))
        dct = DclDataChannelType("Average")
        set_update_cycle!(dct, 60.0)
        set_calculation_period!(dct, 60.0)
        p = DclProperty(dct, DclFormat("Decimal"))
        set_range!(p, DclRange(0.0, 5000.0))
        set_unit!(p, u)
        set_quality_coding!(p, "A")
        set_alert_priority!(p, "Normal")
        set_name!(p, "Main Engine Fuel Consumption")
        set_remarks!(p, "Hourly fuel consumption monitoring")
        set_custom_properties!(p, custom_props)
        add!(l, DclDataChannel(cid, p))
    end

    pkg = DclPackage(h, l)
    lp = DclListPackage(pkg)
    engine_json = dcl_to_json(lp; pretty = true)

    println("Main Engine Monitoring System:")
    println("  Engine   : MAN Energy Solutions ME-C9.5-175")
    println("  Channels : $(length(l))")
    println("  Custom fields: Present")
    println("\nMonitored parameters:")
    for i = 1:length(l)
        dc = l[i]
        nm = name(property(dc))
        print("  [$i] $(short_id(channel_id(dc)))")
        if nm !== nothing
            print(" - $nm")
        end
        println()
    end
    println("\nEngine Monitoring JSON:")
    println(engine_json)
    println()
end

let
    println("10. Advanced: DTO-level manipulation before serialization")
    println("---------------------------------------------------------")

    ship_id = from_string(ShipId, "IMO8027781")
    ts = now(DateTimeOffset)
    config_ref = DclConfigurationReference("dto-patch-demo", ts)
    set_version!(config_ref, "1.0")
    h = DclHeader(ship_id, config_ref)
    l = DclDataChannelList()
    domain_pkg = DclPackage(h, l)
    lp = DclListPackage(domain_pkg)

    compact_json = dcl_to_json(lp; pretty = false)
    dto = dcl_dto_from_json(compact_json)

    dto_pkg = pkg(dto)
    dto_hdr = header(dto_pkg)
    set_author!(dto_hdr, "export-pipeline")

    custom = ensure_custom_headers!(dto_hdr)
    set!(custom, "exportedBy", SerializableDocument("vista-sdk-sample"))

    patched_json = dcl_dto_to_json(dto, true)

    println("DTO patched with author field and exportedBy custom header:")
    println(patched_json)
    println()
end
