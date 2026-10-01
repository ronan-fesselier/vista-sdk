using VistaSdk

println("=== vista-sdk TimeSeriesData Sample ===\n")

let
    println("1. Header: Creating TimeSeriesData package headers")
    println("-----------------------------------------------------")

    imo = parse(ImoNumber, "9074729")
    ship_id = from_imo_number(imo)
    start = parse(DateTimeOffset, "2026-08-27T10:00:00Z")
    stop = parse(DateTimeOffset, "2026-08-27T14:00:00Z")
    ts = TsdTimeSpan(start, stop)
    dt_created = parse(DateTimeOffset, "2026-08-27T10:00:00Z")
    dt_modified = parse(DateTimeOffset, "2026-08-27T14:30:00Z")
    sys_cfg = TsdConfigRef(
        "SystemConfiguration.xml",
        parse(DateTimeOffset, "2026-08-27T00:00:00Z"),
    )

    h = TsdHeader(ship_id)
    set_time_span!(h, ts)
    set_date_created!(h, dt_created)
    set_date_modified!(h, dt_modified)
    set_author!(h, "Vista SDK Sample")
    set_system_configuration!(h, [sys_cfg])

    println("Created header:")
    println("  Ship ID       : $ship_id")
    let tspan = time_span(h)
        if tspan !== nothing
            println("  Time Span     : $(start_time(tspan)) - $(end_time(tspan))")
        end
    end
    println("  Date Created  : $(date_created(h))")
    println("  Date Modified : $(date_modified(h))")
    println("  Author        : $(author(h))")
    println("  System Configs: $(system_configuration_count(h)) entries")
    println()
end

let
    println("2. TabularData: Creating tabular time series data")
    println("----------------------------------------------------")

    ch1 = from_string(TsdChannelId, "Temperature")
    ch2 = from_string(TsdChannelId, "Pressure")
    ts1 = parse(DateTimeOffset, "2026-08-27T10:00:00Z")
    ts2 = parse(DateTimeOffset, "2026-08-27T11:00:00Z")
    ts3 = parse(DateTimeOffset, "2026-08-27T12:00:00Z")
    ds1 = TabularDataSet(ts1, ["100.5", "200.0"])
    ds2 = TabularDataSet(ts2, ["105.2", "205.5"])
    ds3 = TabularDataSet(ts3, ["110.0", "210.0"])
    set_quality!(ds1, ["0", "0"])
    set_quality!(ds2, ["0", "0"])
    set_quality!(ds3, ["0", "0"])
    td = TabularData([ch1, ch2], [ds1, ds2, ds3])

    println("Created TabularData:")
    println("  Data Channels: $(channel_id_count(td))")
    println("  Data Sets    : $(data_set_count(td))")
    println("  Validation   : $(validate(td))")
    println()
end

let
    println("3. EventData: Creating event-based time series data")
    println("------------------------------------------------------")

    ch = from_string(TsdChannelId, "AlarmStatus")
    ts1 = parse(DateTimeOffset, "2026-08-27T10:30:00Z")
    ts2 = parse(DateTimeOffset, "2026-08-27T11:45:00Z")
    eds1 = EventDataSet(ts1, ch, "HIGH")
    eds2 = EventDataSet(ts2, ch, "NORMAL")
    set_quality!(eds1, "0")
    set_quality!(eds2, "0")
    ed = EventData()
    set_data_sets!(ed, [eds1, eds2])

    println("Created EventData:")
    println("  Event Data Sets: $(data_set_count(ed))")
    println()
end

