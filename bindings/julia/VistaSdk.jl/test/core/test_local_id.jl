using Test
using VistaSdk

@testset "LocalId" begin
    v = vis()
    g = gmod(v, V3_4a)
    locs = locations(v, V3_4a)
    cbs = codebooks(v, V3_4a)

    @testset "build with primary item and metadata tag" begin
        primary = from_short_path("411.1/C101.31-2", g, locs)
        @test primary !== nothing
        qty_tag = create_tag(cbs[VistaSdk.Quantity], "temperature")
        @test qty_tag !== nothing
        b = with_metadata_tag(
            with_primary_item(create(LocalIdBuilder, V3_4a), primary),
            qty_tag,
        )
        id = build(b)
        @test string(id) == "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature"
        @test version(id) == V3_4a
        @test value(quantity(id)) == "temperature"
    end

    @testset "build without primary item throws" begin
        b = create(LocalIdBuilder, V3_4a)
        @test_throws VistaError build(b)
    end

    @testset "from_string round-trips" begin
        s = "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature"
        id = from_string(LocalId, s)
        @test id !== nothing
        @test string(id) == s
    end

    @testset "from_string invalid returns nothing" begin
        @test from_string(LocalId, "not a local id") === nothing
    end

    @testset "from_string_with_errors reports errors on failure" begin
        id, errors = from_string_with_errors(LocalId, "")
        @test id === nothing
        @test has_errors(errors)
    end

    @testset "from_string_with_errors succeeds without errors" begin
        s = "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature"
        id, errors = from_string_with_errors(LocalId, s)
        @test id !== nothing
        @test !has_errors(errors)
    end

    @testset "secondary_item none when not set" begin
        id = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature")
        @test id !== nothing
        @test secondary_item(id) === nothing
    end

    @testset "secondary_item some when set" begin
        id = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/621.21/S90/sec/411.1/C101/meta/qty-mass/cnt-fuel.oil/pos-inlet",
        )
        @test id !== nothing
        @test secondary_item(id) !== nothing
    end

    @testset "primary_item GmodPathRef accessors" begin
        id = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature")
        @test id !== nothing
        p = primary_item(id)
        @test p isa GmodPathRef
        @test string(p) == "411.1/C101.31-2"
        @test version(p) == V3_4a
        @test length(p) >= 1
        @test node(p) isa GmodNodeRef
    end

    @testset "secondary_item GmodPathRef accessors" begin
        id = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/621.21/S90/sec/411.1/C101/meta/qty-mass/cnt-fuel.oil/pos-inlet",
        )
        @test id !== nothing
        sec = secondary_item(id)
        @test sec isa GmodPathRef
        @test string(sec) == "411.1/C101"
        @test length(sec) >= 1
    end

    @testset "metadata_tags returns all set tags" begin
        id = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/621.21/S90/sec/411.1/C101/meta/qty-mass/cnt-fuel.oil/pos-inlet",
        )
        @test id !== nothing
        @test length(metadata_tags(id)) == 3
    end

    @testset "builder modify via without/with metadata tag" begin
        original =
            from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature")
        @test original !== nothing
        pos_tag = create_tag(cbs[VistaSdk.Position], "outlet")
        @test pos_tag !== nothing
        modified = build(
            with_metadata_tag(
                without_metadata_tag(builder(original), VistaSdk.Quantity),
                pos_tag,
            ),
        )
        @test string(modified) == "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/pos-outlet"
    end

    @testset "equality" begin
        id1 = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature")
        id2 = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature")
        id3 = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-pressure")
        @test id1 !== nothing && id2 !== nothing && id3 !== nothing
        @test id1 == id2
        @test id1 != id3
    end

    @testset "custom metadata tag flagged as custom" begin
        primary = from_short_path("411.1/C101.31-2", g, locs)
        @test primary !== nothing
        custom_tag = create_tag(cbs[VistaSdk.Quantity], "my_custom_measurement")
        @test custom_tag !== nothing
        id = build(
            with_metadata_tag(
                with_primary_item(create(LocalIdBuilder, V3_4a), primary),
                custom_tag,
            ),
        )
        @test has_custom_tag(id)
        @test is_custom(quantity(id))
    end

    @testset "builder is_valid and is_empty reflect state" begin
        primary = from_short_path("411.1/C101.31-2", g, locs)
        @test primary !== nothing
        qty_tag = create_tag(cbs[VistaSdk.Quantity], "temperature")
        @test qty_tag !== nothing

        empty_b = create(LocalIdBuilder, V3_4a)
        @test !is_valid(empty_b)
        @test is_empty(empty_b)

        incomplete_b = with_primary_item(create(LocalIdBuilder, V3_4a), primary)
        @test !is_valid(incomplete_b)
        @test !is_empty(incomplete_b)

        valid_b = with_metadata_tag(
            with_primary_item(create(LocalIdBuilder, V3_4a), primary),
            qty_tag,
        )
        @test is_valid(valid_b)
        @test !is_empty(valid_b)
    end

    @testset "builder version reflects vis version" begin
        b = create(LocalIdBuilder, V3_4a)
        @test version(b) == V3_4a
    end

    @testset "builder without_vis_version clears version" begin
        b = without_vis_version(create(LocalIdBuilder, V3_4a))
        @test version(b) === nothing
    end

    @testset "builder without_primary_item clears it" begin
        primary = from_short_path("411.1/C101.31-2", g, locs)
        @test primary !== nothing
        b = without_primary_item(with_primary_item(create(LocalIdBuilder, V3_4a), primary))
        @test primary_item(b) === nothing
    end

    @testset "builder with and without secondary item" begin
        primary = from_short_path("411.1/C101.31-2", g, locs)
        secondary = from_short_path("411.1/C101.31-2", g, locs)
        @test primary !== nothing && secondary !== nothing
        b = with_secondary_item(
            with_primary_item(create(LocalIdBuilder, V3_4a), primary),
            secondary,
        )
        @test secondary_item(b) !== nothing
        b2 = without_secondary_item(b)
        @test secondary_item(b2) === nothing
    end

    @testset "builder without_metadata_tag clears tag" begin
        primary = from_short_path("411.1/C101.31-2", g, locs)
        @test primary !== nothing
        qty = create_tag(cbs[VistaSdk.Quantity], "temperature")
        @test qty !== nothing
        b = without_metadata_tag(
            with_metadata_tag(
                with_primary_item(create(LocalIdBuilder, V3_4a), primary),
                qty,
            ),
            VistaSdk.Quantity,
        )
        @test quantity(b) === nothing
    end

    @testset "builder individual metadata tag accessors" begin
        primary = from_short_path("411.1/C101.31-2", g, locs)
        @test primary !== nothing
        qty = create_tag(cbs[VistaSdk.Quantity], "temperature")
        cnt = create_tag(cbs[VistaSdk.Content], "exhaust.gas")
        pos = create_tag(cbs[VistaSdk.Position], "inlet")
        @test qty !== nothing && cnt !== nothing && pos !== nothing
        b = with_metadata_tag(
            with_metadata_tag(
                with_metadata_tag(
                    with_primary_item(create(LocalIdBuilder, V3_4a), primary),
                    qty,
                ),
                cnt,
            ),
            pos,
        )
        @test quantity(b) !== nothing
        @test content(b) !== nothing
        @test VistaSdk.position(b) !== nothing
        @test calculation(b) === nothing
        @test state(b) === nothing
        @test command(b) === nothing
        @test detail(b) === nothing
    end

    @testset "builder verbose mode roundtrip" begin
        b = with_verbose_mode(create(LocalIdBuilder, V3_4a), true)
        @test is_verbose_mode(b)
        b2 = with_verbose_mode(b, false)
        @test !is_verbose_mode(b2)
    end

    @testset "builder is_empty_metadata reflects state" begin
        primary = from_short_path("411.1/C101.31-2", g, locs)
        @test primary !== nothing
        qty = create_tag(cbs[VistaSdk.Quantity], "temperature")
        @test qty !== nothing
        empty_b = with_primary_item(create(LocalIdBuilder, V3_4a), primary)
        @test is_empty_metadata(empty_b)
        with_tag = with_metadata_tag(empty_b, qty)
        @test !is_empty_metadata(with_tag)
    end

    @testset "builder has_custom_tag false for standard value" begin
        primary = from_short_path("411.1/C101.31-2", g, locs)
        @test primary !== nothing
        qty = create_tag(cbs[VistaSdk.Quantity], "temperature")
        @test qty !== nothing
        b = with_metadata_tag(
            with_primary_item(create(LocalIdBuilder, V3_4a), primary),
            qty,
        )
        @test !has_custom_tag(b)
    end

    @testset "MqttLocalId to_string uses underscores" begin
        primary = from_short_path("411.1/C101.31-2", g, locs)
        @test primary !== nothing
        qty = create_tag(cbs[VistaSdk.Quantity], "temperature")
        cnt = create_tag(cbs[VistaSdk.Content], "exhaust.gas")
        pos = create_tag(cbs[VistaSdk.Position], "inlet")
        @test qty !== nothing && cnt !== nothing && pos !== nothing
        b = with_metadata_tag(
            with_metadata_tag(
                with_metadata_tag(
                    with_primary_item(create(LocalIdBuilder, V3_4a), primary),
                    qty,
                ),
                cnt,
            ),
            pos,
        )
        mqtt = create(MqttLocalId, b)
        @test mqtt !== nothing
        @test string(mqtt) ==
              "dnv-v2/vis-3-4a/411.1_C101.31-2/_/qty-temperature/cnt-exhaust.gas/_/_/_/_/pos-inlet/_"
    end

    @testset "MqttLocalId has no leading slash" begin
        primary = from_short_path("411.1/C101.31-2", g, locs)
        @test primary !== nothing
        qty = create_tag(cbs[VistaSdk.Quantity], "temperature")
        @test qty !== nothing
        b = with_metadata_tag(
            with_primary_item(create(LocalIdBuilder, V3_4a), primary),
            qty,
        )
        mqtt = create(MqttLocalId, b)
        @test mqtt !== nothing
        @test !startswith(string(mqtt), '/')
    end

    @testset "MqttLocalId getters reflect builder state" begin
        primary = from_short_path("411.1/C101.31-2", g, locs)
        secondary = from_short_path("411.1/C101.31-5", g, locs)
        @test primary !== nothing && secondary !== nothing
        qty = create_tag(cbs[VistaSdk.Quantity], "temperature")
        cnt = create_tag(cbs[VistaSdk.Content], "exhaust.gas")
        @test qty !== nothing && cnt !== nothing
        b = with_metadata_tag(
            with_metadata_tag(
                with_secondary_item(
                    with_primary_item(create(LocalIdBuilder, V3_4a), primary),
                    secondary,
                ),
                qty,
            ),
            cnt,
        )
        mqtt = create(MqttLocalId, b)
        @test mqtt !== nothing
        @test version(mqtt) == V3_4a
        @test calculation(mqtt) === nothing
        @test state(mqtt) === nothing
        @test command(mqtt) === nothing
        @test detail(mqtt) === nothing
    end

    @testset "MqttLocalId equality" begin
        primary = from_short_path("411.1/C101.31-2", g, locs)
        @test primary !== nothing
        qty = create_tag(cbs[VistaSdk.Quantity], "temperature")
        pressure = create_tag(cbs[VistaSdk.Quantity], "pressure")
        @test qty !== nothing && pressure !== nothing
        b1 = with_metadata_tag(
            with_primary_item(create(LocalIdBuilder, V3_4a), primary),
            qty,
        )
        b2 = with_metadata_tag(
            with_primary_item(create(LocalIdBuilder, V3_4a), primary),
            pressure,
        )
        a = create(MqttLocalId, b1)
        b = create(MqttLocalId, b1)
        c = create(MqttLocalId, b2)
        @test a !== nothing && b !== nothing && c !== nothing
        @test a == b
        @test a != c
    end

    @testset "MqttLocalId create from invalid builder returns nothing" begin
        b = create(LocalIdBuilder, V3_4a)
        @test create(MqttLocalId, b) === nothing
    end
end
