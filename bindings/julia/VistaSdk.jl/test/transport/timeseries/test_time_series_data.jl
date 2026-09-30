using Test
using VistaSdk

const TSD_LOCAL_ID = "/dnv-v2/vis-3-4a/411.1-1/C101.63/S206/meta/qty-temperature/cnt-cooling.air"
const TSD_LOCAL_ID2 = "/dnv-v2/vis-3-4a/511.15-1/E32/meta/qty-power"

function _tsd_make_dcl()
    ts = Base.parse(DateTimeOffset, "2016-01-01T00:00:00Z")
    dcl_cfg = DclConfigurationReference("DataChannelList.xml", ts)
    ver = DclVersionInformation("some_naming_rule", "2.0")
    set_reference_url!(ver, "http://somewhere.net")
    ship = from_string(ShipId, "IMO1234567")
    hdr = DclHeader(ship, dcl_cfg)
    set_version_information!(hdr, ver)
    set_author!(hdr, "Author1")
    set_date_created!(hdr, Base.parse(DateTimeOffset, "2015-12-01T00:00:00Z"))

    dcl = DclDataChannelList()

    let lb = from_string(
            LocalIdBuilder,
            "/dnv-v2/vis-3-4a/411.1-1/C101.63/S206/meta/qty-temperature/cnt-cooling.air",
        )
        no = DclNameObject()
        set_naming_rule!(no, "Naming_Rule")
        cid = DclDataChannelId(lb)
        set_short_id!(cid, "0010")
        set_name_object!(cid, no)
        dct = DclDataChannelType("Inst")
        set_update_cycle!(dct, 1.0)
        restr = DclRestriction()
        set_fraction_digits!(restr, 1)
        set_max_inclusive!(restr, 200.0)
        set_min_inclusive!(restr, -150.0)
        fmt = DclFormat("Decimal")
        set_restriction!(fmt, restr)
        rng = DclRange(0.0, 150.0)
        u = DclUnit("°C")
        set_quantity_name!(u, "Temperature")
        prop = DclProperty(dct, fmt)
        set_range!(prop, rng)
        set_unit!(prop, u)
        set_quality_coding!(prop, "OPC_QUALITY")
        set_name!(prop, "M/E #1 Air Cooler CFW OUT Temp")
        add!(dcl, DclDataChannel(cid, prop))
    end

    let lb = from_string(LocalIdBuilder, "/dnv-v2/vis-3-4a/511.15-1/E32/meta/qty-power")
        cid = DclDataChannelId(lb)
        set_short_id!(cid, "0020")
        dct = DclDataChannelType("Alert")
        restr = DclRestriction()
        set_max_length!(restr, 100)
        set_min_length!(restr, 0)
        fmt = DclFormat("String")
        set_restriction!(fmt, restr)
        prop = DclProperty(dct, fmt)
        set_alert_priority!(prop, "Warning")
        add!(dcl, DclDataChannel(cid, prop))
    end

    pkg = DclPackage(hdr, dcl)
    DclListPackage(pkg)
end

function _tsd_short_ids(dc_pkg::DclListPackage)
    lst = data_channel_list(dc_pkg)
    lst === nothing && return TsdChannelId[]
    n = length(lst)
    ids = TsdChannelId[]
    for i = 1:n
        dc = lst[i]
        cid = channel_id(dc)
        s = short_id(cid)
        s === nothing && continue
        id = from_string(TsdChannelId, s)
        id !== nothing && push!(ids, id)
    end
    ids
end

