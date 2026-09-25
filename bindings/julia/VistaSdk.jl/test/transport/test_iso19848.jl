@testset "Iso19848" begin
    @testset "instance is stable" begin
        a = Iso19848()
        b = Iso19848()
        @test versions(a) == versions(b)
    end

    @testset "versions non-empty and known" begin
        iso = Iso19848()
        vs = versions(iso)
        @test !isempty(vs)
        @test V2018 in vs
        @test V2024 in vs
    end

    @testset "latest is V2024" begin
        iso = Iso19848()
        @test latest(iso) == V2024
    end

    @testset "data_channel_type_names non-empty" begin
        iso = Iso19848()
        names = data_channel_type_names(iso, V2024)
        @test names !== nothing
        @test length(names) > 0
        for entry in names
            @test !isempty(entry.type_)
            @test !isempty(entry.description)
        end
    end

    @testset "data_channel_type_names find known" begin
        iso = Iso19848()
        names = data_channel_type_names(iso, V2024)
        entry = find(names, "Inst")
        @test entry !== nothing
        @test entry.type_ == "Inst"
    end

    @testset "data_channel_type_names find unknown returns nothing" begin
        iso = Iso19848()
        names = data_channel_type_names(iso, V2024)
        @test find(names, "not-a-type") === nothing
    end

    @testset "format_data_types non-empty" begin
        iso = Iso19848()
        types = format_data_types(iso, V2024)
        @test types !== nothing
        @test length(types) > 0
        for entry in types
            @test !isempty(type_(entry))
            @test !isempty(description(entry))
        end
    end

    @testset "format_data_type validate decimal valid" begin
        iso = Iso19848()
        types = format_data_types(iso, V2024)
        fdt = find(types, "Decimal")
        @test fdt !== nothing
        v = validate(fdt, "0.1")
        @test v !== nothing
        @test v isa Decimal
    end

    @testset "format_data_type validate boolean invalid returns nothing" begin
        iso = Iso19848()
        types = format_data_types(iso, V2024)
        fdt = find(types, "Boolean")
        @test fdt !== nothing
        @test validate(fdt, "yes") === nothing
    end

    @testset "value from string" begin
        v = iso19848_value_from_string("test")
        @test v !== nothing
        @test v isa String
        @test v == "test"
    end

    @testset "value from integer" begin
        v = iso19848_value_from_integer(42)
        @test v isa Int64
        @test v == 42
    end

    @testset "value from boolean" begin
        v = iso19848_value_from_boolean(true)
        @test v isa Bool
        @test v == true
    end

    @testset "value from decimal" begin
        d = parse(Decimal, "3.14")
        v = iso19848_value_from_decimal(d)
        @test v isa Decimal
    end

    @testset "value from date time" begin
        dto = DateTimeOffset(DateTime(0), TimeSpan(0))
        v = iso19848_value_from_date_time(dto)
        @test v isa DateTimeOffset
    end

    @testset "value to string integer" begin
        v = iso19848_value_from_integer(123)
        @test iso19848_value_to_string(v) == "123"
    end

    @testset "value to string string variant" begin
        v = iso19848_value_from_string("hello")
        @test iso19848_value_to_string(v) == "hello"
    end

    @testset "value wrong variant does not match" begin
        v = iso19848_value_from_integer(1)
        @test !(v isa String)
        @test !(v isa Bool)
    end
end
