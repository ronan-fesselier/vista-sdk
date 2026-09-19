using Test
using VistaSdk

@testset "Gmod" begin
    v = vis()
    g = gmod(v, V3_4a)

    @testset "version matches requested version" begin
        @test version(g) == V3_4a
    end

    @testset "root node has code VE" begin
        @test code(root_node(g)) == "VE"
    end

    @testset "get_node known code returns matching node" begin
        n = get_node(g, "411.1")
        @test code(n) == "411.1"
    end

    @testset "get_node unknown code throws" begin
        @test_throws VistaError get_node(g, "not-a-code")
    end

    @testset "node_count is positive" begin
        @test node_count(g) > 0
    end

    @testset "iterate covers all nodes including root" begin
        count = node_count(g)
        found_root = any(n -> code(n) == "VE", g)
        @test found_root
        @test sum(1 for _ in g) == count
    end

    @testset "node_at out of bounds returns nothing" begin
        @test node_at(g, node_count(g) + 1) === nothing
    end

    @testset "length equals node_count" begin
        @test length(g) == node_count(g)
    end
end
