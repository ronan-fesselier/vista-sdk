using Test
using VistaSdk

@testset "GmodPath" begin
    v = vis()
    g = gmod(v, V3_4a)
    locs = locations(v, V3_4a)

    @testset "parse valid short path round-trips" begin
        p = from_short_path("411.1/C101.72/I101", g, locs)
        @test p !== nothing
        @test string(p) == "411.1/C101.72/I101"
    end

    @testset "parse invalid short path returns nothing" begin
        @test from_short_path("NOT/VALID/PATH", g, locs) === nothing
    end

    @testset "full path iteration" begin
        p = from_short_path("411.1/C101.72/I101", g, locs)
        @test p !== nothing
        expected = [
            "VE",
            "400a",
            "410",
            "411",
            "411i",
            "411.1",
            "CS1",
            "C101",
            "C101.7",
            "C101.72",
            "I101",
        ]
        @test length(p) == length(expected)
        for (i, c) in enumerate(expected)
            @test code(p[i]) == c
        end
    end

    @testset "full path round-trip" begin
        p = from_short_path("411.1/C101.72/I101", g, locs)
        @test p !== nothing
        full = to_full_path_string(p)
        @test full == "VE/400a/410/411/411i/411.1/CS1/C101/C101.7/C101.72/I101"
        p2 = from_full_path(full, g, locs)
        @test p2 !== nothing
        @test string(p) == string(p2)
        @test to_full_path_string(p) == to_full_path_string(p2)
    end

    @testset "parents excludes target node" begin
        p = from_short_path("411.1/C101.72/I101", g, locs)
        @test p !== nothing
        ps = parents(p)
        @test length(ps) == length(p) - 1
        @test code(ps[end]) == "C101.72"
        @test !any(n -> code(n) == "I101", ps)
    end

    @testset "is_mappable and is_individualizable" begin
        p = from_short_path("411.1/C101.72/I101", g, locs)
        @test p !== nothing
        @test is_mappable(p)
        @test is_individualizable(p)
    end

    @testset "version matches requested version" begin
        p = from_short_path("411.1/C101.72/I101", g, locs)
        @test p !== nothing
        @test version(p) == V3_4a
    end

    @testset "target node matches short path leaf" begin
        p = from_short_path("411.1/C101.72/I101", g, locs)
        @test p !== nothing
        @test code(node(p)) == "I101"
    end

    @testset "common_names non-empty on known path" begin
        p = from_short_path("411.1/C101.72/I101", g, locs)
        @test p !== nothing
        names = common_names(p)
        @test !isempty(names)
        for (depth, nm) in names
            @test depth < length(p)
            @test !isempty(nm)
        end
    end

    @testset "from_short_path_with_errors valid has no errors" begin
        p, errors = from_short_path_with_errors("411.1/C101.72/I101", g, locs)
        @test p !== nothing
        @test !has_errors(errors)
    end

    @testset "from_short_path_with_errors invalid returns errors" begin
        p, errors = from_short_path_with_errors("INVALID/PATH", g, locs)
        @test p === nothing
        @test has_errors(errors)
    end

    @testset "from_full_path_with_errors valid has no errors" begin
        p, errors = from_full_path_with_errors(
            "VE/400a/410/411/411i/411.1/CS1/C101/C101.7/C101.72/I101",
            g,
            locs,
        )
        @test p !== nothing
        @test !has_errors(errors)
    end

    @testset "equality" begin
        p1 = from_short_path("411.1/C101.72/I101", g, locs)
        p2 = from_short_path("411.1/C101.72/I101", g, locs)
        @test p1 !== nothing && p2 !== nothing
        @test p1 == p2
    end

    @testset "without_locations" begin
        p = from_short_path("411.1/C101.72/I101", g, locs)
        @test p !== nothing
        stripped = without_locations(p)
        @test length(stripped) <= length(p)
    end

    @testset "individualizable sets" begin
        p = from_short_path("652.4/I101", g, locs)
        @test p !== nothing
        count = individualizable_set_count(p)
        @test count > 0
        s = individualizable_set_at(p, 1)
        @test s !== nothing
        @test node_count(s) >= 1
        @test code(node_at(s, 1)) == "652.4"
    end
end
