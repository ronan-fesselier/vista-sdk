using VistaSdk

println("=== vista-sdk GmodSubset Sample ===\n")

mutable struct AssetNode
    path::Union{String,Nothing}
    code::String
    name::String
    common_name::Union{String,Nothing}
    display_name::String
    category::String
    type_::String
    location::Union{String,Nothing}
    depth::Int
    children::Vector{Int}
    parent::Union{Int,Nothing}
end

mutable struct AssetModel
    vis_version::VisVersion
    nodes::Vector{AssetNode}
    node_map::Dict{String,Int}
    nodes_by_code::Dict{String,Vector{Int}}
    root::Union{Int,Nothing}
end

AssetModel(vis_version::VisVersion) = AssetModel(
    vis_version,
    AssetNode[],
    Dict{String,Int}(),
    Dict{String,Vector{Int}}(),
    nothing,
)

function add_path!(model::AssetModel, path, g::Gmod, locs::Locations)
    all_nodes = collect(path)

    node_id = ""

    for (i, node) in enumerate(all_nodes)
        depth = i - 1
        prev_id = isempty(node_id) ? nothing : node_id

        if !isempty(node_id)
            node_id *= "/"
        end
        node_id *= string(node)

        haskey(model.node_map, node_id) && continue

        meta = metadata(node)

        sub_path_string, display_name = if depth == 0
            (nothing, name(meta))
        else
            sub_path = from_full_path(node_id, g, locs)
            cn = sub_path !== nothing ? common_names(sub_path) : Tuple{Int,String}[]
            dn = !isempty(cn) ? cn[end][2] : name(meta)
            path_str =
                sub_path !== nothing ? to_full_path_string(sub_path) : string(node)
            (path_str, dn)
        end

        asset_node = AssetNode(
            sub_path_string,
            code(node),
            name(meta),
            common_name(meta),
            display_name,
            category(meta),
            node_type(meta),
            location(node),
            depth,
            Int[],
            nothing,
        )

        idx = length(model.nodes) + 1
        push!(model.nodes, asset_node)
        push!(get!(model.nodes_by_code, code(node), Int[]), idx)

        if depth == 0
            model.root === nothing && (model.root = idx)
            model.node_map[node_id] = idx
            continue
        end

        if prev_id !== nothing && haskey(model.node_map, prev_id)
            parent_idx = model.node_map[prev_id]
            push!(model.nodes[parent_idx].children, idx)
            model.nodes[idx].parent = parent_idx
        end

        model.node_map[node_id] = idx
    end
end

function asset_model_from_paths(vis_version::VisVersion, paths, g::Gmod, locs::Locations)
    model = AssetModel(vis_version)
    for path in paths
        add_path!(model, path, g, locs)
    end
    model
end

function asset_model_from_path_strings(vis_version::VisVersion, path_strings)
    v = vis()
    g = gmod(v, vis_version)
    locs = locations(v, vis_version)

    paths = []
    for path_str in path_strings
        p = from_short_path(path_str, g, locs)
        if p !== nothing
            push!(paths, p)
        else
            println("  Warning: Could not parse path '$path_str'")
        end
    end
    asset_model_from_paths(vis_version, paths, g, locs)
end

nodes_by_code(model::AssetModel, c::AbstractString) = get(model.nodes_by_code, c, Int[])

node_count(model::AssetModel) = length(model.nodes)

max_depth(model::AssetModel) =
    isempty(model.nodes) ? 0 : maximum(n.depth for n in model.nodes)

function _json_escape(s::AbstractString)
    out = IOBuffer()
    print(out, '"')
    for c in s
        (c == '"' || c == '\\') && print(out, '\\')
        print(out, c)
    end
    print(out, '"')
    String(take!(out))
end

function _sorted_children_with_path(model::AssetModel, idx::Int)
    children = [ci for ci in model.nodes[idx].children if model.nodes[ci].path !== nothing]
    sort!(children; by = ci -> something(model.nodes[ci].path, ""))
    children
end

function _node_to_json(model::AssetModel, idx::Int, indent::Int)
    node = model.nodes[idx]
    pad = "  "^indent
    pad2 = "  "^(indent + 1)

    children = _sorted_children_with_path(model, idx)

    io = IOBuffer()
    print(io, "{\n")
    print(io, pad2, "\"path\": ", _json_escape(something(node.path, "")), ",\n")
    print(io, pad2, "\"code\": ", _json_escape(node.code), ",\n")
    print(io, pad2, "\"name\": ", _json_escape(node.name), ",\n")
    print(
        io,
        pad2,
        "\"commonName\": ",
        node.common_name !== nothing ? _json_escape(node.common_name) : "null",
        ",\n",
    )
    print(io, pad2, "\"displayName\": ", _json_escape(node.display_name), ",\n")
    print(io, pad2, "\"category\": ", _json_escape(node.category), ",\n")
    print(io, pad2, "\"type\": ", _json_escape(node.type_), ",\n")
    print(
        io,
        pad2,
        "\"location\": ",
        node.location !== nothing ? _json_escape(node.location) : "null",
        ",\n",
    )

    print(io, pad2, "\"children\": [")
    if isempty(children)
        print(io, "]\n")
    else
        print(io, "\n")
        for (i, ci) in enumerate(children)
            print(io, pad2, "  ")
            print(io, _node_to_json(model, ci, indent + 2))
            i < length(children) && print(io, ",")
            print(io, "\n")
        end
        print(io, pad2, "]\n")
    end
    print(io, pad)
    print(io, "}")
    String(take!(io))
