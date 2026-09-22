function ffi_local_id_query_builder_create()
    ccall((:dnv_vista_sdk_local_id_query_builder_create, VISTA_LIB), Ptr{Cvoid}, ())
end

function ffi_local_id_query_builder_from(local_id::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_query_builder_from, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        local_id,
    )
end

function ffi_local_id_query_builder_from_string(local_id_str::Cstring)
    ccall(
        (:dnv_vista_sdk_local_id_query_builder_from_string, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring,),
        local_id_str,
    )
end

function ffi_local_id_query_builder_free(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_query_builder_free, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_local_id_query_builder_with_primary_item(
    builder::Ptr{Cvoid},
    primary_item::Ptr{Cvoid},
)
    ccall(
        (:dnv_vista_sdk_local_id_query_builder_with_primary_item, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}),
        builder,
        primary_item,
    )
end

function ffi_local_id_query_builder_with_primary_item_query(
    builder::Ptr{Cvoid},
    primary_item::Ptr{Cvoid},
)
    ccall(
        (:dnv_vista_sdk_local_id_query_builder_with_primary_item_query, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}),
        builder,
        primary_item,
    )
end

function ffi_local_id_query_builder_with_secondary_item(
    builder::Ptr{Cvoid},
    secondary_item::Ptr{Cvoid},
)
    ccall(
        (:dnv_vista_sdk_local_id_query_builder_with_secondary_item, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}),
        builder,
        secondary_item,
    )
end

function ffi_local_id_query_builder_with_secondary_item_query(
    builder::Ptr{Cvoid},
    secondary_item::Ptr{Cvoid},
)
    ccall(
        (:dnv_vista_sdk_local_id_query_builder_with_secondary_item_query, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}),
        builder,
        secondary_item,
    )
end

function ffi_local_id_query_builder_with_primary_item_nodes_builder(
    builder::Ptr{Cvoid},
    nodes_builder::Ptr{Cvoid},
)
    ccall(
        (:dnv_vista_sdk_local_id_query_builder_with_primary_item_nodes_builder, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}),
        builder,
        nodes_builder,
    )
end

function ffi_local_id_query_builder_with_primary_item_path_builder(
    builder::Ptr{Cvoid},
    path_builder::Ptr{Cvoid},
)
    ccall(
        (:dnv_vista_sdk_local_id_query_builder_with_primary_item_path_builder, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}),
        builder,
        path_builder,
    )
end

function ffi_local_id_query_builder_with_any_secondary_item(builder::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_query_builder_with_any_secondary_item, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        builder,
    )
end

function ffi_local_id_query_builder_without_secondary_item(builder::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_query_builder_without_secondary_item, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        builder,
    )
end

function ffi_local_id_query_builder_with_secondary_item_nodes_builder(
    builder::Ptr{Cvoid},
    nodes_builder::Ptr{Cvoid},
)
    ccall(
        (
            :dnv_vista_sdk_local_id_query_builder_with_secondary_item_nodes_builder,
            VISTA_LIB,
        ),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}),
        builder,
        nodes_builder,
    )
end

function ffi_local_id_query_builder_with_secondary_item_path_builder(
    builder::Ptr{Cvoid},
    path_builder::Ptr{Cvoid},
)
    ccall(
        (:dnv_vista_sdk_local_id_query_builder_with_secondary_item_path_builder, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}),
        builder,
        path_builder,
    )
end

function ffi_local_id_query_builder_with_tags(builder::Ptr{Cvoid}, tags::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_query_builder_with_tags, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}),
        builder,
        tags,
    )
end

function ffi_local_id_query_builder_without_locations(builder::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_query_builder_without_locations, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        builder,
    )
end

function ffi_local_id_query_builder_primary_item(builder::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_query_builder_primary_item, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        builder,
    )
end

function ffi_local_id_query_builder_secondary_item(builder::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_query_builder_secondary_item, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        builder,
    )
end

function ffi_local_id_query_builder_tags_builder(builder::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_query_builder_tags_builder, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        builder,
    )
end

function ffi_local_id_query_builder_build(builder::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_query_builder_build, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        builder,
    )
end
