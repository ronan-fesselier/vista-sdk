const DCL_SAMPLE_JSON = read(
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
        "DataChannelList.sample.compact.json",
    ),
    String,
)

@testset "DclDtoPackage: parse from JSON" begin
    p = dcl_dto_from_json(DCL_SAMPLE_JSON)
    @test p isa DclDtoPackage
end

@testset "DclDtoPackage: invalid JSON throws" begin
    @test_throws VistaError dcl_dto_from_json("{invalid}")
end

@testset "DclDtoHeaderRef: ship_id" begin
    p = dcl_dto_from_json(DCL_SAMPLE_JSON)
    h = header(pkg(p))
    @test ship_id(h) == "IMO1234567"
end

@testset "DclDtoHeaderRef: author" begin
    p = dcl_dto_from_json(DCL_SAMPLE_JSON)
    h = header(pkg(p))
    @test author(h) == "DNV"
end

@testset "DclDtoHeaderRef: date_created" begin
    p = dcl_dto_from_json(DCL_SAMPLE_JSON)
    h = header(pkg(p))
    @test date_created(h) == "2022-04-04T20:44:31Z"
end

@testset "DclDtoCfgRefRef: id and timestamp" begin
    p = dcl_dto_from_json(DCL_SAMPLE_JSON)
    cr = cfg_ref(header(pkg(p)))
    @test cfg_id(cr) == "export-IMO1234567"
    @test timestamp(cr) == "2022-04-04T20:44:31Z"
end

@testset "DclDtoVersionInfoRef: naming_rule and naming_scheme_version" begin
    p = dcl_dto_from_json(DCL_SAMPLE_JSON)
    vi = version_info(header(pkg(p)))
    @test vi !== nothing
    @test naming_rule(vi) == "dnv"
    @test naming_scheme_version(vi) == "v2"
end

@testset "DclDtoChannelListRef: length" begin
    p = dcl_dto_from_json(DCL_SAMPLE_JSON)
    cl = channel_list(pkg(p))
    @test length(cl) == 14
end

@testset "DclDtoChannelRef: first channel local_id" begin
    p = dcl_dto_from_json(DCL_SAMPLE_JSON)
    cl = channel_list(pkg(p))
    ch = cl[1]
    cid = channel_id(ch)
    @test startswith(local_id(cid), "/dnv-v2/vis-3-4a/")
end

@testset "DclDtoChannelRef: first channel has short_id" begin
    p = dcl_dto_from_json(DCL_SAMPLE_JSON)
    cl = channel_list(pkg(p))
    ch = cl[1]
    cid = channel_id(ch)
    @test short_id(cid) !== nothing
end

@testset "DclDtoPropertyRef: format type" begin
    p = dcl_dto_from_json(DCL_SAMPLE_JSON)
    cl = channel_list(pkg(p))
    prop = property(cl[1])
    @test format_type(format(prop)) == "Decimal"
end

@testset "DclDtoPropertyRef: channel type" begin
    p = dcl_dto_from_json(DCL_SAMPLE_JSON)
    cl = channel_list(pkg(p))
    prop = property(cl[1])
    ct = channel_type(prop)
    @test channel_type(ct) == "Inst"
end

@testset "DclDtoPropertyRef: range" begin
    p = dcl_dto_from_json(DCL_SAMPLE_JSON)
    cl = channel_list(pkg(p))
    prop = property(cl[1])
    r = dcl_range(prop)
    @test r !== nothing
    @test r.low == 0.0
    @test r.high == 100.0
end

@testset "DclDtoPropertyRef: unit symbol" begin
    p = dcl_dto_from_json(DCL_SAMPLE_JSON)
    cl = channel_list(pkg(p))
    prop = property(cl[1])
    u = unit(prop)
    @test u !== nothing
    @test symbol(u) == "% LEL"
end

@testset "DclDtoChannelListRef: iterate" begin
    p = dcl_dto_from_json(DCL_SAMPLE_JSON)
    cl = channel_list(pkg(p))
    count = 0
    for ch in cl
        count += 1
    end
    @test count == length(cl)
end