end

function to_json(model::AssetModel)
    model.root === nothing ? "{}" : _node_to_json(model, model.root, 0)
end

function print_tree(model::AssetModel)
    model.root !== nothing && _print_node(model, model.root, "", true, true)
end

function _print_node(
    model::AssetModel,
    idx::Int,
    prefix::String,
    is_last::Bool,
    is_root::Bool,
)
    node = model.nodes[idx]

    if !is_root
        print(prefix, is_last ? "└─ " : "├─ ")
    end
    print(node.code)
    if node.location !== nothing
        print(" [", node.location, "]")
    end
    println(": ", node.display_name)

    children = _sorted_children_with_path(model, idx)

    for (i, ci) in enumerate(children)
        child_is_last = i == length(children)
        new_prefix = is_root ? prefix : prefix * (is_last ? "   " : "│  ")
        _print_node(model, ci, new_prefix, child_is_last, false)
    end
end

let
    println("1. AssetModel: Building from equipment paths (dual-engine vessel)")
    println("--------------------------------------------------------------------")

    v = vis()
    vis_version = latest(v)

    asset_paths = [
        "411.1-P/C101.31-1",
        "411.1-P/C101.31-2",
        "411.1-P/C101.31-3",
        "411.1-P/C101.31-4",
        "411.1-P/C101.31-5",
        "411.1-P/C101.31-6",
        "411.1-P/C101.63/S206",
        "411.1-S/C101.31-1",
        "411.1-S/C101.31-2",
        "411.1-S/C101.31-3",
        "411.1-S/C101.31-4",
        "411.1-S/C101.31-5",
        "411.1-S/C101.31-6",
        "411.1-S/C101.63/S206",
        "511.11-1/C101",
        "511.11-2/C101",
        "621.21/S90",
    ]

    println("Defining $(length(asset_paths)) equipment paths")
    println("(Each path implicitly includes all parent nodes)\n")

    model = asset_model_from_path_strings(vis_version, asset_paths)

    println("Built model with $(node_count(model)) total nodes")
    println("Maximum depth: $(max_depth(model))\n")

    engines = nodes_by_code(model, "C101")
    println("Lookup: All Engines (C101)")
    println("Found $(length(engines)) instances of C101 (Internal combustion engine):")
    for idx in engines
        p = model.nodes[idx].path
        p !== nothing && println("  - $p")
    end

    println("\nLookup: All Cylinders (C101.31)")
    cylinders = nodes_by_code(model, "C101.31")
    println("Found $(length(cylinders)) instances of C101.31 (Cylinder):")
    for (count, idx) in enumerate(cylinders)
        count > 6 && break
        p = model.nodes[idx].path
        p !== nothing && println("  - $p")
    end
    if length(cylinders) > 6
        println("  ... and $(length(cylinders) - 6) more")
    end

    println("\nAll available codes in model:")
    codes = collect(keys(model.nodes_by_code))
    println("$(length(codes)) unique codes: [$(join(codes, ", "))]")

    println("\nModel Metadata:")
    println("VIS Version  : $(model.vis_version)")
    println("Total nodes  : $(node_count(model))")
    println("Maximum depth: $(max_depth(model))\n")

    println("Asset Model Tree:\n")
    print_tree(model)

    println()
end

let
    println("2. AssetModel: Building from LocalIds (runtime flow)")
    println("-------------------------------------------------------")

    v = vis()
    vis_version = latest(v)

    local_id_strings = [
        "/dnv-v2/vis-3-4a/411.1-1/C101.31-1/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
        "/dnv-v2/vis-3-4a/411.1-1/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
        "/dnv-v2/vis-3-4a/411.1-1/C101.31-3/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
        "/dnv-v2/vis-3-4a/411.1-2/C101.31-1/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
        "/dnv-v2/vis-3-4a/411.1-2/C101.31-2/meta/qty-temperature/cnt-exhaust.gas/pos-inlet",
        "/dnv-v2/vis-3-4a/511.11-1/C101/meta/qty-revolution",
    ]

    println("Received $(length(local_id_strings)) LocalIds from data source\n")

    g = gmod(v, vis_version)
    locs = locations(v, vis_version)
    paths = []

    for lid_str in local_id_strings
        local_id = from_string(LocalId, lid_str)
        local_id === nothing && continue

        full = to_full_path_string(primary_item(local_id))
        p = from_full_path(full, g, locs)
        p !== nothing && push!(paths, p)

        sec = secondary_item(local_id)
        if sec !== nothing
            full2 = to_full_path_string(sec)
            p2 = from_full_path(full2, g, locs)
            p2 !== nothing && push!(paths, p2)
        end
    end

    println("Extracted $(length(paths)) GmodPaths from LocalIds")

    model = asset_model_from_paths(vis_version, paths, g, locs)

    println("Built model with $(node_count(model)) nodes")
    println("Maximum depth: $(max_depth(model))\n")

    println("Asset Model Tree (derived from LocalIds):\n")
    print_tree(model)

    println()
end

let
    println("3. AssetModel: JSON export for visualization")
    println("-----------------------------------------------")

    v = vis()
    vis_version = latest(v)

    asset_paths = ["411.1/C101.31-1", "411.1/C101.31-2"]

    model = asset_model_from_path_strings(vis_version, asset_paths)

    println("JSON output (for D3.js, visualization tools, etc.):\n")
    println(to_json(model))

    println()
end
