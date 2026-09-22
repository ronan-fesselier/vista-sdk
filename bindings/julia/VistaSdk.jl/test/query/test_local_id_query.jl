using Test
using VistaSdk

@testset "LocalIdQuery" begin

    v = vis()
    g34 = gmod(v, V3_4a)
    l34 = locations(v, V3_4a)
    g37 = gmod(v, V3_7a)
    l37 = locations(v, V3_7a)

    @testset "empty query matches all LocalIds" begin
        q = build(create(LocalIdQueryBuilder))

        lid1 = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power")
        @test lid1 !== nothing
        @test is_match(q, lid1)

        lid2 = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/652.31/S90.3/S61/sec/652.1i-1P/meta/cnt-sea.water/state-opened",
        )
        @test lid2 !== nothing
        @test is_match(q, lid2)
    end

    @testset "from_local_id builds exact match query" begin
        lid = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power")
        @test lid !== nothing
        q = build(from_local_id(LocalIdQueryBuilder, lid))
        @test is_match(q, lid)
    end

    @testset "from_string builds exact match query" begin
        s = "/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power"
        q = build(from_string(LocalIdQueryBuilder, s))
        lid = from_string(LocalId, s)
        @test lid !== nothing
        @test is_match(q, lid)
    end

    @testset "with_primary_item_query without_locations" begin
        path = from_short_path("411.1/C101.31", g34, l34)
        @test path !== nothing

        primary_q = build(without_locations(from_path(GmodPathQueryBuilder, path)))
        q = build(with_primary_item_query(create(LocalIdQueryBuilder), primary_q))

        lid1 = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power")
        @test lid1 !== nothing
        @test is_match(q, lid1)

        lid2 = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1-1/C101.31/meta/qty-power")
        @test lid2 !== nothing
        @test is_match(q, lid2)

        lid3 = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/C102.31/meta/qty-power")
        @test lid3 !== nothing
        @test !is_match(q, lid3)
    end

    @testset "with_primary_item exact path rejects different location" begin
        path = from_short_path("411.1/C101.31-1", g34, l34)
        @test path !== nothing
        q = build(with_primary_item(create(LocalIdQueryBuilder), path))

        lid = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
        )
        @test lid !== nothing
        @test !is_match(q, lid)
    end

    @testset "with_primary_item_nodes_builder match by node code" begin
        node_c101 = get_node(g34, "C101")
        primary_q =
            build(with_node_all_locations(create(GmodPathQueryBuilder), node_c101, true))
        q = build(with_primary_item_query(create(LocalIdQueryBuilder), primary_q))

        lid1 = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power")
        @test lid1 !== nothing
        @test is_match(q, lid1)

        lid2 = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/C102.31/meta/qty-power")
        @test lid2 !== nothing
        @test !is_match(q, lid2)
    end

    @testset "with_secondary_item_query without_locations" begin
        path = from_short_path("411.1/C101.31-2", g34, l34)
        @test path !== nothing
        secondary_q = build(without_locations(from_path(GmodPathQueryBuilder, path)))
        q = build(with_secondary_item_query(create(LocalIdQueryBuilder), secondary_q))

        lid = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/411.1/C101.63/S206/sec/411.1/C101.31-5/~propulsion.engine/~cooling.system/~for.propulsion.engine/~cylinder.5/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
        )
        @test lid !== nothing
        @test is_match(q, lid)
    end

    @testset "with_tags match by content" begin
        tags_q =
            build(with_tag(create(MetadataTagsQueryBuilder), VistaSdk.Content, "sea.water"))
        q = build(with_tags(create(LocalIdQueryBuilder), tags_q))

        lid = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/652.31/S90.3/S61/sec/652.1i-1P/meta/cnt-sea.water/state-opened",
        )
        @test lid !== nothing
        @test is_match(q, lid)
    end

    @testset "tags_builder accessor" begin
        tags_q =
            build(with_tag(create(MetadataTagsQueryBuilder), VistaSdk.Content, "sea.water"))
        b = with_tags(create(LocalIdQueryBuilder), tags_q)
        tb = tags_builder(b)
        @test tb !== nothing
        @test tb isa MetadataTagsQueryBuilderRef
    end

    @testset "primary_item accessor" begin
        path = from_short_path("411.1/C101.31", g34, l34)
        @test path !== nothing
        b = with_primary_item(create(LocalIdQueryBuilder), path)
        p = primary_item(b)
        @test p !== nothing
        @test p isa GmodPathRef
    end

    @testset "with_primary_item_nodes_builder rejects path builder" begin
        path = from_short_path("433.1-S/C322.91/S205", g37, l37)
        @test path !== nothing
        pb = from_path(GmodPathQueryBuilder, path)
        @test_throws VistaError with_primary_item_nodes_builder(
            create(LocalIdQueryBuilder),
            pb,
        )
    end

    @testset "with_primary_item_path_builder rejects nodes builder" begin
        node = get_node(g34, "C101")
        nb = with_node_all_locations(create(GmodPathQueryBuilder), node, true)
        @test_throws VistaError with_primary_item_path_builder(
            create(LocalIdQueryBuilder),
            nb,
        )
    end

    @testset "is_match_str" begin
        s = "/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power"
        q = build(from_string(LocalIdQueryBuilder, s))
        @test is_match_str(q, s)
        @test !is_match_str(q, "/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-temperature")
    end

    @testset "without_secondary_item" begin
        q = build(without_secondary_item(create(LocalIdQueryBuilder)))

        lid_no_sec = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power")
        @test lid_no_sec !== nothing
        @test is_match(q, lid_no_sec)

        lid_with_sec = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/411.1/C101.31/sec/652.1i-1P/meta/qty-power",
        )
        @test lid_with_sec !== nothing
        @test !is_match(q, lid_with_sec)
    end

    @testset "with_any_secondary_item" begin
        q = build(with_any_secondary_item(create(LocalIdQueryBuilder)))

        lid_no_sec = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/C101.31/meta/qty-power")
        @test lid_no_sec !== nothing
        @test is_match(q, lid_no_sec)

        lid_with_sec = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/411.1/C101.31/sec/652.1i-1P/meta/qty-power",
        )
        @test lid_with_sec !== nothing
        @test is_match(q, lid_with_sec)
    end

end
