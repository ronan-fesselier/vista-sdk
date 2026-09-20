using Test
using VistaSdk

const VALID_UNIVERSAL_ID_STR = "data.dnv.com/IMO1234567/dnv-v2/vis-3-4a/621.21/S90/sec/411.1/C101/meta/qty-mass/cnt-fuel.oil/pos-inlet"
const VALID_LOCAL_ID_STR = "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet"

@testset "UniversalId" begin
    @testset "naming_entity is data.dnv.com" begin
        @test naming_entity(UniversalId) == "data.dnv.com"
        @test naming_entity(UniversalIdBuilder) == "data.dnv.com"
    end

    @testset "from_string valid returns non-nothing" begin
        uid = from_string(UniversalId, VALID_UNIVERSAL_ID_STR)
        @test uid !== nothing
    end

    @testset "from_string invalid returns nothing" begin
        @test from_string(UniversalId, "not-a-universal-id") === nothing
    end

    @testset "to_string round-trips input" begin
        uid = from_string(UniversalId, VALID_UNIVERSAL_ID_STR)
        @test uid !== nothing
        @test string(uid) == VALID_UNIVERSAL_ID_STR
    end

    @testset "local_id is accessible" begin
        uid = from_string(UniversalId, VALID_UNIVERSAL_ID_STR)
        @test uid !== nothing
        lid = local_id(uid)
        @test lid isa LocalIdRef
    end

    @testset "imo_number matches parsed value" begin
        uid = from_string(UniversalId, VALID_UNIVERSAL_ID_STR)
        @test uid !== nothing
        imo = imo_number(uid)
        @test imo !== nothing
        @test string(imo) == "IMO1234567"
    end

    @testset "builder is_valid on parsed universal id" begin
        uid = from_string(UniversalId, VALID_UNIVERSAL_ID_STR)
        @test uid !== nothing
        @test is_valid(builder(uid))
    end

    @testset "equality" begin
        a = from_string(UniversalId, VALID_UNIVERSAL_ID_STR)
        b = from_string(UniversalId, VALID_UNIVERSAL_ID_STR)
        @test a !== nothing && b !== nothing
        @test a == b
    end

    @testset "builder create returns invalid state" begin
        b = create(UniversalIdBuilder, V3_4a)
        @test !is_valid(b)
    end

    @testset "builder build invalid throws" begin
        b = create(UniversalIdBuilder, V3_4a)
        @test_throws VistaError build(b)
    end

    @testset "builder fluent chain builds valid universal id" begin
        local_id_val = from_string(LocalId, VALID_LOCAL_ID_STR)
        @test local_id_val !== nothing
        imo = ImoNumber(9074729)
        uid = build(
            with_local_id(
                with_imo_number(create(UniversalIdBuilder, V3_4a), imo),
                builder(local_id_val),
            ),
        )
        @test is_valid(builder(uid))
    end

    @testset "builder without_imo_number clears imo" begin
        local_id_val = from_string(LocalId, VALID_LOCAL_ID_STR)
        @test local_id_val !== nothing
        imo = ImoNumber(9074729)
        b = with_local_id(
            with_imo_number(create(UniversalIdBuilder, V3_4a), imo),
            builder(local_id_val),
        )
        b2 = without_imo_number(b)
        @test imo_number(b2) === nothing
        @test !is_valid(b2)
    end

    @testset "builder without_local_id clears local id" begin
        local_id_val = from_string(LocalId, VALID_LOCAL_ID_STR)
        @test local_id_val !== nothing
        imo = ImoNumber(9074729)
        b = with_local_id(
            with_imo_number(create(UniversalIdBuilder, V3_4a), imo),
            builder(local_id_val),
        )
        b2 = without_local_id(b)
        @test local_id(b2) === nothing
        @test !is_valid(b2)
    end

    @testset "builder from_string valid returns non-nothing" begin
        uid = from_string(UniversalIdBuilder, VALID_UNIVERSAL_ID_STR)
        @test uid !== nothing
    end

    @testset "builder from_string invalid returns nothing" begin
        @test from_string(UniversalIdBuilder, "not-a-universal-id") === nothing
    end

    @testset "builder from_string round-trips to_string" begin
        uid = from_string(UniversalIdBuilder, VALID_UNIVERSAL_ID_STR)
        @test uid !== nothing
        @test string(uid) == VALID_UNIVERSAL_ID_STR
    end

    @testset "from_string_with_errors invalid has errors" begin
        uid, errors = from_string_with_errors(UniversalIdBuilder, "not-a-universal-id")
        @test uid === nothing
        @test has_errors(errors)
    end

    @testset "from_string_with_errors valid has no errors" begin
        uid, errors = from_string_with_errors(UniversalIdBuilder, VALID_UNIVERSAL_ID_STR)
        @test uid !== nothing
        @test !has_errors(errors)
    end
end
