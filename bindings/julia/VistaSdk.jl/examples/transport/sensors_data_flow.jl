using VistaSdk

println("=== vista-sdk SensorDataFlow Sample ===\n")

function _create_data_channel_list()
    dcl = DclDataChannelList()

    local_id1 = from_string(
        LocalId,
        "/dnv-v2/vis-3-4a/411.1-1/C101.63/S206/meta/qty-temperature/cnt-cooling.air",
    )
    dc_id1 = DclDataChannelId(local_id1)
    set_short_id!(dc_id1, "TEMP001")

    restriction1 = DclRestriction()
    set_fraction_digits!(restriction1, 1)
    set_max_inclusive!(restriction1, 200.0)
    set_min_inclusive!(restriction1, -50.0)
    format1 = DclFormat("Decimal")
    set_restriction!(format1, restriction1)
    dct1 = DclDataChannelType("Inst")
    set_update_cycle!(dct1, 1.0)

    range1 = DclRange(0.0, 150.0)
    unit1 = DclUnit("degC")
    set_quantity_name!(unit1, "Temperature")

    prop1 = DclProperty(dct1, format1)
    set_range!(prop1, range1)
    set_unit!(prop1, unit1)
    set_quality_coding!(prop1, "OPC_QUALITY")
    set_name!(prop1, "Main Engine Air Cooler Temperature")

    add!(dcl, DclDataChannel(dc_id1, prop1))

    local_id2 = from_string(LocalId, "/dnv-v2/vis-3-4a/511.15-1/E32/meta/qty-power")
    dc_id2 = DclDataChannelId(local_id2)
    set_short_id!(dc_id2, "PWR001")

    format2 = DclFormat("Decimal")
    dct2 = DclDataChannelType("Inst")
    set_update_cycle!(dct2, 1.0)
    range2 = DclRange(0.0, 10000.0)
    unit2 = DclUnit("kW")
    set_quantity_name!(unit2, "Power")

    prop2 = DclProperty(dct2, format2)
    set_range!(prop2, range2)
    set_unit!(prop2, unit2)
    set_name!(prop2, "Generator Power Output")

    add!(dcl, DclDataChannel(dc_id2, prop2))

    local_id3 =
        from_string(LocalId, "/dnv-v2/vis-3-4a/621.22i/S110/meta/cnt-diesel.oil/cmd-stop")
    dc_id3 = DclDataChannelId(local_id3)
    set_short_id!(dc_id3, "ALT001")

    format3 = DclFormat("Boolean")
    dct3 = DclDataChannelType("Command")

    prop3 = DclProperty(dct3, format3)
    set_name!(prop3, "Diesel Oil Stop Command")

    add!(dcl, DclDataChannel(dc_id3, prop3))

    ship_id = from_imo_number(parse(ImoNumber, "1234567"))
    config_ref = DclConfigurationReference(
        "DataChannelList-v1",
        parse(DateTimeOffset, "2026-08-28T00:00:00Z"),
    )
    version_info = DclVersionInformation("dnv", "v2")
    set_reference_url!(version_info, "https://docs.vista.dnv.com")

    header = DclHeader(ship_id, config_ref)
    set_version_information!(header, version_info)
    set_author!(header, "Vista SDK Sample")
    set_date_created!(header, parse(DateTimeOffset, "2026-08-28T00:00:00Z"))

    package = DclPackage(header, dcl)
    DclListPackage(package)
end

const DCL_PACKAGE = _create_data_channel_list()

const BASE_TIME = parse(DateTimeOffset, "2026-08-28T10:00:00Z")
const BASE_PLUS_5 = parse(DateTimeOffset, "2026-08-28T10:05:00Z")

struct SensorReading
    system_id::String
    value::Float64
    timestamp::DateTimeOffset
    quality::String
end

const READINGS = SensorReading[
    SensorReading("TEMP001", 45.2, BASE_TIME, "Good"),
    SensorReading("TEMP001", 46.1, BASE_PLUS_5, "Good"),
    SensorReading("PWR001", 2500.0, BASE_TIME, "Good"),
    SensorReading("PWR001", 2650.5, BASE_PLUS_5, "Good"),
    SensorReading("ALT001", 1.0, BASE_TIME, "Good"),
    SensorReading("UNKNOWN", 123.4, BASE_TIME, "Good"),
]

format_double(value::Float64) = string(value)