let
    println("4. TimeSeriesData: Combining tabular and event data")
    println("------------------------------------------------------")

    data_cfg =
        TsdConfigRef("DataChannelList.xml", parse(DateTimeOffset, "2026-08-27T00:00:00Z"))
    ch1 = from_string(TsdChannelId, "Temp1")
    ch2 = from_string(TsdChannelId, "Press1")
    ds = TabularDataSet(parse(DateTimeOffset, "2026-08-27T10:00:00Z"), ["100.0", "200.0"])
    td = TabularData([ch1, ch2], [ds])

    alarm_ch = from_string(TsdChannelId, "Alarm1")
    eds = EventDataSet(parse(DateTimeOffset, "2026-08-27T10:30:00Z"), alarm_ch, "HIGH")
    set_quality!(eds, "0")
    ed = EventData()
    set_data_sets!(ed, [eds])

    tsd = TimeSeriesData()
    set_data_configuration!(tsd, data_cfg)
    set_tabular_data!(tsd, [td])
    set_event_data!(tsd, ed)

    println("Created TimeSeriesData:")
    let cfg = data_configuration(tsd)
        if cfg !== nothing
            println("  DataConfig  : $(config_id(cfg)) @ $(time_stamp(cfg))")
        end
    end
    for i = 1:tabular_data_count(tsd)
        tab = tabular_data_at(tsd, i)
        labels = [string(channel_id_at(tab, j)) for j = 1:channel_id_count(tab)]
        println(
            "  TabularData [$i]: $(channel_id_count(tab)) channel(s), $(data_set_count(tab)) data set(s)",
        )
        println("    channels: $(join(labels, ", "))")
        for j = 1:data_set_count(tab)
            set_ = data_set_at(tab, j)
            q = quality(set_)
            q_str = q !== nothing ? join(q, ", ") : "-"
            println(
                "    [$j] ts=$(time_stamp(set_)) values=[$(join(VistaSdk.values(set_), ", "))] quality=$q_str",
            )
        end
    end
    let ev = event_data(tsd)
        if ev !== nothing
            println("  EventData: $(data_set_count(ev)) event(s)")
            for i = 1:data_set_count(ev)
                eds_ = data_set_at(ev, i)
                q = quality(eds_)
                println(
                    "    [$i] ts=$(time_stamp(eds_)) channel=$(channel_id(eds_)) value=$(value(eds_)) quality=$(q !== nothing ? q : "-")",
                )
            end
        end
    end
    println()
end

let
    println("5. Package: Creating complete TimeSeriesData package")
    println("-------------------------------------------------------")

    imo = parse(ImoNumber, "9074729")
    ship_id = from_imo_number(imo)
    ts = TsdTimeSpan(
        parse(DateTimeOffset, "2026-08-27T10:00:00Z"),
        parse(DateTimeOffset, "2026-08-27T14:00:00Z"),
    )

    h = TsdHeader(ship_id)
    set_time_span!(h, ts)
    set_date_created!(h, parse(DateTimeOffset, "2026-08-27T10:00:00Z"))
    set_date_modified!(h, parse(DateTimeOffset, "2026-08-27T14:30:00Z"))
    set_author!(h, "Vista SDK")

    data_cfg =
        TsdConfigRef("DataChannelList.xml", parse(DateTimeOffset, "2026-08-27T00:00:00Z"))
    ch = from_string(TsdChannelId, "Sensor1")
    ds = TabularDataSet(parse(DateTimeOffset, "2026-08-27T10:00:00Z"), ["100.0"])
    td = TabularData([ch], [ds])

    tsd = TimeSeriesData()
    set_data_configuration!(tsd, data_cfg)
    set_tabular_data!(tsd, [td])

    tsd_pkg = TsdPackage(h, [tsd])
    p = TimeSeriesDataPackage(tsd_pkg)

    println("Created TimeSeriesDataPackage:")
    println("  Ship ID        : $ship_id")
    let tspan = time_span(h)
        if tspan !== nothing
            println("  Time Span      : $(start_time(tspan)) - $(end_time(tspan))")
        end
    end
    println(
        "  Date Created   : $(let dc_ = date_created(h); dc_ !== nothing ? dc_ : "-" end)",
    )
    println("  Author        : $(let a = author(h); a !== nothing ? a : "-" end)")
    println("  TimeSeriesData: $(time_series_data_count(tsd_pkg)) block(s)")
    for i = 1:time_series_data_count(tsd_pkg)
        t = time_series_data_at(tsd_pkg, i)
        let cfg = data_configuration(t)
            if cfg !== nothing
                println("    [$i] DataConfig   : $(config_id(cfg))")
            end
        end
        println("    [$i] TabularData  : $(tabular_data_count(t)) block(s)")
        let ev = event_data(t)
            if ev !== nothing
                println("    [$i] EventData    : $(data_set_count(ev)) event(s)")
            end
        end
    end
    println()
end

