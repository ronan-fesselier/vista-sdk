using Test
using VistaSdk

@testset "Locations" begin
    @testset "version matches request" begin
        locs = locations(vis(), V3_11a)
        @test version(locs) == V3_11a
    end

    @testset "relative location count and iteration" begin
        locs = locations(vis(), V3_11a)
        @test length(locs) > 0
        first_rl = locs[1]
        @test code(first_rl) != '\0'
        @test !isempty(name(first_rl))
        @test !isempty(location_value(first_rl))
        @test length(collect(locs)) == length(locs)
    end

    @testset "out of range returns BoundsError" begin
        locs = locations(vis(), V3_11a)
        @test_throws BoundsError locs[length(locs)+1]
    end

    @testset "group Side has entries" begin
        locs = locations(vis(), V3_11a)
        side_entries = group(locs, VistaSdk.Side)
        @test length(side_entries) > 0
    end

    @testset "LocationGroup variants" begin
        locs = locations(vis(), V3_11a)
        for grp in [
            VistaSdk.NumberGroup,
            VistaSdk.Side,
            VistaSdk.Vertical,
            VistaSdk.Transverse,
            VistaSdk.Longitudinal,
        ]
            entries = group(locs, grp)
            @test entries isa Vector{RelativeLocation}
        end
        @test Int32(VistaSdk.NumberGroup) == 0
        @test Int32(VistaSdk.Side) == 1
        @test Int32(VistaSdk.Vertical) == 2
        @test Int32(VistaSdk.Transverse) == 3
        @test Int32(VistaSdk.Longitudinal) == 4
    end

    @testset "parse valid string" begin
        locs = locations(vis(), V3_11a)
        loc = parse(Location, locs, "11FIPU")
        @test loc !== nothing
        @test string(loc) == "11FIPU"
    end

    @testset "parse invalid string returns nothing" begin
        locs = locations(vis(), V3_11a)
        @test parse(Location, locs, "not-a-location") === nothing
    end

    @testset "parse_with_errors valid yields empty errors" begin
        locs = locations(vis(), V3_11a)
        loc, errors = parse_with_errors(locs, "11FIPU")
        @test loc !== nothing
        @test !has_errors(errors)
    end

    @testset "parse_with_errors invalid yields errors" begin
        locs = locations(vis(), V3_11a)
        loc, errors = parse_with_errors(locs, "not-a-location")
        @test loc === nothing
        @test has_errors(errors)
        @test length(errors) > 0
    end
end

@testset "LocationBuilder" begin
    @testset "create returns builder" begin
        locs = locations(vis(), V3_11a)
        b = LocationBuilder(locs)
        @test string(b) == ""
    end

    @testset "version matches request" begin
        locs = locations(vis(), V3_11a)
        b = LocationBuilder(locs)
        @test version(b) == V3_11a
    end

    @testset "fluent chain builds matching location" begin
        locs = locations(vis(), V3_11a)
        b =
            LocationBuilder(locs) |>
            b ->
                with_number(b, 11) |>
                b ->
                    with_side(b, 'P') |>
                    b ->
                        with_transverse(b, 'I') |>
                        b -> with_longitudinal(b, 'F') |> b -> with_vertical(b, 'U')
        loc = build(b)
        @test string(loc) == "11FIPU"
    end

    @testset "component getters reflect chain" begin
        locs = locations(vis(), V3_11a)
        b = with_number(LocationBuilder(locs), 11) |> b -> with_side(b, 'P')
        @test number(b) == 11
        @test side(b) == 'P'
        @test vertical(b) === nothing
    end

    @testset "without_number clears component" begin
        locs = locations(vis(), V3_11a)
        b = without_number(with_number(LocationBuilder(locs), 11))
        @test number(b) === nothing
        @test string(b) == ""
    end

    @testset "with_number invalid throws" begin
        locs = locations(vis(), V3_11a)
        @test_throws VistaError with_number(LocationBuilder(locs), 0)
    end

    @testset "with_side invalid throws" begin
        locs = locations(vis(), V3_11a)
        @test_throws VistaError with_side(LocationBuilder(locs), 'A')
    end

    @testset "with_code auto-detects group" begin
        locs = locations(vis(), V3_11a)
        via_code = with_code(LocationBuilder(locs), 'P')
        via_side = with_side(LocationBuilder(locs), 'P')
        @test string(via_code) == string(via_side)
    end

    @testset "with_location reconstructs builder" begin
        locs = locations(vis(), V3_11a)
        loc = parse(Location, locs, "11FIPU")
        @test loc !== nothing
        b = with_location(LocationBuilder(locs), loc)
        @test string(b) == "11FIPU"
    end

    @testset "without_value clears targeted group" begin
        locs = locations(vis(), V3_11a)
        loc = parse(Location, locs, "11FIPU")
        @test loc !== nothing
        b = without_value(with_location(LocationBuilder(locs), loc), VistaSdk.Side)
        @test side(b) === nothing
    end
end