let
    println("1. DataChannelList: Creating sample data channel list")
    println("--------------------------------------------------------")

    dcl_ref = data_channel_list(DCL_PACKAGE)
    println("Created DataChannelList with $(length(dcl_ref)) channels:")

    for i = 1:length(dcl_ref)
        dc = dcl_ref[i]
        cycle = update_cycle(data_channel_type(property(dc)))
        cycle_str = cycle !== nothing ? "$(format_double(cycle))s" : "None"
        sid = short_id(channel_id(dc))
        sid_str = sid !== nothing ? sid : "<no-short-id>"
        println("  $sid_str (updateCycle=$cycle_str)")
    end

    println()
    json_str = dcl_to_json(DCL_PACKAGE; pretty = true)
    println("Serialized to JSON: $(length(json_str)) bytes")
    println("Package:\n$json_str")
    println()
end

let
    println("2. Proprietary sensor data: Simulating sensor readings")
    println("-----------------------------------------------------------")

    println("Created $(length(READINGS)) sensor readings from proprietary system")
    println("Sample readings:")
    for r in READINGS
        println("  SystemID=$(r.system_id), Value=$(r.value), Timestamp=$(r.timestamp)")
    end
    println()
end

let
    println("3. Validation: Filter readings against DataChannelList")
    println("-----------------------------------------------------------")

    valid_readings = Dict{String,Vector{SensorReading}}()

    for reading in READINGS
        dcl_ref = data_channel_list(DCL_PACKAGE)
        if from_short_id(dcl_ref, reading.system_id) !== nothing
            push!(get!(valid_readings, reading.system_id, SensorReading[]), reading)
            println("  Accepted: $(reading.system_id) = $(reading.value)")
        else
            println("  Rejected: $(reading.system_id) (unknown channel)")
        end
    end

    println("\nAccepted $(length(valid_readings)) unique channels with valid data")
    println()
end

let
    println("4. Grouping: Group channels by updateCycle (ISO19848 optimization)")
    println("-------------------------------------------------------------------------")

    valid_readings = Dict{String,Vector{SensorReading}}()
    for reading in READINGS
        dcl_ref = data_channel_list(DCL_PACKAGE)
        if from_short_id(dcl_ref, reading.system_id) !== nothing
            push!(get!(valid_readings, reading.system_id, SensorReading[]), reading)
        end
    end

    channels_by_cycle = Dict{String,Vector{Tuple{String,String}}}()

    for system_id in keys(valid_readings)
        dcl_ref = data_channel_list(DCL_PACKAGE)
        dc = from_short_id(dcl_ref, system_id)
        if dc !== nothing
            cycle = update_cycle(data_channel_type(property(dc)))
            cycle_key = cycle !== nothing ? format_double(cycle) : "none"
            local_id_str = local_id_string(channel_id(dc))
            local_id_str = local_id_str !== nothing ? local_id_str : ""
            push!(
                get!(channels_by_cycle, cycle_key, Tuple{String,String}[]),
                (local_id_str, system_id),
            )
        end
    end

    println("Grouped channels into $(length(channels_by_cycle)) updateCycle groups:")
    for (cycle_key, channels) in channels_by_cycle
        cycle_str = cycle_key == "none" ? "None" : "$(cycle_key)s"
        println("  updateCycle=$cycle_str: $(length(channels)) channels")
    end
    println()
end

let
    println("5. TimeSeriesData: Build TabularData for each group")
    println("---------------------------------------------------------")

    valid_readings = Dict{String,Vector{SensorReading}}()
    for reading in READINGS
        dcl_ref = data_channel_list(DCL_PACKAGE)
        if from_short_id(dcl_ref, reading.system_id) !== nothing
            push!(get!(valid_readings, reading.system_id, SensorReading[]), reading)
        end
    end

    channels_by_cycle = Dict{String,Vector{Tuple{String,String}}}()
    for system_id in keys(valid_readings)
        dcl_ref = data_channel_list(DCL_PACKAGE)
        dc = from_short_id(dcl_ref, system_id)
        if dc !== nothing
            cycle = update_cycle(data_channel_type(property(dc)))
            cycle_key = cycle !== nothing ? format_double(cycle) : "none"
            local_id_str = local_id_string(channel_id(dc))
            local_id_str = local_id_str !== nothing ? local_id_str : ""
            push!(
                get!(channels_by_cycle, cycle_key, Tuple{String,String}[]),
                (local_id_str, system_id),
            )
        end
    end

    tabular_data_list = TabularData[]

    for (cycle_key, channel_pairs) in channels_by_cycle
        all_timestamps = DateTimeOffset[]
        for (_, system_id) in channel_pairs
            for r in get(valid_readings, system_id, SensorReading[])
                if !any(t -> ticks(t) == ticks(r.timestamp), all_timestamps)
                    push!(all_timestamps, r.timestamp)
                end
            end
        end
        sort!(all_timestamps; by = ticks)

        data_channel_ids = TsdChannelId[]
        for (local_id_str, _) in channel_pairs
            id = from_string(TsdChannelId, local_id_str)
            id !== nothing && push!(data_channel_ids, id)
        end

        data_sets = TabularDataSet[]
        for ts in all_timestamps
            vals = String[]
            qualities = String[]

            for (_, system_id) in channel_pairs
                dcl_ref = data_channel_list(DCL_PACKAGE)
                dc = from_short_id(dcl_ref, system_id)
                format_type = dc !== nothing ? type_(format(property(dc))) : ""

                rs = get(valid_readings, system_id, SensorReading[])
                idx = findfirst(r -> ticks(r.timestamp) == ticks(ts), rs)
                if idx !== nothing
                    r = rs[idx]
                    if format_type == "Boolean"
                        push!(vals, r.value != 0.0 ? "True" : "False")
                    else
                        push!(vals, format_double(r.value))
                    end
                    push!(qualities, r.quality)
                else
                    push!(vals, "")
                    push!(qualities, "Bad")
                end
            end

            ds = TabularDataSet(ts, vals)
            set_quality!(ds, qualities)
            push!(data_sets, ds)
        end

        cycle_str = cycle_key == "none" ? "None" : "$(cycle_key)s"
        println(
            "  TabularData: $(length(channel_pairs)) channels, $(length(data_sets)) rows (updateCycle=$cycle_str)",
        )

        push!(tabular_data_list, TabularData(data_channel_ids, data_sets))
    end
    println()