@testset "TimeSeriesData" begin

    @testset "time_series_data_package_is_non_empty" begin
        dc_pkg = _tsd_make_dcl()
        all_ids = _tsd_short_ids(dc_pkg)

        ts1 = Base.parse(DateTimeOffset, "2016-01-01T12:00:00Z")
        ts2 = Base.parse(DateTimeOffset, "2016-01-02T12:00:00Z")
        ts3 = Base.parse(DateTimeOffset, "2016-01-03T12:00:00Z")
        event_ts = Base.parse(DateTimeOffset, "2016-01-01T12:00:01Z")
        start_ = Base.parse(DateTimeOffset, "2016-01-01T12:00:00Z")
        end_ = Base.parse(DateTimeOffset, "2016-01-03T12:00:00Z")
        created_modified = Base.parse(DateTimeOffset, "2016-01-03T12:00:00Z")
        config_ts = Base.parse(DateTimeOffset, "2016-01-03T00:00:00Z")

        ds1_0 = TabularDataSet(ts1, ["100.0", "200.0"])
        set_quality!(ds1_0, ["0", "0"])
        ds1_1 = TabularDataSet(ts2, ["105.0", "210.0"])
        set_quality!(ds1_1, ["0", "0"])
        tabular1 = TabularData(all_ids[1:2], [ds1_0, ds1_1])

        ds2_0 = TabularDataSet(ts1, ["100.0"])
        ds2_1 = TabularDataSet(ts2, ["100.1"])
        ds2_2 = TabularDataSet(ts3, ["100.2"])
        tabular2 = TabularData(all_ids[1:1], [ds2_0, ds2_1, ds2_2])

        eds = EventDataSet(event_ts, all_ids[2], "HIGH")
        set_quality!(eds, "0")
        event_data = EventData()
        set_data_sets!(event_data, [eds])

        data_config = TsdConfigRef(
            "DataChannelList.xml",
            Base.parse(DateTimeOffset, "2016-01-01T00:00:00Z"),
        )

        sys_cfg_0 = TsdConfigRef("SystemConfiguration.xml", config_ts)
        sys_cfg_1 = TsdConfigRef("SystemConfiguration.xml", config_ts)

        custom1 = sd_object()
        set!(custom1, "dataQuality", SerializableDocument("high"))

        tsd1 = TimeSeriesData()
        set_data_configuration!(tsd1, data_config)
        set_tabular_data!(tsd1, [tabular1, tabular2])
        set_event_data!(tsd1, event_data)
        set_custom_data_kinds!(tsd1, custom1)

        custom2 = sd_object()
        set!(custom2, "source", SerializableDocument("sensor_array_1"))

        tsd2 = TimeSeriesData()
        set_data_configuration!(tsd2, data_config)
        set_tabular_data!(tsd2, [tabular1, tabular2])
        set_event_data!(tsd2, event_data)
        set_custom_data_kinds!(tsd2, custom2)

        ship = from_string(ShipId, "IMO1234567")
        ts = TsdTimeSpan(start_, end_)
        hdr = TsdHeader(ship)
        set_time_span!(hdr, ts)
        set_date_created!(hdr, created_modified)
        set_date_modified!(hdr, created_modified)
        set_author!(hdr, "Shipboard data server")
        set_system_configuration!(hdr, [sys_cfg_0, sys_cfg_1])
        custom_hdr = sd_object()
        set!(custom_hdr, "temperatureUnit", SerializableDocument("Celsius"))
        set_custom_headers!(hdr, custom_hdr)

        pkg = TsdPackage(hdr, [tsd1, tsd2])
        p = TimeSeriesDataPackage(pkg)
        @test !is_empty(p)
    end

    @testset "time_series_data_structure_validation" begin
        dc_pkg = _tsd_make_dcl()
        all_ids = _tsd_short_ids(dc_pkg)

        ts1 = Base.parse(DateTimeOffset, "2016-01-01T12:00:00Z")
        ts2 = Base.parse(DateTimeOffset, "2016-01-02T12:00:00Z")
        ts3 = Base.parse(DateTimeOffset, "2016-01-03T12:00:00Z")

        ds1_0 = TabularDataSet(ts1, ["100.0", "200.0"])
        set_quality!(ds1_0, ["0", "0"])
        ds1_1 = TabularDataSet(ts2, ["105.0", "210.0"])
        set_quality!(ds1_1, ["0", "0"])
        tabular1 = TabularData(all_ids[1:2], [ds1_0, ds1_1])
        @test channel_id_count(tabular1) != 0
        @test data_set_count(tabular1) != 0

        ds2_0 = TabularDataSet(ts1, ["100.0"])
        ds2_1 = TabularDataSet(ts2, ["100.1"])
        ds2_2 = TabularDataSet(ts3, ["100.2"])
        tabular2 = TabularData(all_ids[1:1], [ds2_0, ds2_1, ds2_2])
        @test channel_id_count(tabular2) != 0
        @test data_set_count(tabular2) != 0

        event_ts = Base.parse(DateTimeOffset, "2016-01-01T12:00:01Z")
        eds = EventDataSet(event_ts, all_ids[2], "HIGH")
        set_quality!(eds, "0")
        ed = EventData()
        set_data_sets!(ed, [eds])
        @test data_set_count(ed) > 0
    end

    @testset "time_span_valid_construction_and_setters" begin
        start_ = Base.parse(DateTimeOffset, "2016-01-01T12:00:00Z")
        end_ = Base.parse(DateTimeOffset, "2016-01-03T12:00:00Z")

        ts = TsdTimeSpan(start_, end_)
        @test string(start_time(ts)) == string(start_)
        @test string(end_time(ts)) == string(end_)

        new_start = Base.parse(DateTimeOffset, "2016-01-02T12:00:00Z")
        set_start!(ts, new_start)
        @test string(start_time(ts)) == string(new_start)

        new_end = Base.parse(DateTimeOffset, "2016-01-04T12:00:00Z")
        set_end!(ts, new_end)
        @test string(end_time(ts)) == string(new_end)
    end

    @testset "time_span_invalid_range_returns_err" begin
        start_ = Base.parse(DateTimeOffset, "2016-01-03T12:00:00Z")
        end_ = Base.parse(DateTimeOffset, "2016-01-01T12:00:00Z")
        @test_throws VistaError TsdTimeSpan(start_, end_)
    end

    @testset "tabular_data_valid_validation" begin
        id1 = from_string(TsdChannelId, "0010")
        id2 = from_string(TsdChannelId, "0020")
        ts = Base.parse(DateTimeOffset, "2016-01-01T12:00:00Z")
        ds = TabularDataSet(ts, ["100.0", "200.0"])
        set_quality!(ds, ["0", "0"])
        tabular = TabularData([id1, id2], [ds])
        @test validate(tabular)
    end

    @testset "tabular_data_dimension_mismatch_fails_validation" begin
        id1 = from_string(TsdChannelId, "0010")
        id2 = from_string(TsdChannelId, "0020")
        ts = Base.parse(DateTimeOffset, "2016-01-01T12:00:00Z")
        ds = TabularDataSet(ts, ["100.0", "200.0", "300.0"])
        set_quality!(ds, ["0", "0", "0"])
        tabular = TabularData([id1, id2], [ds])
        @test !validate(tabular)
    end

    @testset "event_data_operations" begin
        ed = EventData()
        @test data_set_count(ed) == 0

        ts = Base.parse(DateTimeOffset, "2016-01-01T12:00:01Z")
        id = from_string(TsdChannelId, "0010")
        eds1 = EventDataSet(ts, id, "HIGH")
        set_quality!(eds1, "0")
        set_data_sets!(ed, [eds1])
        @test data_set_count(ed) == 1

        eds2 = EventDataSet(ts, id, "LOW")
        set_quality!(eds2, "0")
        set_data_sets!(ed, [eds1, eds2])
        @test data_set_count(ed) == 2

        clear_data_sets!(ed)
        @test data_set_count(ed) == 0
    end

    @testset "config_ref_construction_and_setters" begin
        ts = Base.parse(DateTimeOffset, "2016-01-01T00:00:00Z")
        cfg = TsdConfigRef("TestConfig.xml", ts)
        @test config_id(cfg) == "TestConfig.xml"
        @test string(time_stamp(cfg)) == string(ts)

        set_config_id!(cfg, "NewConfig.xml")
        @test config_id(cfg) == "NewConfig.xml"

        new_ts = Base.parse(DateTimeOffset, "2016-01-02T00:00:00Z")
        set_time_stamp!(cfg, new_ts)
        @test string(time_stamp(cfg)) == string(new_ts)
    end

    @testset "tabular_data_set_construction_with_and_without_quality" begin
        ts = Base.parse(DateTimeOffset, "2016-01-01T12:00:00Z")
        ds = TabularDataSet(ts, ["100.0", "200.0"])
        set_quality!(ds, ["0", "0"])
        @test string(time_stamp(ds)) == string(ts)
        vals = VistaSdk.values(ds)
        @test length(vals) == 2
        @test vals[1] == "100.0"
        @test vals[2] == "200.0"
        q = quality(ds)
        @test q !== nothing
        @test length(q) == 2
        @test q[1] == "0"

        ds_nq = TabularDataSet(ts, ["100.0", "200.0"])
        @test quality(ds_nq) === nothing
    end

    @testset "event_data_set_construction_with_and_without_quality" begin
        ts = Base.parse(DateTimeOffset, "2016-01-01T12:00:01Z")
        id = from_string(TsdChannelId, "0010")

        eds = EventDataSet(ts, id, "HIGH")
        set_quality!(eds, "0")
        @test string(time_stamp(eds)) == string(ts)
        @test value(eds) == "HIGH"
        @test quality(eds) == "0"

        eds_nq = EventDataSet(ts, id, "LOW")
        @test quality(eds_nq) === nothing
    end

    @testset "package_empty_time_series_data_is_allowed" begin
        ship = from_string(ShipId, "IMO1234567")
        hdr = TsdHeader(ship)
        pkg = TsdPackage(hdr, TimeSeriesData[])
        @test time_series_data_is_empty(pkg)
        @test has_header(pkg)
    end

    @testset "valid_time_series_data_against_data_channel_list" begin
        dc_pkg = _tsd_make_dcl()
        all_ids = _tsd_short_ids(dc_pkg)

        ts1 = Base.parse(DateTimeOffset, "2016-01-01T12:00:00Z")
        ts2 = Base.parse(DateTimeOffset, "2016-01-02T12:00:00Z")
        ds1 = TabularDataSet(ts1, ["100.0", "200.0"])
        set_quality!(ds1, ["0", "0"])
        ds2 = TabularDataSet(ts2, ["105.0", "210.0"])
        set_quality!(ds2, ["0", "0"])
        tabular = TabularData(all_ids[1:2], [ds1, ds2])

        data_config = TsdConfigRef(
            "DataChannelList.xml",
            Base.parse(DateTimeOffset, "2016-01-01T00:00:00Z"),
        )
        tsd = TimeSeriesData()
        set_data_configuration!(tsd, data_config)
        set_tabular_data!(tsd, [tabular])

        ok_cb = (_, _, _, _) -> true
        result = validate(tsd, dc_pkg, ok_cb, ok_cb)
        @test is_valid(result)
    end

    @testset "invalid_data_channel_id_not_found_in_dcl" begin
        dc_pkg = _tsd_make_dcl()

        ts = Base.parse(DateTimeOffset, "2016-01-01T12:00:00Z")
        invalid_id = from_string(TsdChannelId, "9999")
        ds = TabularDataSet(ts, ["100.0"])
        tabular = TabularData([invalid_id], [ds])

        data_config = TsdConfigRef(
            "DataChannelList.xml",
            Base.parse(DateTimeOffset, "2016-01-01T00:00:00Z"),
        )
        tsd = TimeSeriesData()
        set_data_configuration!(tsd, data_config)
        set_tabular_data!(tsd, [tabular])

        ok_cb = (_, _, _, _) -> true
        result = validate(tsd, dc_pkg, ok_cb, ok_cb)
        @test !is_valid(result)
        @test !isempty(errors(result))
        @test occursin("not found", errors(result)[1])
    end

    @testset "invalid_value_format_mismatch" begin
        dc_pkg = _tsd_make_dcl()
        all_ids = _tsd_short_ids(dc_pkg)

        ts = Base.parse(DateTimeOffset, "2016-01-01T12:00:00Z")
        ds = TabularDataSet(ts, ["invalid_not_a_number"])
        tabular = TabularData(all_ids[1:1], [ds])

        data_config = TsdConfigRef(
            "DataChannelList.xml",
            Base.parse(DateTimeOffset, "2016-01-01T00:00:00Z"),
        )
        tsd = TimeSeriesData()
        set_data_configuration!(tsd, data_config)
        set_tabular_data!(tsd, [tabular])

        ok_cb = (_, _, _, _) -> true
        result = validate(tsd, dc_pkg, ok_cb, ok_cb)
        @test !is_valid(result)
        @test !isempty(errors(result))
        @test occursin("invalid value", errors(result)[1])
    end

    @testset "invalid_data_config_id_mismatch" begin
        dc_pkg = _tsd_make_dcl()
        all_ids = _tsd_short_ids(dc_pkg)

        ts1 = Base.parse(DateTimeOffset, "2016-01-01T12:00:00Z")
        ds = TabularDataSet(ts1, ["100.0", "200.0"])
        set_quality!(ds, ["0", "0"])
        tabular = TabularData(all_ids[1:2], [ds])

        wrong_config = TsdConfigRef(
            "WrongDataChannelList.xml",
            Base.parse(DateTimeOffset, "2016-01-01T00:00:00Z"),
        )
        tsd = TimeSeriesData()
        set_data_configuration!(tsd, wrong_config)
        set_tabular_data!(tsd, [tabular])

        ok_cb = (_, _, _, _) -> true
        result = validate(tsd, dc_pkg, ok_cb, ok_cb)
        @test !is_valid(result)
        @test !isempty(errors(result))
        @test occursin("does not match", errors(result)[1])
    end

    @testset "invalid_empty_time_series_data" begin
        dc_pkg = _tsd_make_dcl()

        data_config = TsdConfigRef(
            "DataChannelList.xml",
            Base.parse(DateTimeOffset, "2016-01-01T00:00:00Z"),
        )
        tsd = TimeSeriesData()
        set_data_configuration!(tsd, data_config)

        ok_cb = (_, _, _, _) -> true
        result = validate(tsd, dc_pkg, ok_cb, ok_cb)
        @test !is_valid(result)
        @test !isempty(errors(result))
        @test occursin("without data", errors(result)[1])
    end

    @testset "custom_callback_rejection" begin
        dc_pkg = _tsd_make_dcl()
        all_ids = _tsd_short_ids(dc_pkg)

        ts1 = Base.parse(DateTimeOffset, "2016-01-01T12:00:00Z")
        ts2 = Base.parse(DateTimeOffset, "2016-01-02T12:00:00Z")
        ds1 = TabularDataSet(ts1, ["100.0", "200.0"])
        set_quality!(ds1, ["0", "0"])
        ds2 = TabularDataSet(ts2, ["105.0", "210.0"])
        set_quality!(ds2, ["0", "0"])
        tabular = TabularData(all_ids[1:2], [ds1, ds2])

        data_config = TsdConfigRef(
            "DataChannelList.xml",
            Base.parse(DateTimeOffset, "2016-01-01T00:00:00Z"),
        )
        tsd = TimeSeriesData()
        set_data_configuration!(tsd, data_config)
        set_tabular_data!(tsd, [tabular])

        reject_cb = (_, _, _, _) -> ["Custom business rule violation"]
        ok_cb = (_, _, _, _) -> true
        result = validate(tsd, dc_pkg, reject_cb, ok_cb)
        @test !is_valid(result)
        @test !isempty(errors(result))
        @test occursin("Custom business rule violation", errors(result)[1])
    end

    @testset "custom_callback_multi_message_rejection" begin
        dc_pkg = _tsd_make_dcl()
        all_ids = _tsd_short_ids(dc_pkg)

        ts1 = Base.parse(DateTimeOffset, "2016-01-01T12:00:00Z")
        ds1 = TabularDataSet(ts1, ["100.0"])
        set_quality!(ds1, ["0"])
        tabular = TabularData(all_ids[1:1], [ds1])

        data_config = TsdConfigRef(
            "DataChannelList.xml",
            Base.parse(DateTimeOffset, "2016-01-01T00:00:00Z"),
        )
        tsd = TimeSeriesData()
        set_data_configuration!(tsd, data_config)
        set_tabular_data!(tsd, [tabular])

        reject_cb =
            (_, _, _, _) ->
                ["first business rule violation", "second business rule violation"]
        ok_cb = (_, _, _, _) -> true
        result = validate(tsd, dc_pkg, reject_cb, ok_cb)
        @test !is_valid(result)
        @test errors(result) ==
              ["first business rule violation", "second business rule violation"]
    end

    @testset "tsd_package_with_header" begin
        ship = from_string(ShipId, "IMO1234567")
        hdr = TsdHeader(ship)
        pkg = TsdPackage(nothing, TimeSeriesData[])
        @test !has_header(pkg)
        set_header!(pkg, hdr)
        @test has_header(pkg)
    end

    @testset "tsd_header_date_created_and_modified" begin
        ship = from_string(ShipId, "IMO1234567")
        ts = utc_now(DateTimeOffset)
        h = TsdHeader(ship)
        set_date_created!(h, ts)
        set_date_modified!(h, ts)
        @test date_created(h) !== nothing
        @test date_modified(h) !== nothing
        clear_date_created!(h)
        clear_date_modified!(h)
        @test date_created(h) === nothing
        @test date_modified(h) === nothing
    end

    @testset "tsd_header_without_author" begin
        ship = from_string(ShipId, "IMO1234567")
        h = TsdHeader(ship)
        set_author!(h, "Alice")
        @test author(h) == "Alice"
        clear_author!(h)
        @test author(h) === nothing
    end

    @testset "tsd_header_system_configuration" begin
        ship = from_string(ShipId, "IMO1234567")
        ts = Base.parse(DateTimeOffset, "2024-01-01T00:00:00Z")
        cfg = TsdConfigRef("cfg-1", ts)
        h = TsdHeader(ship)
        set_system_configuration!(h, [cfg])
        @test system_configuration_count(h) == 1
        @test system_configuration_at(h, 1) !== nothing
        @test system_configuration_at(h, 2) === nothing
        clear_system_configuration!(h)
        @test system_configuration_count(h) == 0
    end

    @testset "tsd_header_custom_headers" begin
        ship = from_string(ShipId, "IMO1234567")
        h = TsdHeader(ship)
        doc = sd_object()
        set_custom_headers!(h, doc)
        @test custom_headers(h) !== nothing
        clear_custom_headers!(h)
        @test custom_headers(h) === nothing
    end

    @testset "time_series_data_without_tabular_and_data_configuration" begin
        ts = Base.parse(DateTimeOffset, "2024-01-01T00:00:00Z")
        cfg = TsdConfigRef("cfg-1", ts)
        ch = from_string(TsdChannelId, TSD_LOCAL_ID)
        ds = TabularDataSet(utc_now(DateTimeOffset), ["1.0"])
        td = TabularData([ch], [ds])
        tsd = TimeSeriesData()
        set_data_configuration!(tsd, cfg)
        set_tabular_data!(tsd, [td])
        @test data_configuration(tsd) !== nothing
        @test tabular_data_count(tsd) == 1
        clear_data_configuration!(tsd)
        clear_tabular_data!(tsd)
        @test data_configuration(tsd) === nothing
        @test tabular_data_count(tsd) == 0
    end

    @testset "time_series_data_with_and_without_event_data" begin
        ch = from_string(TsdChannelId, TSD_LOCAL_ID)
        ts = utc_now(DateTimeOffset)
        eds = EventDataSet(ts, ch, "42.0")
        ed = EventData()
        set_data_sets!(ed, [eds])
        tsd = TimeSeriesData()
        set_event_data!(tsd, ed)
        @test event_data(tsd) !== nothing
        clear_event_data!(tsd)
        @test event_data(tsd) === nothing
    end

    @testset "tabular_data_accessors" begin
        ch = from_string(TsdChannelId, TSD_LOCAL_ID)
        ds = TabularDataSet(utc_now(DateTimeOffset), ["1.0"])
        td = TabularData([ch], [ds])
        @test channel_id_count(td) == 1
        @test data_set_count(td) == 1
        @test channel_id_at(td, 1) !== nothing
        @test channel_id_at(td, 2) === nothing
        @test data_set_at(td, 1) !== nothing
        @test data_set_at(td, 2) === nothing
        @test validate(td)
    end

    @testset "tabular_data_set_values_and_quality" begin
        ts = utc_now(DateTimeOffset)
        ds = TabularDataSet(ts, ["1.0", "2.0"])
        set_quality!(ds, ["Good", "Bad"])
        @test ticks(time_stamp(ds)) == ticks(ts)
        vals = VistaSdk.values(ds)
        @test length(vals) == 2
        @test vals[1] == "1.0"
        q = quality(ds)
        @test q !== nothing
        @test q[1] == "Good"
        clear_quality!(ds)
        @test quality(ds) === nothing
    end

    @testset "event_data_set_accessors" begin
        ch = from_string(TsdChannelId, TSD_LOCAL_ID)
        ts = utc_now(DateTimeOffset)
        eds = EventDataSet(ts, ch, "42.0")
        set_quality!(eds, "Good")
        @test ticks(time_stamp(eds)) == ticks(ts)
        @test value(eds) == "42.0"
        @test quality(eds) == "Good"
        clear_quality!(eds)
        @test quality(eds) === nothing
    end

    @testset "event_data_accessors" begin
        ch = from_string(TsdChannelId, TSD_LOCAL_ID)
        ts = utc_now(DateTimeOffset)
        eds = EventDataSet(ts, ch, "1.0")
        ed = EventData()
        set_data_sets!(ed, [eds])
        @test data_set_count(ed) == 1
        @test data_set_at(ed, 1) !== nothing
        @test data_set_at(ed, 2) === nothing
        clear_data_sets!(ed)
        @test data_set_count(ed) == 0
    end

    @testset "time_series_data_package_is_empty" begin
        pkg = TsdPackage(nothing, TimeSeriesData[])
        p = TimeSeriesDataPackage(pkg)
        @test is_empty(p)
        @test time_series_data_count(p) == 0
    end

    @testset "time_series_data_with_custom_data_kinds" begin
        doc = sd_object()
        tsd = TimeSeriesData()
        set_custom_data_kinds!(tsd, doc)
        @test custom_data_kinds(tsd) !== nothing
        clear_custom_data_kinds!(tsd)
        @test custom_data_kinds(tsd) === nothing
    end

end
