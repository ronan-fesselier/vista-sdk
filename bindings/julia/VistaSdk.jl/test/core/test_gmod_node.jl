using Test
using VistaSdk

@testset "GmodNode" begin
    v = vis()
    g = gmod(v, V3_4a)

    @testset "root node properties" begin
        root = root_node(g)
        @test code(root) == "VE"
        @test version(root) == V3_4a
        @test is_root(root)
        @test location(root) === nothing
        @test parent_count(root) == 0
        @test child_count(root) > 0
    end

    @testset "children iterator matches child_at" begin
        root = root_node(g)
        from_iter = [code(n) for n in children(root)]
        from_at = [code(child_at(root, i)) for i = 1:child_count(root)]
        @test from_iter == from_at
        @test !isempty(from_iter)
    end

    @testset "parents iterator matches parent_at" begin
        n = get_node(g, "411")
        from_iter = [code(p) for p in parents(n)]
        from_at = [code(parent_at(n, i)) for i = 1:parent_count(n)]
        @test from_iter == from_at
        @test !isempty(from_iter)
    end

    @testset "is_child and is_child_code" begin
        root = root_node(g)
        child = child_at(root, 1)
        @test is_child(root, child)
        @test is_child_code(root, code(child))
        @test !is_child_code(root, "UNKNOWN")
    end

    @testset "leaf node is_leaf_node and is_mappable" begin
        n = get_node(g, "F201.11")
        @test is_leaf_node(n)
        @test is_mappable(n)
    end

    @testset "show contains code" begin
        n = get_node(g, "411.1")
        s = string(n)
        @test occursin("411.1", s)
    end
end
