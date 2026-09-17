using Test
using VistaSdk

@testset "ParsingErrors" begin
    @testset "empty collection" begin
        errors = ParsingErrors()
        @test length(errors) == 0
        @test !has_errors(errors)
        @test !has_error_type(errors, "NamingRule")
    end

    @testset "iteration on empty" begin
        errors = ParsingErrors()
        @test collect(errors) == []
    end

    @testset "bounds error on empty" begin
        errors = ParsingErrors()
        @test_throws BoundsError errors[1]
    end

    @testset "show on empty" begin
        errors = ParsingErrors()
        s = string(errors)
        @test s isa String
    end
end
