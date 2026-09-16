using Test
using VistaSdk

@testset "CodebookName" begin
    @testset "variants are sequential" begin
        @test Int(VistaSdk.Quantity) == 1
        @test Int(VistaSdk.Content) == 2
        @test Int(VistaSdk.Calculation) == 3
        @test Int(VistaSdk.State) == 4
        @test Int(VistaSdk.Command) == 5
        @test Int(VistaSdk.Type) == 6
        @test Int(VistaSdk.FunctionalServices) == 7
        @test Int(VistaSdk.MaintenanceCategory) == 8
        @test Int(VistaSdk.ActivityType) == 9
        @test Int(VistaSdk.Position) == 10
        @test Int(VistaSdk.Detail) == 11
    end

    @testset "from_prefix valid" begin
        @test codebook_name_from_prefix("qty") == VistaSdk.Quantity
        @test codebook_name_from_prefix("cnt") == VistaSdk.Content
        @test codebook_name_from_prefix("calc") == VistaSdk.Calculation
        @test codebook_name_from_prefix("state") == VistaSdk.State
        @test codebook_name_from_prefix("cmd") == VistaSdk.Command
        @test codebook_name_from_prefix("type") == VistaSdk.Type
        @test codebook_name_from_prefix("funct.svc") == VistaSdk.FunctionalServices
        @test codebook_name_from_prefix("maint.cat") == VistaSdk.MaintenanceCategory
        @test codebook_name_from_prefix("act.type") == VistaSdk.ActivityType
        @test codebook_name_from_prefix("pos") == VistaSdk.Position
        @test codebook_name_from_prefix("detail") == VistaSdk.Detail
    end

    @testset "from_prefix invalid returns nothing" begin
        @test codebook_name_from_prefix("invalid") === nothing
        @test codebook_name_from_prefix("") === nothing
    end

    @testset "from_prefix case sensitive" begin
        @test codebook_name_from_prefix("QTY") === nothing
        @test codebook_name_from_prefix("Qty") === nothing
        @test codebook_name_from_prefix("CNT") === nothing
    end

    @testset "from_prefix whitespace not trimmed" begin
        @test codebook_name_from_prefix(" qty") === nothing
        @test codebook_name_from_prefix("qty ") === nothing
    end

    @testset "to_prefix valid" begin
        @test codebook_name_to_prefix(VistaSdk.Quantity) == "qty"
        @test codebook_name_to_prefix(VistaSdk.Content) == "cnt"
        @test codebook_name_to_prefix(VistaSdk.Calculation) == "calc"
        @test codebook_name_to_prefix(VistaSdk.State) == "state"
        @test codebook_name_to_prefix(VistaSdk.Command) == "cmd"
        @test codebook_name_to_prefix(VistaSdk.Type) == "type"
        @test codebook_name_to_prefix(VistaSdk.FunctionalServices) == "funct.svc"
        @test codebook_name_to_prefix(VistaSdk.MaintenanceCategory) == "maint.cat"
        @test codebook_name_to_prefix(VistaSdk.ActivityType) == "act.type"
        @test codebook_name_to_prefix(VistaSdk.Position) == "pos"
        @test codebook_name_to_prefix(VistaSdk.Detail) == "detail"
    end

    @testset "roundtrip prefix" begin
        for name in instances(CodebookName)
            @test codebook_name_from_prefix(codebook_name_to_prefix(name)) == name
        end
    end
end