end

let
    println("6. Package: Create TimeSeriesDataPackage with header")
    println("-----------------------------------------------------------")

    valid_readings = Dict{String,Vector{SensorReading}}()
    for reading in READINGS
        dcl_ref = data_channel_list(DCL_PACKAGE)
        if from_short_id(dcl_ref, reading.system_id) !== nothing
            push!(get!(valid_readings, reading.system_id, SensorReading[]), reading)
        end
    end

    channels_by_cycle = Dict{String,Vector{Tuple{String,String}}}()
    for system_id in keys(valid_readings)
        dcl_ref = data_channel_list(DCL_PACKAGE)
        dc = from_short_id(dcl_ref, system_id)
        if dc !== nothing
            cycle = update_cycle(data_channel_type(property(dc)))
            cycle_key = cycle !== nothing ? format_double(cycle) : "none"
            local_id_str = local_id_string(channel_id(dc))
            local_id_str = local_id_str !== nothing ? local_id_str : ""
            push!(
                get!(channels_by_cycle, cycle_key, Tuple{String,String}[]),
                (local_id_str, system_id),
            )
        end
    end

    tabular_data_list = TabularData[]
    for (_, channel_pairs) in channels_by_cycle
        all_timestamps = DateTimeOffset[]
        for (_, system_id) in channel_pairs
            for r in get(valid_readings, system_id, SensorReading[])
                if !any(t -> ticks(t) == ticks(r.timestamp), all_timestamps)
                    push!(all_timestamps, r.timestamp)
                end
            end
        end
        sort!(all_timestamps; by = ticks)

        data_channel_ids = TsdChannelId[]
        for (local_id_str, _) in channel_pairs
            id = from_string(TsdChannelId, local_id_str)
            id !== nothing && push!(data_channel_ids, id)
        end

        data_sets = TabularDataSet[]
        for ts in all_timestamps
            vals = String[]
            for (_, system_id) in channel_pairs
                rs = get(valid_readings, system_id, SensorReading[])
                idx = findfirst(r -> ticks(r.timestamp) == ticks(ts), rs)
                push!(vals, idx !== nothing ? format_double(rs[idx].value) : "")
            end
            push!(data_sets, TabularDataSet(ts, vals))
        end

        push!(tabular_data_list, TabularData(data_channel_ids, data_sets))
    end

    all_timestamps_list = DateTimeOffset[]
    for rs in Base.values(valid_readings)
        for r in rs
            push!(all_timestamps_list, r.timestamp)
        end
    end

    if isempty(all_timestamps_list)
        println("No valid readings to build a TimeSeriesData package from.")
    else
        min_ts = all_timestamps_list[argmin(ticks.(all_timestamps_list))]
        max_ts = all_timestamps_list[argmax(ticks.(all_timestamps_list))]

        ship_id = from_imo_number(parse(ImoNumber, "1234567"))

        dcl_id = data_channel_list_id(header(package(DCL_PACKAGE)))
        configuration = TsdConfigRef(id(dcl_id), timestamp(dcl_id))

        time_span = TsdTimeSpan(min_ts, max_ts)
        ts_header = TsdHeader(ship_id)
        set_time_span!(ts_header, time_span)
        set_author!(ts_header, "Vista SDK Sample")

        ts_data = TimeSeriesData()
        set_data_configuration!(ts_data, configuration)
        set_tabular_data!(ts_data, tabular_data_list)

        tsd_pkg = TsdPackage(ts_header, [ts_data])
        tsd_package = TimeSeriesDataPackage(tsd_pkg)

        println("Created TimeSeriesDataPackage:")
        println("  Ship ID  : IMO1234567")
        println("  TimeSpan : $min_ts to $max_ts")
        println("  TabularData blocks: $(length(tabular_data_list))")
        println()
    end
