using Test
using VistaSdk

@testset "MetadataTagsQuery" begin

    @testset "empty query matches any LocalId" begin
        q = build(create(MetadataTagsQueryBuilder))

        lid1 = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature")
        @test lid1 !== nothing
        @test is_match(q, lid1)

        lid2 = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas",
        )
        @test lid2 !== nothing
        @test is_match(q, lid2)
    end

    @testset "empty query with exact mode rejects any LocalId with tags" begin
        q = build(with_allow_other_tags(create(MetadataTagsQueryBuilder), false))

        lid = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature")
        @test lid !== nothing
        @test !is_match(q, lid)
    end

    @testset "single tag subset mode" begin
        q = build(
            with_allow_other_tags(
                with_tag(create(MetadataTagsQueryBuilder), VistaSdk.Content, "sea.water"),
                true,
            ),
        )

        lid1 = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/652.31/S90.3/S61/sec/652.1i-1P/meta/cnt-sea.water/state-opened",
        )
        @test lid1 !== nothing
        @test is_match(q, lid1)

        lid2 = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas",
        )
        @test lid2 !== nothing
        @test !is_match(q, lid2)

        lid3 = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature")
        @test lid3 !== nothing
        @test !is_match(q, lid3)
    end

    @testset "single tag exact mode" begin
        q = build(
            with_allow_other_tags(
                with_tag(create(MetadataTagsQueryBuilder), VistaSdk.Content, "exhaust.gas"),
                false,
            ),
        )

        lid1 = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/meta/cnt-exhaust.gas")
        @test lid1 !== nothing
        @test is_match(q, lid1)

        lid2 = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas",
        )
        @test lid2 !== nothing
        @test !is_match(q, lid2)
    end

    @testset "multiple tags subset mode" begin
        q = build(
            with_allow_other_tags(
                with_tag(
                    with_tag(
                        create(MetadataTagsQueryBuilder),
                        VistaSdk.Quantity,
                        "temperature",
                    ),
                    VistaSdk.Content,
                    "exhaust.gas",
                ),
                true,
            ),
        )

        lid1 = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
        )
        @test lid1 !== nothing
        @test is_match(q, lid1)

        lid2 = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature")
        @test lid2 !== nothing
        @test !is_match(q, lid2)

        lid3 = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-sea.water",
        )
        @test lid3 !== nothing
        @test !is_match(q, lid3)
    end

    @testset "multiple tags exact mode" begin
        q = build(
            with_allow_other_tags(
                with_tag(
                    with_tag(
                        create(MetadataTagsQueryBuilder),
                        VistaSdk.Quantity,
                        "temperature",
                    ),
                    VistaSdk.Content,
                    "exhaust.gas",
                ),
                false,
            ),
        )

        lid1 = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas",
        )
        @test lid1 !== nothing
        @test is_match(q, lid1)

        lid2 = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
        )
        @test lid2 !== nothing
        @test !is_match(q, lid2)
    end

    @testset "from_local_id subset mode" begin
        source = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas",
        )
        @test source !== nothing
        q = build(from_local_id(MetadataTagsQueryBuilder, source, true))

        @test is_match(q, source)

        lid2 = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
        )
        @test lid2 !== nothing
        @test is_match(q, lid2)
    end

    @testset "from_local_id exact mode" begin
        source = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas",
        )
        @test source !== nothing
        q = build(from_local_id(MetadataTagsQueryBuilder, source, false))

        @test is_match(q, source)

        lid2 = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
        )
        @test lid2 !== nothing
        @test !is_match(q, lid2)
    end

    @testset "with_tag immutability: original builder unchanged" begin
        b1 = create(MetadataTagsQueryBuilder)
        b2 = with_tag(b1, VistaSdk.Quantity, "temperature")

        q1 = build(b1)
        q2 = build(b2)

        lid = from_string(LocalId, "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature")
        @test lid !== nothing
        @test is_match(q1, lid)
        @test is_match(q2, lid)
    end

    @testset "chained with_tag calls" begin
        q = build(
            with_allow_other_tags(
                with_tag(
                    with_tag(
                        with_tag(
                            create(MetadataTagsQueryBuilder),
                            VistaSdk.Quantity,
                            "temperature",
                        ),
                        VistaSdk.Content,
                        "exhaust.gas",
                    ),
                    VistaSdk.Position,
                    "inlet",
                ),
                false,
            ),
        )

        lid = from_string(
            LocalId,
            "/dnv-v2/vis-3-4a/411.1/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
        )
        @test lid !== nothing
        @test is_match(q, lid)
    end

    @testset "builder accessor on query" begin
        q = build(with_tag(create(MetadataTagsQueryBuilder), VistaSdk.Content, "sea.water"))
        b = builder(q)
        @test b isa MetadataTagsQueryBuilderRef
    end

end