let
    println("6. JSON Serialization: Round-trip serialization")
    println("--------------------------------------------------")

    imo = parse(ImoNumber, "1234567")
    ship_id = from_imo_number(imo)
    h = TsdHeader(ship_id)
    set_date_created!(h, parse(DateTimeOffset, "2026-08-27T10:00:00Z"))

    data_cfg =
        TsdConfigRef("DataChannelList.xml", parse(DateTimeOffset, "2026-08-27T00:00:00Z"))
    ch = from_string(TsdChannelId, "Ch1")
    ds = TabularDataSet(parse(DateTimeOffset, "2026-08-27T10:00:00Z"), ["42.0"])
    td = TabularData([ch], [ds])

    tsd = TimeSeriesData()
    set_data_configuration!(tsd, data_cfg)
    set_tabular_data!(tsd, [td])

    tsd_pkg = TsdPackage(h, [tsd])
    original_package = TimeSeriesDataPackage(tsd_pkg)

    json_str = tsd_to_json(original_package, true)
    println("Serialized ($(length(json_str)) bytes):")
    println(json_str)

    deserialized = tsd_from_json(json_str)
    n = time_series_data_count(tsd_pkg)
    println("Round-trip check:")
    println(
        "  TimeSeriesData blocks: $(time_series_data_count(deserialized)) (original: $n)",
    )
    for i = 1:n
        t = time_series_data_at(tsd_pkg, i)
        let cfg = data_configuration(t)
            if cfg !== nothing
                println("  [$i] DataConfig : $(config_id(cfg))")
            end
        end
        for j = 1:tabular_data_count(t)
            tab = tabular_data_at(t, j)
            channels = [string(channel_id_at(tab, k)) for k = 1:channel_id_count(tab)]
            for k = 1:data_set_count(tab)
                set_ = data_set_at(tab, k)
                println(
                    "  [$j] channels=[$(join(channels, ", "))] ts=$(time_stamp(set_)) values=[$(join(VistaSdk.values(set_), ", "))]",
                )
            end
        end
    end
    println()
end

let
    println("7. JSON Serialization: TabularData + EventData combined")
    println("----------------------------------------------------------")

    imo = parse(ImoNumber, "1234567")
    ship_id = from_imo_number(imo)
    h = TsdHeader(ship_id)
    set_date_created!(h, parse(DateTimeOffset, "2026-08-27T10:00:00Z"))

    data_cfg =
        TsdConfigRef("DataChannelList.xml", parse(DateTimeOffset, "2026-08-27T00:00:00Z"))
    ch = from_string(TsdChannelId, "Ch1")
    ds = TabularDataSet(parse(DateTimeOffset, "2026-08-27T10:00:00Z"), ["42.0"])
    td = TabularData([ch], [ds])

    alarm_ch = from_string(TsdChannelId, "Alarm1")
    eds = EventDataSet(
        parse(DateTimeOffset, "2026-08-27T10:00:42Z"),
        alarm_ch,
        "alarm.active",
    )
    set_quality!(eds, "0")
    ed = EventData()
    set_data_sets!(ed, [eds])

    tsd = TimeSeriesData()
    set_data_configuration!(tsd, data_cfg)
    set_tabular_data!(tsd, [td])
    set_event_data!(tsd, ed)

    tsd_pkg = TsdPackage(h, [tsd])
    p = TimeSeriesDataPackage(tsd_pkg)
    json_str = tsd_to_json(p, true)
    println("Serialized to JSON ($(length(json_str)) bytes)")
    println(json_str)
    println()
end

