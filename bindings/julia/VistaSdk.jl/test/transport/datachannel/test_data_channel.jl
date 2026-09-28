const VALID_LOCAL_ID_STR = "/dnv-v2/vis-3-4a/411.1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet"

@testset "DclRestriction: fresh has no fields set" begin
    r = DclRestriction()
    @test fraction_digits(r) === nothing
    @test length_(r) === nothing
    @test enumeration_count(r) == 0
end

@testset "DclRestriction: enumeration round-trip" begin
    r = DclRestriction()
    set_enumeration!(r, ["A", "B", "C"])
    @test enumeration_count(r) == 3
    @test enumeration_at(r, 1) == "A"
    @test enumeration_at(r, 3) == "C"
end

@testset "DclRestriction: total_digits zero throws" begin
    r = DclRestriction()
    @test_throws VistaError set_total_digits!(r, 0)
end

@testset "DclRestriction: white_space round-trip" begin
    r = DclRestriction()
    set_white_space!(r, Collapse)
    @test white_space(r) == Collapse
end

@testset "DclRange: valid bounds" begin
    r = DclRange(0.0, 100.0)
    @test low(r) ≈ 0.0
    @test high(r) ≈ 100.0
end

@testset "DclRange: low >= high throws" begin
    @test_throws VistaError DclRange(100.0, 0.0)
end

@testset "DclRange: set_low > high throws" begin
    r = DclRange(0.0, 10.0)
    @test_throws VistaError set_low!(r, 20.0)
end

@testset "DclRange: low == high is allowed" begin
    r = DclRange(0.0, 10.0)
    set_low!(r, 10.0)
    @test low(r) ≈ 10.0
end

@testset "DclFormat: type round-trip" begin
    f = DclFormat("Decimal")
    @test type_(f) == "Decimal"
end

@testset "DclFormat: restriction round-trip" begin
    r = DclRestriction()
    set_total_digits!(r, 5)
    f = DclFormat("Decimal")
    set_restriction!(f, r)
    stored = restriction(f)
    @test stored !== nothing
    @test total_digits(stored) == 5
end

@testset "DclFormat: validate_value" begin
    f = DclFormat("Decimal")
    @test validate_value(f, "42.5")
end

@testset "DclDataChannelType: is_alert" begin
    dct = DclDataChannelType("Alert")
    @test is_alert(dct)
end

@testset "DclNameObject: default naming_rule" begin
    no = DclNameObject()
    @test naming_rule(no) == "/dnv-v2"
end

@testset "DclNameObject: custom_name_objects ownership transfer" begin
    doc = SerializableDocument("custom")
    no = DclNameObject()
    set_custom_name_objects!(no, doc)
    stored = custom_name_objects(no)
    @test stored !== nothing
    @test as_string(stored) == "custom"
end

@testset "DclUnit: unit_symbol round-trip" begin
    u = DclUnit("Cel")
    @test unit_symbol(u) == "Cel"
    @test quantity_name(u) === nothing
end

@testset "DclProperty: Decimal without range/unit fails validate" begin
    dct = DclDataChannelType("Inst")
    fmt = DclFormat("Decimal")
    p = DclProperty(dct, fmt)
    @test !validate(p)
end

@testset "DclProperty: Decimal with range and unit validates" begin
    dct = DclDataChannelType("Inst")
    fmt = DclFormat("Decimal")
    r = DclRange(0.0, 100.0)
    u = DclUnit("Cel")
    p = DclProperty(dct, fmt)
    set_range!(p, r)
    set_unit!(p, u)
    @test validate(p)
end

@testset "DclProperty: Alert without alert_priority fails validate" begin
    dct = DclDataChannelType("Alert")
    fmt = DclFormat("Boolean")
    p = DclProperty(dct, fmt)
    @test !validate(p)
end

@testset "DclConfigurationReference: id and timestamp round-trip" begin
    ts = now(DateTimeOffset)
    cr = DclConfigurationReference("cfg-1", ts)
    @test id(cr) == "cfg-1"
    @test equals_exact(timestamp(cr), ts)
end

@testset "DclConfigurationReference: version round-trip" begin
    ts = now(DateTimeOffset)
    cr = DclConfigurationReference("id-1", ts)
    set_version!(cr, "v1")
    @test version(cr) == "v1"
    clear_version!(cr)
    @test version(cr) === nothing
end

@testset "DclVersionInformation: default" begin
    vi = DclVersionInformation()
    @test !isempty(naming_rule(vi))
end

@testset "DclVersionInformation: fields round-trip" begin
    vi = DclVersionInformation("custom-rule", "v3")
    @test naming_rule(vi) == "custom-rule"
    @test naming_scheme_version(vi) == "v3"
end

@testset "DclVersionInformation: reference_url" begin
    vi = DclVersionInformation("dnv", "v2")
    set_reference_url!(vi, "https://example.com")
    @test reference_url(vi) == "https://example.com"
    clear_reference_url!(vi)
    @test reference_url(vi) === nothing
end

