using VistaSdk

println("=== VistaSdk.jl VIS Versioning Sample ===\n")

let
    println("1. Vis: Available versions")
    println("----------------------------")

    v = vis()
    println("Latest VIS version: $(string(latest(v)))")
    println("\nAll available versions:")
    for ver in versions(v)
        println("  - $(string(ver))")
    end
    println()
end

let
    println("2. Vis::convert_node: Converting individual Gmod nodes")
    println("-------------------------------------------------------")

    v = vis()
    source_gmod = gmod(v, V3_4a)

    node1 = get_node(source_gmod, "323.5")
    target1 = convert_node(v, V3_4a, node1, V3_6a)
    println("Converting node '323.5' from v3.4a to v3.6a:")
    println("  Source: $(code(node1)) ($(name(metadata(node1))))")
    println("  Target: $(code(target1)) ($(name(metadata(target1))))")

    node2 = get_node(source_gmod, "412.72")
    target2 = convert_node(v, V3_4a, node2, latest(v))
    println("\nConverting node '412.72' from v3.4a to $(string(latest(v))):")
    println("  Source: $(code(node2)) ($(name(metadata(node2))))")
    println("  Target: $(code(target2)) ($(name(metadata(target2))))")

    node3 = get_node(source_gmod, "1014.211")
    target3 = convert_node(v, V3_4a, node3, V3_6a)
    println("\nConverting node '1014.211' from v3.4a to v3.6a:")
    println("  Source: $(code(node3))")
    println("  Target: $(code(target3)) (unchanged)")
    println()
end

let
    println("3. Vis::convert_path: Converting Gmod paths (simple code changes)")
    println("-------------------------------------------------------------------")

    v = vis()
    src_gmod = gmod(v, V3_4a)
    src_locs = locations(v, V3_4a)

    path1 = from_short_path("323.51/H362.1", src_gmod, src_locs)
    if path1 !== nothing
        target1 = convert_path(v, V3_4a, path1, V3_6a)
        println("Converting path '323.51/H362.1' from v3.4a to v3.6a:")
        println("  Source: $(string(path1))")
        println("  Target: $(string(target1))")
    end

    path2 = from_short_path("511.11/C101.663i/C663.5/CS6d", src_gmod, src_locs)
    if path2 !== nothing
        target2 = convert_path(v, V3_4a, path2, V3_6a)
        println("\nConverting path '511.11/C101.663i/C663.5/CS6d' from v3.4a to v3.6a:")
        println("  Source: $(string(path2))")
        println("  Target: $(string(target2))")
    end
    println()
end

let
    println("4. Vis::convert_path: Converting paths with depth changes")
    println("-----------------------------------------------------------")

    v = vis()
    src_gmod = gmod(v, V3_4a)
    tgt_gmod = gmod(v, V3_6a)
    src_locs = locations(v, V3_4a)
    tgt_locs = locations(v, V3_6a)

    path = from_short_path("511.331/C221", src_gmod, src_locs)
    if path !== nothing
        println("Converting path '511.331/C221' from v3.4a to v3.6a:")
        println("  Source: $(string(path)) (depth: $(length(path)))")
        target = convert_path(v, V3_4a, path, V3_6a)
        println("  Target: $(string(target)) (depth: $(length(target)))")
        println("  Note: Depth changed from $(length(path)) to $(length(target)) nodes")
        expected = from_short_path("511.31/C121.31/C221", tgt_gmod, tgt_locs)
        if expected !== nothing
            if target == expected
                println("  OK: Conversion matches expected path")
            else
                println("  FAILED: Expected $(string(expected)), got $(string(target))")
            end
        end
    end
    println()
end