let
    println("8. Cross-Validation: Validating against DataChannelList")
    println("----------------------------------------------------------")

    v = vis()
    gmod_ = gmod(v, latest(v))
    locs = locations(v, latest(v))
    cbs = codebooks(v, latest(v))

    imo = parse(ImoNumber, "9074729")
    ship_id = from_imo_number(imo)
    config_ref = DclConfigurationReference(
        "DataChannelList.xml",
        parse(DateTimeOffset, "2026-08-27T00:00:00Z"),
    )

    primary_item = from_short_path("411.1/C101.31-2", gmod_, locs)
    qty_tag = create_tag(cbs[VistaSdk.Quantity], "temperature")
    local_id = build(
        with_metadata_tag(
            with_primary_item(create(LocalIdBuilder, latest(v)), primary_item),
            qty_tag,
        ),
    )

    dcl_cid = DclDataChannelId(local_id)
    set_short_id!(dcl_cid, "TempSensor")

    restr = DclRestriction()
    set_fraction_digits!(restr, 1)
    fmt = DclFormat("Decimal")
    set_restriction!(fmt, restr)
    p = DclProperty(DclDataChannelType("Inst"), fmt)
    set_range!(p, DclRange(0.0, 200.0))
    set_unit!(p, DclUnit("degC"))

    dcl = DclDataChannelList()
    add!(dcl, DclDataChannel(dcl_cid, p))
    dcl_h = DclHeader(ship_id, config_ref)
    dcl_p = DclListPackage(DclPackage(dcl_h, dcl))

    ts_cfg =
        TsdConfigRef("DataChannelList.xml", parse(DateTimeOffset, "2026-08-27T00:00:00Z"))
    ts_ch = from_string(TsdChannelId, "TempSensor")
    ds = TabularDataSet(parse(DateTimeOffset, "2026-08-27T10:00:00Z"), ["100.5"])
    td = TabularData([ts_ch], [ds])
    eds = EventDataSet(parse(DateTimeOffset, "2026-08-27T10:15:00Z"), ts_ch, "105.2")
    set_quality!(eds, "0")
    ed = EventData()
    set_data_sets!(ed, [eds])

    tsd = TimeSeriesData()
    set_data_configuration!(tsd, ts_cfg)
    set_tabular_data!(tsd, [td])
    set_event_data!(tsd, ed)

    result = validate(
        tsd,
        dcl_p,
        (ts, dc_, val, qual) -> begin
            sid = short_id(channel_id(dc_))
            println(
                "  Tabular data at $ts | channel: $(sid !== nothing ? sid : "?") | value: $val | quality: $(qual !== nothing ? qual : "-")",
            )
            true
        end,
        (ts, dc_, val, qual) -> begin
            sid = short_id(channel_id(dc_))
            println(
                "  Event data at $ts   | channel: $(sid !== nothing ? sid : "?") | value: $val | quality: $(qual !== nothing ? qual : "-")",
            )
            true
        end,
    )

    println("Cross-validation result:")
    println("  Is valid: $(is_valid(result))")
    if !is_valid(result)
        for err in errors(result)
            println("    - $err")
        end
    end
    println()
end

let
    println("9. Custom Validation: Business rule callbacks")
    println("------------------------------------------------")

    v = vis()
    gmod_ = gmod(v, latest(v))
    locs = locations(v, latest(v))
    cbs = codebooks(v, latest(v))

    imo = parse(ImoNumber, "1234567")
    ship_id = from_imo_number(imo)
    config_ref = DclConfigurationReference(
        "DataChannelList.xml",
        parse(DateTimeOffset, "2026-08-27T00:00:00Z"),
    )

    primary_item = from_short_path("411.1/C101", gmod_, locs)
    qty_tag = create_tag(cbs[VistaSdk.Quantity], "temperature")
    local_id = build(
        with_metadata_tag(
            with_primary_item(create(LocalIdBuilder, latest(v)), primary_item),
            qty_tag,
        ),
    )

    dcl_cid = DclDataChannelId(local_id)
    set_short_id!(dcl_cid, "ExhaustTemp")

    p = DclProperty(DclDataChannelType("Inst"), DclFormat("Decimal"))
    set_range!(p, DclRange(0.0, 450.0))
    set_unit!(p, DclUnit("deg C"))
    set_alert_priority!(p, "high-temperature")

    dcl = DclDataChannelList()
    add!(dcl, DclDataChannel(dcl_cid, p))
    dcl_h = DclHeader(ship_id, config_ref)
    dcl_p = DclListPackage(DclPackage(dcl_h, dcl))

    ts_cfg =
        TsdConfigRef("DataChannelList.xml", parse(DateTimeOffset, "2026-08-27T00:00:00Z"))
    ts_ch = from_string(TsdChannelId, "ExhaustTemp")
    ds = TabularDataSet(parse(DateTimeOffset, "2026-08-27T10:00:00Z"), ["420.0"])
    td = TabularData([ts_ch], [ds])
    eds = EventDataSet(parse(DateTimeOffset, "2026-08-27T11:00:00Z"), ts_ch, "380.0")
    set_quality!(eds, "0")
    ed = EventData()
    set_data_sets!(ed, [eds])

    tsd = TimeSeriesData()
    set_data_configuration!(tsd, ts_cfg)
    set_tabular_data!(tsd, [td])
    set_event_data!(tsd, ed)

    result = validate(
        tsd,
        dcl_p,
        (ts, dc_, val, _qual) -> begin
            sid = short_id(channel_id(dc_))
            println(
                "  Exhaust temp tabular at $ts | channel: $(sid !== nothing ? sid : "?") | value: $val",
            )
            upper = Decimal(high(VistaSdk.range(property(dc_))))
            cruise = Decimal(400.0)
            String[
                "Value $val triggered alarm 'high-temperature': approaching upper limit of $upper deg C",
                "Value $val exceeds recommended cruise threshold of $cruise deg C",
            ]
        end,
        (ts, dc_, val, _qual) -> begin
            sid = short_id(channel_id(dc_))
            println(
                "  Exhaust temp event at $ts   | channel: $(sid !== nothing ? sid : "?") | value: $val",
            )
            true
        end,
    )

    println("Custom validation result:")
    println("  Is valid: $(is_valid(result))")
    if !is_valid(result)
        for err in errors(result)
            println("    - $err")
        end
    end
    println()
