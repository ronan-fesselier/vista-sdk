@testset "Iso19848Version" begin
    @testset "from_str invalid returns error" begin
        @test_throws ArgumentError Base.parse(Iso19848Version, "v9999")
        @test_throws ArgumentError Base.parse(Iso19848Version, "")
        @test_throws ArgumentError Base.parse(Iso19848Version, "2018")
    end

    @testset "ordering ascending" begin
        @test V2018 < V2024
    end

    @testset "known as_str values" begin
        @test Base.string(V2018) == "v2018"
        @test Base.string(V2024) == "v2024"
    end

    @testset "round-trip" begin
        @test Base.parse(Iso19848Version, "v2018") == V2018
        @test Base.parse(Iso19848Version, "v2024") == V2024
    end

    @testset "show" begin
        @test repr(V2018) == "v2018"
        @test repr(V2024) == "v2024"
    end
end
