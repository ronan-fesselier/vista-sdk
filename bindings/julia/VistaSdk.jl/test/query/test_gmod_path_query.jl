using Test
using VistaSdk

@testset "GmodPathQuery" begin

    v = vis()
    g34 = gmod(v, V3_4a)
    l34 = locations(v, V3_4a)
    g37 = gmod(v, V3_7a)
    l37 = locations(v, V3_7a)
    g39 = gmod(v, V3_9a)
    l39 = locations(v, V3_9a)

    @testset "path builder matches itself" begin
        path = from_short_path("411.1-1/C101", g34, l34)
        @test path !== nothing
        q = build(from_path(GmodPathQueryBuilder, path))
        @test is_match(q, path)
    end

    @testset "path builder parametrized" begin
        cases = [
            ("411.1-1/C101", V3_4a, [], true),
            ("411.1-1/C101", V3_4a, [("411.1", ["1"])], true),
            ("411.1-1/C101", V3_4a, [("411.1", ["A"])], false),
            ("433.1-P/C322.31/C173", V3_4a, [("C322.31", String[])], true),
            (
                "433.1-P/C322.31-2/C173",
                V3_4a,
                [("433.1", ["P"]), ("C322.31", String[])],
                true,
            ),
            (
                "433.1-P/C322.31-2/C173",
                V3_4a,
                [("433.1", ["A"]), ("C322.31", String[])],
                false,
            ),
            (
                "433.1-P/C322.31-2/C173",
                V3_4a,
                [("433.1", ["P"]), ("C322.31", ["1"])],
                false,
            ),
            ("433.1/C322.31-2/C173", V3_4a, [("433.1", ["A"])], false),
            ("433.1/C322.31-2/C173", V3_4a, [("433.1", String[])], true),
        ]

        for (path_str, ver, params, expected) in cases
            g = gmod(v, ver)
            l = locations(v, ver)
            path = from_short_path(path_str, g, l)
            @test path !== nothing

            b = from_path(GmodPathQueryBuilder, path)
            @test is_match(build(b), path)

            for (node_code, loc_strs) in params
                if isempty(loc_strs)
                    b = path_with_node_all_locations(b, node_code, true)
                else
                    locs_parsed = [Base.parse(Location, l, loc_s) for loc_s in loc_strs]
                    b = path_with_node_locations(b, node_code, locs_parsed)
                end
            end

            @test is_match(build(b), path) == expected
        end
    end

    @testset "nodes builder parametrized" begin
        cases = [
            ("411.1-1/C101", V3_4a, [("411.1", ["1"])], true),
            ("411.1/C101.61-1/S203.3/S110.2/C101", V3_7a, [("C101.61", ["1"])], true),
            ("511.11/C101.61-1/S203.3/S110.2/C101", V3_7a, [("C101.61", ["1"])], true),
            ("411.1/C101.61-1/S203.3/S110.2/C101", V3_7a, [("C101.61", String[])], true),
            (
                "511.11/C101.61-1/S203.3/S110.2/C101",
                V3_7a,
                [("411.1", String[]), ("C101.61", String[])],
                false,
            ),
        ]

        for (path_str, ver, params, expected) in cases
            g = gmod(v, ver)
            l = locations(v, ver)
            path = from_short_path(path_str, g, l)
            @test path !== nothing

            b = create(GmodPathQueryBuilder)
            for (node_code, loc_strs) in params
                node = get_node(g, node_code)
                if isempty(loc_strs)
                    b = with_node_all_locations(b, node, true)
                else
                    locs_parsed = [Base.parse(Location, l, loc_s) for loc_s in loc_strs]
                    b = with_node_locations(b, node, locs_parsed)
                end
            end

            @test is_match(build(b), path) == expected
        end
    end

    @testset "with_any_node_before ignores parent nodes" begin
        base = from_short_path("411.1/C101.31", g39, l39)
        @test base !== nothing
        q = build(
            without_locations(
                with_any_node_before(from_path(GmodPathQueryBuilder, base), "C101"),
            ),
        )
        @test is_match(q, base)

        diff_parent = from_short_path("511.11/C101.31", g39, l39)
        @test diff_parent !== nothing
        @test is_match(q, diff_parent)

        diff_c = from_short_path("411.1/C102.31", g39, l39)
        @test diff_c !== nothing
        @test !is_match(q, diff_c)
    end

    @testset "with_any_node_after ignores children" begin
        base = from_short_path("411.1/C101.31", g34, l34)
        @test base !== nothing
        q = build(
            with_any_node_after(
                without_locations(from_path(GmodPathQueryBuilder, base)),
                "411.1",
            ),
        )

        p1 = from_short_path("411.1/C101.31-2", g34, l34)
        @test p1 !== nothing
        @test is_match(q, p1)

        p2 = from_short_path("511.11/C101.63/S206", g34, l34)
        @test p2 !== nothing
        @test !is_match(q, p2)
    end

    @testset "with_any_node_after errors when node not in path" begin
        base = from_short_path("411.1/C101.63/S206", g34, l34)
        @test base !== nothing
        b = without_locations(from_path(GmodPathQueryBuilder, base))
        @test_throws VistaError with_any_node_after(b, "C101.31")
    end

    @testset "without_locations ignores location individualization" begin
        base = from_short_path("411.1-1/C101.31", g34, l34)
        @test base !== nothing
        q = build(without_locations(from_path(GmodPathQueryBuilder, base)))

        for s in ["411.1-1/C101.31", "411.1-2/C101.31", "411.1/C101.31"]
            p = from_short_path(s, g34, l34)
            @test p !== nothing
            @test is_match(q, p)
        end
    end

    @testset "path-only methods error on nodes builder" begin
        node = get_node(g34, "411.1")
        nb = with_node_all_locations(create(GmodPathQueryBuilder), node, true)
        @test_throws VistaError path_with_node_all_locations(nb, "411.1", true)
        @test_throws VistaError with_any_node_before(nb, "411.1")
        @test_throws VistaError with_any_node_after(nb, "411.1")
        @test_throws VistaError without_locations(nb)
    end

    @testset "nodes-only methods error on path builder" begin
        path = from_short_path("411.1/C101.31", g34, l34)
        @test path !== nothing
        pb = from_path(GmodPathQueryBuilder, path)
        node = get_node(g34, "411.1")
        @test_throws VistaError with_node_all_locations(pb, node, true)
        @test_throws VistaError with_node_locations(pb, node, Location[])
    end

    @testset "builder immutability" begin
        path = from_short_path("411.1/C101.31", g34, l34)
        @test path !== nothing
        b1 = from_path(GmodPathQueryBuilder, path)
        q1 = build(b1)
        b2 = without_locations(b1)
        q2 = build(b2)
        @test is_match(q1, path)
        @test is_match(q2, path)
    end

    @testset "path accessor" begin
        path = from_short_path("411.1/C101.31", g34, l34)
        @test path !== nothing
        b = from_path(GmodPathQueryBuilder, path)
        p = VistaSdk.path(b)
        @test p !== nothing
        @test p isa GmodPathRef
    end

end