end

let
    println("10. CustomHeaders & CustomDataKinds: Extension data support")
    println("-------------------------------------------------------------")

    custom_hdrs = sd_object()
    set!(custom_hdrs, "version", SerializableDocument("1.2.3"))
    set!(custom_hdrs, "recordCount", SerializableDocument(42))
    set!(custom_hdrs, "isCompressed", SerializableDocument(true))
    set!(custom_hdrs, "temperature", SerializableDocument(Decimal(98.6)))
    set!(custom_hdrs, "createdAt", SerializableDocument("2026-08-27T10:00:00Z"))

    imo = parse(ImoNumber, "1234567")
    ship_id = from_imo_number(imo)
    h = TsdHeader(ship_id)
    set_date_created!(h, parse(DateTimeOffset, "2026-08-27T10:00:00Z"))
    set_author!(h, "CustomData Demo")
    set_custom_headers!(h, custom_hdrs)

    custom_dk = sd_object()
    set!(custom_dk, "sensorModel", SerializableDocument("TempSensor-XYZ-2000"))
    set!(custom_dk, "calibrationDate", SerializableDocument("2026-08-27T00:00:00Z"))
    set!(custom_dk, "sampleRate", SerializableDocument(1000))
    set!(custom_dk, "accuracy", SerializableDocument(Decimal(0.001)))
    set!(custom_dk, "validated", SerializableDocument(true))

    data_cfg =
        TsdConfigRef("DataChannelList.xml", parse(DateTimeOffset, "2026-08-27T00:00:00Z"))
    ch = from_string(TsdChannelId, "Sensor1")
    ds = TabularDataSet(parse(DateTimeOffset, "2026-08-27T10:00:00Z"), ["100.0"])
    td = TabularData([ch], [ds])

    tsd = TimeSeriesData()
    set_data_configuration!(tsd, data_cfg)
    set_tabular_data!(tsd, [td])
    set_custom_data_kinds!(tsd, custom_dk)

    tsd_pkg = TsdPackage(h, [tsd])
    p = TimeSeriesDataPackage(tsd_pkg)
    json_str = tsd_to_json(p, true)

    println("Created package with custom extension data")
    println("\nSerialized JSON with custom data:")
    println(json_str)
    println()
end

let
    println("11. DataChannelId: dispatching on LocalId vs ShortId")
    println("--------------------------------------------------------")

    v = vis()
    gmod_ = gmod(v, latest(v))
    locs = locations(v, latest(v))
    cbs = codebooks(v, latest(v))

    primary_item = from_short_path("411.1", gmod_, locs)
    qty_tag = create_tag(cbs[VistaSdk.Quantity], "temperature")
    lid = build(
        with_metadata_tag(
            with_primary_item(create(LocalIdBuilder, latest(v)), primary_item),
            qty_tag,
        ),
    )

    channel_ids =
        [from_string(TsdChannelId, string(lid)), from_string(TsdChannelId, "Sensor42")]

    for cid in channel_ids
        if is_local_id(cid)
            println("  LocalId channel : $(local_id(cid))")
        else
            println("  ShortId channel : '$(short_id(cid))'")
        end
    end
    println()
end

let
    println("12. Advanced: DTO-level manipulation before serialization")
    println("---------------------------------------------------------")

    imo = parse(ImoNumber, "8027781")
    ship_id = from_imo_number(imo)
    h = TsdHeader(ship_id)
    tsd_pkg = TsdPackage(h, TimeSeriesData[])
    domain = TimeSeriesDataPackage(tsd_pkg)

    dto = tsd_to_dto(domain)
    dto_pkg = pkg(dto)
    dto_hdr = header(dto_pkg)

    set_author!(dto_hdr, "export-pipeline")
    set_date_modified!(dto_hdr, string(now(DateTimeOffset)))

    custom_hdrs = ensure_custom_headers!(dto_hdr)
    set!(custom_hdrs, "exportedBy", SerializableDocument("vista-sdk-sample"))

    patched_json = tsd_dto_to_json(dto, true)

    println("DTO patched with author, dateModified and exportedBy custom header:")
    println(patched_json)
    println()
end
