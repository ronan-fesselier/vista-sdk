using Test
using VistaSdk

@testset "VisVersion" begin
    @testset "show" begin
        @test string(V3_11a) == "3-11a"
    end

    @testset "parse known versions" begin
        @test parse(VisVersion, "3-4a") == V3_4a
        @test parse(VisVersion, "3-11a") == V3_11a
    end

    @testset "parse invalid" begin
        @test_throws ArgumentError parse(VisVersion, "invalid")
        @test_throws ArgumentError parse(VisVersion, "")
        @test_throws ArgumentError parse(VisVersion, "   ")
        @test_throws ArgumentError parse(VisVersion, "\t")
        @test_throws ArgumentError parse(VisVersion, "\n")
    end

    @testset "parse case sensitive" begin
        @test_throws ArgumentError parse(VisVersion, "3-11A")
        @test parse(VisVersion, "3-11a") == V3_11a
    end

    @testset "roundtrip all versions" begin
        for v in instances(VisVersion)
            @test parse(VisVersion, string(v)) == v
        end
    end

    @testset "all non-empty" begin
        @test !isempty(instances(VisVersion))
    end
end
