@testset "ShipId" begin
    @testset "construction from imo number" begin
        imo = parse(ImoNumber, "9074729")
        s = from_imo_number(imo)
        @test is_imo_number(s)
        @test !is_other_id(s)
    end

    @testset "construction from other id" begin
        s = from_other_id("VESSEL-123")
        @test !is_imo_number(s)
        @test is_other_id(s)
    end

    @testset "construction from empty other id throws" begin
        @test_throws VistaError from_other_id("")
    end

    @testset "imo_number accessor" begin
        imo = parse(ImoNumber, "9074729")
        s = from_imo_number(imo)
        @test imo_number(s) == imo
        @test other_id(s) === nothing
    end

    @testset "other_id accessor" begin
        s = from_other_id("VESSEL-ABC-456")
        @test other_id(s) == "VESSEL-ABC-456"
        @test imo_number(s) === nothing
    end

    @testset "show imo number" begin
        imo = parse(ImoNumber, "9074729")
        s = from_imo_number(imo)
        @test string(s) == "IMO9074729"
    end

    @testset "show other id" begin
        s = from_other_id("CUSTOM-SHIP-ID")
        @test string(s) == "CUSTOM-SHIP-ID"
    end

    @testset "from_string imo with prefix" begin
        s = from_string(ShipId, "IMO9074729")
        @test s !== nothing
        @test is_imo_number(s)
        @test string(imo_number(s)) == "IMO9074729"
    end

    @testset "from_string imo case insensitive" begin
        s1 = from_string(ShipId, "imo9074729")
        @test s1 !== nothing && is_imo_number(s1)
        s2 = from_string(ShipId, "ImO9074729")
        @test s2 !== nothing && is_imo_number(s2)
    end

    @testset "from_string invalid imo falls back to other" begin
        s = from_string(ShipId, "IMO1234568")
        @test s !== nothing
        @test is_other_id(s)
        @test other_id(s) == "IMO1234568"
    end

    @testset "from_string other id" begin
        s = from_string(ShipId, "VESSEL-XYZ-789")
        @test s !== nothing
        @test is_other_id(s)
        @test other_id(s) == "VESSEL-XYZ-789"
    end

    @testset "from_string empty returns nothing" begin
        @test from_string(ShipId, "") === nothing
    end

    @testset "from_string whitespace returns nothing" begin
        @test from_string(ShipId, "   ") === nothing
    end

    @testset "equality imo numbers" begin
        s1 = from_imo_number(parse(ImoNumber, "9074729"))
        s2 = from_imo_number(parse(ImoNumber, "9074729"))
        s3 = from_imo_number(ImoNumber(1234567))
        @test s1 == s2
        @test s1 != s3
    end

    @testset "equality other ids" begin
        s1 = from_other_id("VESSEL-A")
        s2 = from_other_id("VESSEL-A")
        s3 = from_other_id("VESSEL-B")
        @test s1 == s2
        @test s1 != s3
    end

    @testset "equality imo vs other" begin
        s1 = from_imo_number(parse(ImoNumber, "9074729"))
        s2 = from_other_id("9074729")
        @test s1 != s2
    end

    @testset "round-trip imo number" begin
        s1 = from_imo_number(parse(ImoNumber, "9074729"))
        s2 = from_string(ShipId, string(s1))
        @test s2 !== nothing
        @test s1 == s2
        @test is_imo_number(s2)
    end

    @testset "round-trip other id" begin
        s1 = from_other_id("VESSEL-ROUND-TRIP")
        s2 = from_string(ShipId, string(s1))
        @test s2 !== nothing
        @test s1 == s2
        @test is_other_id(s2)
    end
end