@testset "DclDataChannel: valid property succeeds" begin
    local_id = from_string(LocalId, VALID_LOCAL_ID_STR)
    cid = DclDataChannelId(local_id)
    dct = DclDataChannelType("Inst")
    fmt = DclFormat("Boolean")
    p = DclProperty(dct, fmt)
    dc = DclDataChannel(cid, p)
    @test dc isa DclDataChannel
end

@testset "DclDataChannel: Decimal without range/unit throws" begin
    local_id = from_string(LocalId, VALID_LOCAL_ID_STR)
    cid = DclDataChannelId(local_id)
    dct = DclDataChannelType("Inst")
    fmt = DclFormat("Decimal")
    p = DclProperty(dct, fmt)
    @test_throws VistaError DclDataChannel(cid, p)
end

@testset "DclDataChannelList: empty list" begin
    l = DclDataChannelList()
    @test length(l) == 0
end

@testset "DclDataChannelList: add / at / from_short_id / from_local_id" begin
    local_id = from_string(LocalId, VALID_LOCAL_ID_STR)
    cid = DclDataChannelId(local_id)
    set_short_id!(cid, "SID-1")
    dct = DclDataChannelType("Inst")
    fmt = DclFormat("Boolean")
    p = DclProperty(dct, fmt)
    dc = DclDataChannel(cid, p)

    l = DclDataChannelList()
    @test add!(l, dc)
    @test length(l) == 1
    @test l[1] isa DclDataChannelRef
    @test from_short_id(l, "SID-1") !== nothing
    @test from_local_id(l, local_id) !== nothing
    @test from_short_id(l, "missing") === nothing
end

@testset "DclDataChannelList: duplicate local_id fails" begin
    local_id = from_string(LocalId, VALID_LOCAL_ID_STR)
    cid = DclDataChannelId(local_id)
    dct = DclDataChannelType("Inst")
    fmt = DclFormat("Boolean")
    p = DclProperty(dct, fmt)
    dc = DclDataChannel(cid, p)

    l = DclDataChannelList()
    @test add!(l, dc)
    @test !add!(l, dc)
end

@testset "DclDataChannelList: remove and empty!" begin
    local_id = from_string(LocalId, VALID_LOCAL_ID_STR)
    cid = DclDataChannelId(local_id)
    dct = DclDataChannelType("Inst")
    fmt = DclFormat("Boolean")
    p = DclProperty(dct, fmt)
    dc = DclDataChannel(cid, p)

    l = DclDataChannelList()
    add!(l, dc)
    @test length(l) == 1
    @test remove!(l, dc)
    @test length(l) == 0
    add!(l, dc)
    empty!(l)
    @test length(l) == 0
end

function _make_header()
    imo = ImoNumber(9074729)
    sid = from_imo_number(imo)
    ts = now(DateTimeOffset)
    cr = DclConfigurationReference("cfg-1", ts)
    DclHeader(sid, cr)
end

@testset "DclPackage: header and data_channel_list round-trip" begin
    h = _make_header()
    l = DclDataChannelList()
    pkg = DclPackage(h, l)
    @test pkg isa DclPackage
    @test data_channel_list(pkg) !== nothing
end

@testset "DclListPackage: convenience accessor" begin
    h = _make_header()
    l = DclDataChannelList()
    pkg = DclPackage(h, l)
    lp = DclListPackage(pkg)
    @test lp isa DclListPackage
    @test data_channel_list(lp) !== nothing
end

@testset "DclHeader: author" begin
    h = _make_header()
    set_author!(h, "Alice")
    @test author(h) == "Alice"
    clear_author!(h)
    @test author(h) === nothing
end

@testset "DclHeader: date_created" begin
    h = _make_header()
    ts = now(DateTimeOffset)
    set_date_created!(h, ts)
    @test date_created(h) !== nothing
    clear_date_created!(h)
    @test date_created(h) === nothing
end

@testset "DclHeader: version_information" begin
    h = _make_header()
    vi = DclVersionInformation("dnv", "v2")
    set_version_information!(h, vi)
    @test version_information(h) !== nothing
    clear_version_information!(h)
    @test version_information(h) === nothing
end

@testset "DclHeader: custom_headers ownership transfer" begin
    h = _make_header()
    set_custom_headers!(h, sd_object())
    @test custom_headers(h) !== nothing
    clear_custom_headers!(h)
    @test custom_headers(h) === nothing
end

@testset "DclDataChannelId: short_id" begin
    local_id = from_string(LocalId, VALID_LOCAL_ID_STR)
    cid = DclDataChannelId(local_id)
    set_short_id!(cid, "S1")
    @test short_id(cid) == "S1"
    clear_short_id!(cid)
    @test short_id(cid) === nothing
end

@testset "DclDataChannelId: local_id_string" begin
    local_id = from_string(LocalId, VALID_LOCAL_ID_STR)
    cid = DclDataChannelId(local_id)
    @test local_id_string(cid) !== nothing
end

@testset "DclDataChannelId: name_object" begin
    local_id = from_string(LocalId, VALID_LOCAL_ID_STR)
    cid = DclDataChannelId(local_id)
    no = DclNameObject()
    set_name_object!(cid, no)
    @test name_object(cid) !== nothing
    clear_name_object!(cid)
    @test name_object(cid) === nothing
end
