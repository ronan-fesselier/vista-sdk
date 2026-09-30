const TSD_SAMPLE_JSON = read(
    joinpath(
        @__DIR__,
        "..",
        "..",
        "..",
        "..",
        "..",
        "..",
        "schemas",
        "json",
        "TimeSeriesData.sample.json",
    ),
    String,
)

@testset "tsd_from_json: round-trip preserves ship_id" begin
    p = tsd_from_json(TSD_SAMPLE_JSON)
    json_out = tsd_to_json(p)
    p2 = tsd_from_json(json_out)
    dto = tsd_to_dto(p2)
    h = header(pkg(dto))
    @test h !== nothing
    @test ship_id(h) !== nothing
end

@testset "tsd_from_json: round-trip produces non-empty output" begin
    p = tsd_from_json(TSD_SAMPLE_JSON)
    json_out = tsd_to_json(p)
    @test length(json_out) > 0
end

@testset "tsd_to_json: pretty output is longer" begin
    p = tsd_from_json(TSD_SAMPLE_JSON)
    compact = tsd_to_json(p)
    pretty = tsd_to_json(p, true)
    @test length(pretty) > length(compact)
end

@testset "tsd_from_json: invalid JSON throws VistaError" begin
    @test_throws VistaError tsd_from_json("not json")
end
