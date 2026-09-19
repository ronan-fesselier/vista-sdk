using VistaSdk

println("=== VistaSdk.jl GmodTraversal Sample ===\n")

let
    println("1. Gmod::traverse: counting all nodes depth-first")
    println("--------------------------------------------------")

    v = vis()
    g = gmod(v, latest(v))

    total = Ref(0)
    traverse(g, (_, _node) -> begin
        total[] += 1
        TraversalContinue
    end)
    println("Total nodes visited (DFS): $(total[])\n")
end

let
    println("2. Gmod::traverse: collecting nodes at depth 2")
    println("------------------------------------------------")

    v = vis()
    g = gmod(v, latest(v))

    depth2 = String[]
    traverse(g, (parents, node) -> begin
        length(parents) == 2 && push!(depth2, code(node))
        TraversalContinue
    end)
    println("Nodes at depth 2: $(length(depth2))")
    for c in Iterators.take(depth2, 5)
        println("  $c")
    end
    length(depth2) > 5 && println("  ... and $(length(depth2) - 5) more")
    println()
end

let
    println("3. Gmod::traverse: stopping early after 10 nodes")
    println("-------------------------------------------------")

    v = vis()
    g = gmod(v, latest(v))

    visited = String[]
    completed = traverse(
        g,
        (_, node) -> begin
            push!(visited, code(node))
            length(visited) >= 10 ? TraversalStop : TraversalContinue
        end,
    )
    println("Traversal completed: $completed")
    println("First 10 nodes in DFS order:")
    for c in visited
        println("  $c")
    end
    println()
end

let
    println("4. Gmod::traverse: skipping a subtree")
    println("---------------------------------------")

    v = vis()
    g = gmod(v, latest(v))

    skipped_code = Ref("")
    first_child = Ref(true)
    n_visited = Ref(0)
    traverse(g, (parents, node) -> begin
        if length(parents) == 1 && first_child[]
            first_child[] = false
            skipped_code[] = code(node)
            return TraversalSkipSubtree
        end
        n_visited[] += 1
        TraversalContinue
    end)
    println("Skipped subtree rooted at: $(skipped_code[])")
    println("Nodes visited after skip : $(n_visited[])\n")
end

let
    println("5. Gmod::traverse: accumulating state")
    println("---------------------------------------")

    v = vis()
    g = gmod(v, latest(v))

    leaf_count = Ref(0)
    max_depth = Ref(0)
    traverse(g, (parents, node) -> begin
        is_leaf_node(node) && (leaf_count[] += 1)
        max_depth[] = max(max_depth[], length(parents))
        TraversalContinue
    end)
    println("Leaf nodes visited: $(leaf_count[])")
    println("Max path depth    : $(max_depth[])\n")
end
