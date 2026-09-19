function ffi_gmod_node_free(node::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_node_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), node)
end

function ffi_gmod_node_version(node::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_node_version, VISTA_LIB), Cstring, (Ptr{Cvoid},), node)
end

function ffi_gmod_node_code(node::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_node_code, VISTA_LIB), Cstring, (Ptr{Cvoid},), node)
end

function ffi_gmod_node_location(node::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_node_location, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), node)
end

function ffi_gmod_node_metadata(node::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_node_metadata, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), node)
end

function ffi_gmod_node_child_count(node::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_node_child_count, VISTA_LIB), Csize_t, (Ptr{Cvoid},), node)
end

function ffi_gmod_node_child_at(node::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_gmod_node_child_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        node,
        index,
    )
end

function ffi_gmod_node_parent_count(node::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_node_parent_count, VISTA_LIB), Csize_t, (Ptr{Cvoid},), node)
end

function ffi_gmod_node_parent_at(node::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_gmod_node_parent_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        node,
        index,
    )
end

function ffi_gmod_node_product_type(node::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_node_product_type, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        node,
    )
end

function ffi_gmod_node_product_selection(node::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_node_product_selection, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        node,
    )
end

function ffi_gmod_node_is_function_composition(node::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_node_is_function_composition, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        node,
    )
end

function ffi_gmod_node_is_mappable(node::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_node_is_mappable, VISTA_LIB), Cint, (Ptr{Cvoid},), node)
end

function ffi_gmod_node_is_product_selection(node::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_node_is_product_selection, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        node,
    )
end

function ffi_gmod_node_is_product_type(node::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_node_is_product_type, VISTA_LIB), Cint, (Ptr{Cvoid},), node)
end

function ffi_gmod_node_is_asset(node::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_node_is_asset, VISTA_LIB), Cint, (Ptr{Cvoid},), node)
end

function ffi_gmod_node_is_leaf_node(node::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_node_is_leaf_node, VISTA_LIB), Cint, (Ptr{Cvoid},), node)
end

function ffi_gmod_node_is_function_node(node::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_node_is_function_node, VISTA_LIB), Cint, (Ptr{Cvoid},), node)
end

function ffi_gmod_node_is_asset_function_node(node::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_node_is_asset_function_node, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        node,
    )
end

function ffi_gmod_node_is_root(node::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_node_is_root, VISTA_LIB), Cint, (Ptr{Cvoid},), node)
end

function ffi_gmod_node_is_child(node::Ptr{Cvoid}, other::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_node_is_child, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        node,
        other,
    )
end

function ffi_gmod_node_is_child_code(node::Ptr{Cvoid}, code::Cstring)
    ccall(
        (:dnv_vista_sdk_gmod_node_is_child_code, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Cstring),
        node,
        code,
    )
end

function ffi_gmod_node_to_string(node::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_gmod_node_to_string, VISTA_LIB), Ptr{UInt8}, (Ptr{Cvoid},), node)
end
