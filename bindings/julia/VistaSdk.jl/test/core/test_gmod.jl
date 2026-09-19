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

    @testset "traverse visits root then stops early" begin
        visited = String[]
        completed = traverse(
            g,
            (parents, node) -> begin
                push!(visited, code(node))
                length(visited) >= 20 ? TraversalStop : TraversalContinue
            end,
        )
        @test !completed
        @test "VE" in visited
        @test length(visited) == 20
    end

    @testset "traverse skips subtree when handler returns TraversalSkipSubtree" begin
        visited_with_skip = String[]
        traverse(
            g,
            (parents, node) -> begin
                c = code(node)
                push!(visited_with_skip, c)
                c == "400a" ? TraversalSkipSubtree : TraversalContinue
            end,
        )

        @test !("410" in visited_with_skip)
        @test "VE" in visited_with_skip
        @test "500a" in visited_with_skip
    end

    @testset "traverse survives GC pressure during the callback (GC.@preserve regression)" begin
        count = 0
        completed = traverse(
            g,
            (parents, node) -> begin
                count += 1
                count % 20 == 0 && GC.gc()
                count >= 100 ? TraversalStop : TraversalContinue
            end,
        )
        @test !completed
        @test count == 100
    end
end
