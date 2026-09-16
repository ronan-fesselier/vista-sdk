using Test
using VistaSdk

@testset "Codebooks" begin
    @testset "loads all versions" begin
        for ver in versions(vis())
            cbs = codebooks(vis(), ver)
            @test version(cbs) == ver
        end
    end

    @testset "has standard value" begin
        cbs = codebooks(vis(), V3_4a)
        @test has_standard_value(cbs[VistaSdk.Position], "centre")
    end

    @testset "name returns correct codebook name" begin
        cbs = codebooks(vis(), V3_4a)
        @test name(cbs[VistaSdk.Quantity]) == VistaSdk.Quantity
        @test name(cbs[VistaSdk.Position]) == VistaSdk.Position
        @test name(cbs[VistaSdk.Detail]) == VistaSdk.Detail
    end

    @testset "standard_values non-empty" begin
        cbs = codebooks(vis(), V3_4a)
        @test !isempty(standard_values(cbs[VistaSdk.Quantity]))
    end

    @testset "groups non-empty for Position" begin
        cbs = codebooks(vis(), V3_4a)
        @test !isempty(groups(cbs[VistaSdk.Position]))
    end

    @testset "has_group known and unknown" begin
        cbs = codebooks(vis(), V3_4a)
        cb = cbs[VistaSdk.Position]
        g = first(groups(cb))
        @test has_group(cb, g)
        @test !has_group(cb, "__nonexistent__")
    end

    @testset "validate_position" begin
        cbs = codebooks(vis(), V3_4a)
        cb = cbs[VistaSdk.Position]
        @test validate_position(cb, "centre") == VistaSdk.Valid
        @test validate_position(cb, "customvalue") == VistaSdk.Custom
        @test validate_position(cb, "INVALID VALUE WITH SPACES") == VistaSdk.Invalid
    end

    @testset "create_tag valid" begin
        cbs = codebooks(vis(), V3_4a)
        tag = create_tag(cbs[VistaSdk.Quantity], "temperature")
        @test tag !== nothing
        @test string(tag) == "qty-temperature"
    end

    @testset "create_tag invalid returns nothing" begin
        cbs = codebooks(vis(), V3_4a)
        @test create_tag(cbs[VistaSdk.Quantity], "invalid value with spaces") === nothing
    end

    @testset "create_tag unknown valid value returns custom tag" begin
        cbs = codebooks(vis(), V3_4a)
        tag = create_tag(cbs[VistaSdk.Quantity], "nonexistent")
        @test tag !== nothing
        @test is_custom(tag)
    end

    @testset "MetadataTag show" begin
        cbs = codebooks(vis(), V3_4a)
        tag = create_tag(cbs[VistaSdk.Content], "exhaust.gas")
        @test tag !== nothing
        @test string(tag) == "cnt-exhaust.gas"
    end
end
