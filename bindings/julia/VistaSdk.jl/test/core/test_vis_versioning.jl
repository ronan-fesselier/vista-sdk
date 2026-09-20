using Test
using VistaSdk

@testset "Vis cross-version conversion" begin
    @testset "convert_path across versions" begin
        cases = [
            ("411.1/C101.72/I101", "411.1/C101.72/I101"),
            ("323.51/H362.1", "323.61/H362.1"),
            ("321.38/C906", "321.39/C906"),
            ("511.331/C221", "511.31/C121.31/C221"),
            ("001", "001"),
        ]

        v = vis()
        source_ver = V3_4a
        target_ver = V3_6a
        source_gmod = gmod(v, source_ver)
        source_locs = locations(v, source_ver)

        for (input, expected) in cases
            path = from_short_path(input, source_gmod, source_locs)
            @test path !== nothing
            converted = convert_path(v, source_ver, path, target_ver)
            @test string(converted) == expected
        end
    end

    @testset "convert_path out of scope throws" begin
        v = vis()
        source_ver = V3_7a
        target_ver = V3_8a
        g = gmod(v, source_ver)
        locs = locations(v, source_ver)
        path = from_short_path("244.1i/H101.111/H401", g, locs)
        @test path !== nothing
        @test_throws VistaError convert_path(v, source_ver, path, target_ver)
    end

    @testset "convert_node across versions" begin
        cases = [
            ("1014.211", "1014.211"),
            ("323.5", "323.6"),
            ("412.72", "412.7i"),
            ("C101.212", "C101.22"),
        ]

        v = vis()
        source_ver = V3_4a
        target_ver = V3_6a
        source_gmod = gmod(v, source_ver)
        target_gmod = gmod(v, target_ver)

        for (input_code, expected_code) in cases
            source_node = get_node(source_gmod, input_code)
            expected_node = get_node(target_gmod, expected_code)
            converted = convert_node(v, source_ver, source_node, target_ver)
            @test code(converted) == code(expected_node)
        end
    end

    @testset "convert_local_id across versions" begin
        cases = [(
            "/dnv-v2/vis-3-4a/411.1/C101/sec/411.1/C101.64i/S201/meta/cnt-condensate",
            "/dnv-v2/vis-3-5a/411.1/C101/sec/411.1/C101.64/S201/meta/cnt-condensate",
        ),]

        v = vis()
        for (source_str, target_str) in cases
            source_lid = from_string(LocalId, source_str)
            target_lid = from_string(LocalId, target_str)
            @test source_lid !== nothing
            @test target_lid !== nothing
            converted = convert_local_id(v, source_lid, version(target_lid))
            @test string(converted) == target_str
        end
    end

    @testset "convert_local_id_builder across versions" begin
        v = vis()
        source_lid = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/411.1/C101/sec/411.1/C101.64i/S201/meta/cnt-condensate",
        )
        @test source_lid !== nothing
        converted_lb = convert_local_id_builder(v, builder(source_lid), V3_5a)
        @test string(converted_lb) ==
              "/dnv-v2/vis-3-5a/411.1/C101/sec/411.1/C101.64/S201/meta/cnt-condensate"
    end
end