end

let
    println("7. Serialization: Convert to ISO19848 JSON format")
    println("--------------------------------------------------------")

    valid_readings = Dict{String,Vector{SensorReading}}()
    for reading in READINGS
        dcl_ref = data_channel_list(DCL_PACKAGE)
        if from_short_id(dcl_ref, reading.system_id) !== nothing
            push!(get!(valid_readings, reading.system_id, SensorReading[]), reading)
        end
    end

    dcl_ref = data_channel_list(DCL_PACKAGE)
    dc1 = from_short_id(dcl_ref, "TEMP001")
    local_id_str = local_id_string(channel_id(dc1))
    channel_id1 = from_string(TsdChannelId, local_id_str)

    ds1 = TabularDataSet(BASE_TIME, ["45.2"])
    ds2 = TabularDataSet(BASE_PLUS_5, ["46.1"])
    td = TabularData([channel_id1], [ds1, ds2])

    dcl_id = data_channel_list_id(header(package(DCL_PACKAGE)))
    configuration = TsdConfigRef(id(dcl_id), timestamp(dcl_id))

    ship_id = from_imo_number(parse(ImoNumber, "1234567"))
    ts_header = TsdHeader(ship_id)
    set_time_span!(ts_header, TsdTimeSpan(BASE_TIME, BASE_PLUS_5))
    set_author!(ts_header, "Vista SDK Sample")

    ts_data = TimeSeriesData()
    set_data_configuration!(ts_data, configuration)
    set_tabular_data!(ts_data, [td])

    tsd_pkg = TsdPackage(ts_header, [ts_data])
    tsd_package = TimeSeriesDataPackage(tsd_pkg)

    json_out = tsd_to_json(tsd_package, true)
    println("Serialized to JSON: $(length(json_out)) bytes")
    println("Package:\n$json_out")
    println()
end

let
    println("8. Validation: Cross-check TimeSeriesData against DataChannelList")
    println("-------------------------------------------------------------------------")

    dcl_ref = data_channel_list(DCL_PACKAGE)
    dc1 = from_short_id(dcl_ref, "TEMP001")
    local_id_str = local_id_string(channel_id(dc1))
    channel_id1 = from_string(TsdChannelId, local_id_str)

    ds = TabularDataSet(BASE_TIME, ["45.2"])
    td = TabularData([channel_id1], [ds])

    dcl_id = data_channel_list_id(header(package(DCL_PACKAGE)))
    configuration = TsdConfigRef(id(dcl_id), timestamp(dcl_id))

    ts_data = TimeSeriesData()
    set_data_configuration!(ts_data, configuration)
    set_tabular_data!(ts_data, [td])

    validation = validate(
        ts_data,
        DCL_PACKAGE,
        (_ts, _dc, _val, _qual) -> true,
        (_ts, _dc, _val, _qual) -> true,
    )

    println("  Is valid: $(is_valid(validation))")
    if !is_valid(validation)
        println("  Errors:")
        for err in errors(validation)
            println("    - $err")
        end
    end
    println()
end

let
    println("9. Validation failure: Out-of-range sensor value rejected")
    println("---------------------------------------------------------------")

    dcl_ref = data_channel_list(DCL_PACKAGE)
    temp001_dc = from_short_id(dcl_ref, "TEMP001")
    temp001_local_id = local_id_string(channel_id(temp001_dc))

    faulty_channel_id = from_string(TsdChannelId, temp001_local_id)

    dcl_id = data_channel_list_id(header(package(DCL_PACKAGE)))
    configuration = TsdConfigRef(id(dcl_id), timestamp(dcl_id))

    faulty_ds = TabularDataSet(BASE_TIME, ["999.9"])
    set_quality!(faulty_ds, ["Good"])
    faulty_tabular = TabularData([faulty_channel_id], [faulty_ds])

    faulty_ts_data = TimeSeriesData()
    set_data_configuration!(faulty_ts_data, configuration)
    set_tabular_data!(faulty_ts_data, [faulty_tabular])

    println("  Reporting TEMP001 = 999.9 degC (Restriction.MaxInclusive is 200)")

    faulty_validation = validate(
        faulty_ts_data,
        DCL_PACKAGE,
        (_ts, _dc, _val, _qual) -> true,
        (_ts, _dc, _val, _qual) -> true,
    )
    println("  Is valid: $(is_valid(faulty_validation))")
    if !is_valid(faulty_validation)
        println("  Errors:")
        for err in errors(faulty_validation)
            println("    - $err")
        end
    end
    println()
end
