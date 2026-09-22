function ffi_gmod_path_query_builder_create()
    ccall((:dnv_vista_sdk_gmod_path_query_builder_create, VISTA_LIB), Ptr{Cvoid}, ())
end

function ffi_gmod_path_query_builder_from(path::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_path_query_builder_from, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        path,
    )
end

function ffi_gmod_path_query_builder_free(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_path_query_builder_free, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_gmod_path_query_builder_path(builder::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_path_query_builder_path, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        builder,
    )
end

function ffi_gmod_path_query_builder_path_with_node_all_locations(
    builder::Ptr{Cvoid},
    code::Cstring,
    match_all_locations::Cint,
)
    ccall(
        (:dnv_vista_sdk_gmod_path_query_builder_path_with_node_all_locations, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cstring, Cint),
        builder,
        code,
        match_all_locations,
    )
end

function ffi_gmod_path_query_builder_path_with_node_locations(
    builder::Ptr{Cvoid},
    code::Cstring,
    locations::Ptr{Ptr{Cvoid}},
    location_count::Csize_t,
)
    ccall(
        (:dnv_vista_sdk_gmod_path_query_builder_path_with_node_locations, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cstring, Ptr{Ptr{Cvoid}}, Csize_t),
        builder,
        code,
        locations,
        location_count,
    )
end

function ffi_gmod_path_query_builder_with_any_node_before(
    builder::Ptr{Cvoid},
    code::Cstring,
)
    ccall(
        (:dnv_vista_sdk_gmod_path_query_builder_with_any_node_before, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cstring),
        builder,
        code,
    )
end

function ffi_gmod_path_query_builder_with_any_node_after(builder::Ptr{Cvoid}, code::Cstring)
    ccall(
        (:dnv_vista_sdk_gmod_path_query_builder_with_any_node_after, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cstring),
        builder,
        code,
    )
end

function ffi_gmod_path_query_builder_without_locations(builder::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_path_query_builder_without_locations, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        builder,
    )
end

function ffi_gmod_path_query_builder_with_node_all_locations(
    builder::Ptr{Cvoid},
    node::Ptr{Cvoid},
    match_all_locations::Cint,
)
    ccall(
        (:dnv_vista_sdk_gmod_path_query_builder_with_node_all_locations, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}, Cint),
        builder,
        node,
        match_all_locations,
    )
end

function ffi_gmod_path_query_builder_with_node_locations(
    builder::Ptr{Cvoid},
    node::Ptr{Cvoid},
    locations::Ptr{Ptr{Cvoid}},
    location_count::Csize_t,
)
    ccall(
        (:dnv_vista_sdk_gmod_path_query_builder_with_node_locations, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}, Ptr{Ptr{Cvoid}}, Csize_t),
        builder,
        node,
        locations,
        location_count,
    )
end

function ffi_gmod_path_query_builder_build(builder::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_path_query_builder_build, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        builder,
    )
end
