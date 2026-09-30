@testset "TsdDtoPackage: parse from JSON" begin
    p = tsd_dto_from_json(TSD_SAMPLE_JSON)
    @test p isa TsdDtoPackage
end

@testset "TsdDtoPackage: invalid JSON throws" begin
    @test_throws VistaError tsd_dto_from_json("{invalid}")
end

@testset "TsdDtoPkgRef: has header" begin
    p = tsd_dto_from_json(TSD_SAMPLE_JSON)
    @test header(pkg(p)) !== nothing
end

@testset "TsdDtoHeaderRef: ship_id" begin
    p = tsd_dto_from_json(TSD_SAMPLE_JSON)
    h = header(pkg(p))
    @test h !== nothing
    @test ship_id(h) !== nothing
    @test length(ship_id(h)) > 0
end

@testset "TsdDtoPkgRef: tsd_count" begin
    p = tsd_dto_from_json(TSD_SAMPLE_JSON)
    @test tsd_count(pkg(p)) > 0
end

@testset "TsdDtoTsdRef: first tsd accessible" begin
    p = tsd_dto_from_json(TSD_SAMPLE_JSON)
    t = tsd_at(pkg(p), 1)
    @test t !== nothing
end

@testset "tsd_to_dto: round-trip via domain" begin
    p = tsd_from_json(TSD_SAMPLE_JSON)
    dto = tsd_to_dto(p)
    @test dto isa TsdDtoPackage
    p2 = tsd_to_domain(dto)
    @test p2 isa TimeSeriesDataPackage
end

@testset "tsd_dto_to_json: pretty output is longer" begin
    p = tsd_dto_from_json(TSD_SAMPLE_JSON)
    compact = tsd_dto_to_json(p)
    pretty = tsd_dto_to_json(p, true)
    @test length(pretty) > length(compact)
end

@testset "TsdDtoHeaderRef: set_ship_id round-trip" begin
    p = tsd_dto_from_json(TSD_SAMPLE_JSON)
    h = header(pkg(p))
    @test h !== nothing
    set_ship_id!(h, "IMO9999999")
    @test ship_id(h) == "IMO9999999"
end
