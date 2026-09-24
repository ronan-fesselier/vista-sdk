import VistaSdk:
    Decimal,
    RoundingMode,
    ToNearest,
    ToNearestTiesAway,
    ToZero,
    ToPositiveInfinity,
    ToNegativeInfinity

@testset "Decimal" begin

    @testset "zero" begin
        d = zero(Decimal)
        @test d == Decimal(0)
        @test Base.string(d) == "0"
    end

    @testset "from Integer" begin
        @test Base.string(Decimal(42)) == "42"
        @test Base.string(Decimal(-7)) == "-7"
        @test Base.string(Decimal(1_000_000)) == "1000000"
    end

    @testset "from Float64" begin
        d = Decimal(1.25)
        @test abs(to_f64(d) - 1.25) < 1e-10
    end

    @testset "parse valid" begin
        d = Base.parse(Decimal, "123.45")
        @test Base.string(d) == "123.45"
    end

    @testset "parse negative" begin
        d = Base.parse(Decimal, "-0.001")
        @test Base.string(d) == "-0.001"
    end

    @testset "parse invalid throws" begin
        @test_throws Exception Base.parse(Decimal, "not-a-number")
    end

    @testset "equality" begin
        a = Base.parse(Decimal, "1.5")
        b = Base.parse(Decimal, "1.5")
        c = Base.parse(Decimal, "2.0")
        @test a == b
        @test a != c
    end

    @testset "ordering" begin
        a = Base.parse(Decimal, "1.0")
        b = Base.parse(Decimal, "2.0")
        @test a < b
        @test b > a
        @test a <= a
    end

    @testset "addition" begin
        a = Base.parse(Decimal, "45.72")
        b = Base.parse(Decimal, "12.00")
        @test Base.string(a + b) == "57.72"
    end

    @testset "subtraction" begin
        a = Base.parse(Decimal, "87.3")
        b = Base.parse(Decimal, "72.1")
        @test Base.string(a - b) == "15.2"
    end

    @testset "multiplication exact" begin
        consumption = Base.parse(Decimal, "45.72")
        days = Decimal(12)
        @test Base.string(consumption * days) == "548.64"
    end

    @testset "multiplication vs float accumulation" begin
        increment = Base.parse(Decimal, "0.047")
        total = zero(Decimal)
        for _ = 1:1000
            total = total + increment
        end
        double_total = sum(0.047 for _ = 1:1000)
        @test total == Decimal(47)
        @test double_total != 47.0
    end

    @testset "division" begin
        a = Base.parse(Decimal, "10.0")
        b = Base.parse(Decimal, "4.0")
        @test Base.string(a / b) == "2.5"
    end

    @testset "division by zero throws" begin
        @test_throws VistaError Decimal(10.0) / zero(Decimal)
    end

    @testset "sqrt" begin
        @test Base.string(sqrt(Decimal(4.0))) == "2"
    end

    @testset "sqrt of negative throws" begin
        @test_throws VistaError sqrt(Decimal(-4.0))
    end

    @testset "negation" begin
        d = Base.parse(Decimal, "1.5")
        @test Base.string(-d) == "-1.5"
    end

    @testset "abs" begin
        d = Base.parse(Decimal, "-1.25")
        @test Base.string(Base.abs(d)) == "1.25"
    end

    @testset "ceil" begin
        d = Base.parse(Decimal, "97.4375")
        @test Base.string(Base.ceil(d)) == "98"
    end

    @testset "floor" begin
        d = Base.parse(Decimal, "97.4375")
        @test Base.string(Base.floor(d)) == "97"
    end

    @testset "trunc" begin
        d = Base.parse(Decimal, "97.4375")
        @test Base.string(trunc(d)) == "97"
    end

    @testset "round ToNearest" begin
        d = Base.parse(Decimal, "97.4375")
        @test Base.string(round(d, 1, ToNearest)) == "97.4"
    end

    @testset "round ToNearestTiesAway" begin
        d = Base.parse(Decimal, "97.4375")
        @test Base.string(round(d, 1, ToNearestTiesAway)) == "97.4"
    end

    @testset "scale" begin
        d = Base.parse(Decimal, "0.8914")
        @test scale(d) == 4
    end

    @testset "typemin typemax lowest" begin
        @test Base.typemin(Decimal) > zero(Decimal)
        @test Base.typemax(Decimal) > Base.typemin(Decimal)
        @test lowest(Decimal) < zero(Decimal)
    end

    @testset "show" begin
        d = Base.parse(Decimal, "1.23")
        @test repr(d) == "Decimal(1.23)"
    end

    @testset "from_f64 precision" begin
        from_non_exact = Decimal(0.1 + 0.2)
        exact = Base.parse(Decimal, "0.3")
        @test from_non_exact != exact
    end

end