let
    println("5. Vis::convert_path: Converting paths with locations")
    println("-------------------------------------------------------")

    v = vis()
    src_gmod = gmod(v, V3_7a)
    src_locs = locations(v, V3_7a)

    path = from_short_path("691.811i-A/H101.11-1", src_gmod, src_locs)
    if path !== nothing
        println("Converting path '691.811i-A/H101.11-1' from v3.7a to v3.9a:")
        println("  Source: $(string(path))")
        println("  Source has location: $(location(node(path)) !== nothing)")
        target = convert_path(v, V3_7a, path, V3_9a)
        println("  Target: $(string(target))")
        println("  Target has location: $(location(node(target)) !== nothing)")
        loc = location(node(target))
        if loc !== nothing
            println("  OK: Location preserved: $(string(loc))")
        else
            println("  FAILED: Location not preserved in target path")
        end
    end
    println()
end

let
    println("6. Vis::convert_path: Converting to latest version")
    println("----------------------------------------------------")

    v = vis()
    ver = latest(v)
    src_gmod = gmod(v, V3_4a)
    src_locs = locations(v, V3_4a)

    path_strings = [
        "411.1/C101.72/I101",
        "1012.21/C1147.221/C1051.7/C101.22",
        "632.32i/S110.2/C111.42/G203.31/S90.5/C401",
    ]

    println("Converting paths from v3.4a to latest ($(string(ver))):")
    for s in path_strings
        path = from_short_path(s, src_gmod, src_locs)
        if path !== nothing
            try
                target = convert_path(v, V3_4a, path, ver)
                println("  $(rpad(s, 45)) -> $(string(target))")
            catch
                println("  $(rpad(s, 45)) -> FAILED")
            end
        end
    end
    println()
end

let
    println("7. Vis::convert_local_id: Converting LocalIds between versions")
    println("---------------------------------------------------------------")

    v = vis()

    s1 = "/dnv-v2/vis-3-4a/411.1/C101/sec/411.1/C101.64i/S201/meta/cnt-condensate"
    lid1 = from_string(LocalId, s1)
    if lid1 !== nothing
        println("Source LocalId (v3.4a):")
        println("  $(string(lid1))")
        target1 = convert_local_id(v, lid1, V3_5a)
        println("\nTarget LocalId (v3.5a):")
        println("  $(string(target1))")
        println("\n  Note: 'C101.64i' converted to 'C101.64'")
    end

    s2 = "/dnv-v2/vis-3-4a/411.1/C101.64i-1/S201.1/C151.2/S110/meta/cnt-hydraulic.oil/state-running"
    lid2 = from_string(LocalId, s2)
    if lid2 !== nothing
        println("\nSource LocalId (v3.4a):")
        println("  $(string(lid2))")
        target2 = convert_local_id(v, lid2, V3_9a)
        println("\nTarget LocalId (v3.9a):")
        println("  $(string(target2))")
    end
    println()
end

let
    println("8. Vis: Loading multiple versions at once")
    println("------------------------------------------")

    v = vis()
    vers = versions(v)
    println(
        "Loading Gmods for all versions ($(string(first(vers))) -> $(string(last(vers)))):",
    )
    println("\nUnique node count per version:")
    for ver in vers
        g = gmod(v, ver)
        println("  v$(string(ver)): $(length(g)) nodes")
    end
    println()
end

let
    println("9. Vis::convert_path: Chained conversion through all versions")
    println("---------------------------------------------------------------")

    v = vis()
    vers = versions(v)
    start_str = "511.36/I101"
    start_ver = V3_4a

    println("Tracing '$start_str' from $(string(start_ver)) through all versions:\n")

    src_gmod = gmod(v, start_ver)
    src_locs = locations(v, start_ver)
    current = from_short_path(start_str, src_gmod, src_locs)

    if current !== nothing
        col = maximum(length(string(ver)) for ver in vers)
        println("  $(rpad(string(start_ver), col)): $(string(current))")
        for i = 2:length(vers)
            src_ver = vers[i-1]
            tgt_ver = vers[i]
            print("  $(rpad(string(tgt_ver), col)): ")
            try
                current = convert_path(v, src_ver, current, tgt_ver)
                println(string(current))
            catch
                println("(conversion failed)")
                break
            end
        end
    else
        println("  ERROR: Failed to parse source path")
    end
    println()
end
