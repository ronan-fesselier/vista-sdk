using Test
using VistaSdk

@testset "GmodNodeMetadata" begin
    v = vis()
    g = gmod(v, V3_11a)

    @testset "root metadata category and type are non-empty" begin
        meta = metadata(root_node(g))
        @test !isempty(category(meta))
        @test !isempty(node_type(meta))
    end

    @testset "full_type contains category and type" begin
        meta = metadata(root_node(g))
        full = full_type(meta)
        @test occursin(category(meta), full)
        @test occursin(node_type(meta), full)
    end

    @testset "common_name does not throw" begin
        meta = metadata(get_node(g, "411.1"))
        @test common_name(meta) isa Union{String,Nothing}
    end

    @testset "common_definition does not throw" begin
        meta = metadata(get_node(g, "411.1"))
        @test common_definition(meta) isa Union{String,Nothing}
    end

    @testset "install_substructure does not throw" begin
        meta = metadata(get_node(g, "411.1"))
        @test install_substructure(meta) isa Union{Bool,Nothing}
    end

    @testset "normal_assignment_names count consistent with at" begin
        meta = metadata(get_node(g, "411.1"))
        count = normal_assignment_name_count(meta)
        for i = 1:count
            k, v2 = normal_assignment_name_at(meta, i)
            @test !isempty(k)
            @test !isempty(v2)
        end
        @test normal_assignment_name_at(meta, count + 1) === nothing
    end

    @testset "normal_assignment_names iterator matches count" begin
        meta = metadata(get_node(g, "411.1"))
        @test length(collect(normal_assignment_names(meta))) ==
              normal_assignment_name_count(meta)
    end

    @testset "all nodes have non-empty category" begin
        for n in g
            meta = metadata(n)
            @test !isempty(category(meta))
        end
    end
end
