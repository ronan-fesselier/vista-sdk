using Test
using VistaSdk

@testset "ImoNumber" begin
    @testset "is_valid accepts valid numbers" begin
        @test is_valid(9074729)
        @test is_valid(9785811)
        @test is_valid(9704611)
    end

    @testset "is_valid rejects invalid numbers" begin
        @test !is_valid(-1)
        @test !is_valid(0)
        @test !is_valid(1)
        @test !is_valid(1234507)
    end

    @testset "construct from integer" begin
        imo = ImoNumber(9074729)
        @test value(imo) == 9074729
    end

    @testset "construct invalid throws" begin
        @test_throws VistaError ImoNumber(1234507)
    end

    @testset "parse string without prefix" begin
        imo = parse(ImoNumber, "9074729")
        @test value(imo) == 9074729
    end

    @testset "parse string with IMO prefix" begin
        imo = parse(ImoNumber, "IMO9074729")
        @test value(imo) == 9074729
    end

    @testset "parse with and without prefix are equal" begin
        a = parse(ImoNumber, "9074729")
        b = parse(ImoNumber, "IMO9074729")
        @test a == b
    end

    @testset "parse invalid throws" begin
        @test_throws VistaError parse(ImoNumber, "not-an-imo")
    end

    @testset "construct and parse are equal" begin
        from_int = ImoNumber(9074729)
        from_str = parse(ImoNumber, "9074729")
        @test from_int == from_str
    end

    @testset "string formats with IMO prefix" begin
        imo = parse(ImoNumber, "9074729")
        @test string(imo) == "IMO9074729"
    end

    @testset "different numbers are not equal" begin
        a = parse(ImoNumber, "9074729")
        b = parse(ImoNumber, "9785811")
        @test a != b
    end

    @testset "hash consistency" begin
        a = parse(ImoNumber, "9074729")
        b = parse(ImoNumber, "9074729")
        @test hash(a) == hash(b)
    end
end
