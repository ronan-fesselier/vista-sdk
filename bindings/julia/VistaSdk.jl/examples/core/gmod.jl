using VistaSdk

println("=== VistaSdk.jl Gmod Sample ===\n")

let
    println("1. Gmod: accessing Gmod for a VIS version")
    println("-------------------------------------------------------")

    v = vis()
    ver = latest(v)
    g = gmod(v, ver)

    println("Gmod for version   : $ver")
    println("Total nodes in Gmod: $(length(g))")
    println("Root node code     : $(code(root_node(g)))")
    println()
end

let
    println("2. GmodNode: accessing nodes by code")
    println("-------------------------------------------------------")

    v = vis()
    g = gmod(v, latest(v))

    node1 = get_node(g, "411.1")
    println("Node code    : $(code(node1))")
    println("Node name    : $(name(metadata(node1)))")
    println("Node category: $(category(metadata(node1)))")
    println("Node type    : $(node_type(metadata(node1)))")

    node2 = get_node(g, "C101.31")
    println("\nFound node: $(code(node2))")
    println("Name      : $(name(metadata(node2)))")

    try
        get_node(g, "INVALID")
        println("\nInvalid code lookup: true")
    catch
        println("\nInvalid code lookup: false")
    end
    println()
end

let
    println("3. GmodNode: navigating the tree hierarchy")
    println("-------------------------------------------------------")

    v = vis()
    g = gmod(v, latest(v))
    node = get_node(g, "411.1")

    ps = parents(node)
    if !isempty(ps)
        println("Parents of $(code(node)) ($(parent_count(node))):")
        for p in ps
            println("  - $(code(p))")
        end
    end

    max_display = 5
    nchildren = child_count(node)
    println("\nChildren of $(code(node)) ($nchildren):")
    for (i, child) in enumerate(children(node))
        if i > max_display
            println("  ... and $(nchildren - max_display) more")
            break
        end
        println("  - $(code(child)) ($(name(metadata(child))))")
    end

    println("\nIs root  ? $(is_root(node))")
    println("Root node: $(code(root_node(g)))")
    println("Is root  ? $(is_root(root_node(g)))")
    println()
end

let
    println("4. GmodNode: node metadata")
    println("-------------------------------------------------------")

    v = vis()
    g = gmod(v, latest(v))
    node = get_node(g, "S222.11")
    meta = metadata(node)

    println("Node                : $(code(node))")
    println("Category            : $(category(meta))")
    println("Type                : $(node_type(meta))")
    println("Name                : $(name(meta))")

    def = common_definition(meta)
    def !== nothing && println("Definition          : $def")

    inst = install_substructure(meta)
    inst !== nothing && println("Install substructure: $inst")
    println()
end

let
    println("5. GmodNode: product types and selections")
    println("-------------------------------------------------------")

    v = vis()
    g = gmod(v, latest(v))

    node5a = get_node(g, "411.3")
    println("Node             : $(code(node5a))")
    println("Has product type : $(product_type(node5a) !== nothing)")
    pt = product_type(node5a)
    if pt !== nothing
        println("Product type code: $(code(pt))")
        println("Product type name: $(name(metadata(pt)))")
    end

    node5b = get_node(g, "411.2")
    println("\nNode: $(code(node5b))")
    println("Has product selection : $(product_selection(node5b) !== nothing)")
    ps = product_selection(node5b)
    if ps !== nothing
        println("Product selection code: $(code(ps))")
        println("Product selection name: $(name(metadata(ps)))")
    end

    cs = get_node(g, "CS1")
    println("\nNode: $(code(cs))")
    println("Is product selection: $(is_product_selection(cs))")
    println()
end

let
    println("6. GmodNode: mappability")
    println("-------------------------------------------------------")

    v = vis()
    g = gmod(v, latest(v))
    test_codes = ["VE", "411", "411.1", "C101", "C101.31", "CS1"]

    for c in test_codes
        try
            n = get_node(g, c)
            println("  $(rpad(c, 7)): $(is_mappable(n))")
        catch
        end
    end
    println()
end

let
    println("7. Gmod: iterating all nodes")
    println("-------------------------------------------------------")

    v = vis()
    g = gmod(v, latest(v))
    println("First 10 nodes:")
    for (i, n) in enumerate(g)
        i > 10 && break
        println("  $(rpad(code(n), 8)) - $(name(metadata(n)))")
    end
    println()
end

let
    println("8. GmodNode: finding specific node types")
    println("-------------------------------------------------------")

    v = vis()
    g = gmod(v, latest(v))

    println("Sample PRODUCT TYPE nodes:")
    let n_found = 0
        for n in g
            meta = metadata(n)
            if category(meta) == "PRODUCT" && node_type(meta) == "TYPE"
                n_found += 1
                n_found > 5 && break
                println("  $(rpad(code(n), 10)) - $(name(meta))")
            end
        end
    end

    println("\nSample ASSET FUNCTION LEAF nodes:")
    let n_found = 0
        for n in g
            if is_leaf_node(n) && category(metadata(n)) == "ASSET FUNCTION"
                n_found += 1
                n_found > 5 && break
                println("  $(rpad(code(n), 10)) - $(name(metadata(n)))")
            end
        end
    end
    println()
end

let
    println("9. GmodNode: tree navigation example")
    println("-------------------------------------------------------")

    v = vis()
    g = gmod(v, latest(v))
    node = get_node(g, "C101.31")
    println("Path from $(code(node)) to root:")

    let current = node, depth = 0, visited = Set{String}()
        while true
            c = code(current)
            if c in visited
                println("$(" "^(depth*2))- [cycle detected at $c]")
                break
            end
            push!(visited, c)
            println("$(" "^(depth*2))- $c ($(name(metadata(current))))")
            p = parent_at(current, 1)
            p === nothing && break
            current = p
            depth += 1
        end
    end
    println()
end

let
    println("10. GmodNode: node properties comparison")
    println("-------------------------------------------------------")

    v = vis()
    g = gmod(v, latest(v))
    codes = ["411.1", "C101", "CS1", "F201"]

    println(
        "$(rpad("Code", 12))$(rpad("Category", 20))$(rpad("Type", 15))$(rpad("Mappable", 12))IsLeaf",
    )
    println("-"^71)

    for c in codes
        try
            n = get_node(g, c)
            meta = metadata(n)
            println(
                "$(rpad(c, 12))$(rpad(category(meta), 20))$(rpad(node_type(meta), 15))$(rpad(string(is_mappable(n)), 12))$(is_leaf_node(n))",
            )
        catch
        end
    end
    println()
end

let
    println("11. Gmod: working with different versions")
    println("-------------------------------------------------------")

    v = vis()
    println("Node counts across VIS versions:")
    for ver in versions(v)
        g = gmod(v, ver)
        println("  $(rpad(string(ver), 5)): $(lpad(length(g), 4)) nodes")
    end
    println()
end
