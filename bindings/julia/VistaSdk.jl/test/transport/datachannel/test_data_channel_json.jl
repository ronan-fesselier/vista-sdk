@testset "dcl_dto_from_json: round-trip preserves ship_id" begin
    p = dcl_dto_from_json(DCL_SAMPLE_JSON)
    json_out = dcl_dto_to_json(p)
    p2 = dcl_dto_from_json(json_out)
    @test ship_id(header(pkg(p2))) == "IMO1234567"
end

@testset "dcl_dto_from_json: round-trip preserves channel count" begin
    p = dcl_dto_from_json(DCL_SAMPLE_JSON)
    json_out = dcl_dto_to_json(p)
    p2 = dcl_dto_from_json(json_out)
    @test length(channel_list(pkg(p2))) == 14
end

@testset "dcl_dto_to_json: pretty output is longer" begin
    p = dcl_dto_from_json(DCL_SAMPLE_JSON)
    compact = dcl_dto_to_json(p)
    pretty = dcl_dto_to_json(p, true)
    @test length(pretty) > length(compact)
end

@testset "dcl_dto_from_json: invalid JSON throws VistaError" begin
    @test_throws VistaError dcl_dto_from_json("not json")
end
