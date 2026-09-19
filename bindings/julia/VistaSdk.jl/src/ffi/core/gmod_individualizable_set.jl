function ffi_gmod_individualizable_set_create(
    node_indices::Ptr{Cint},
    count::Csize_t,
    source_path::Ptr{Cvoid},
)
    ccall(
        (:dnv_vista_sdk_gmod_individualizable_set_create, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cint}, Csize_t, Ptr{Cvoid}),
        node_indices,
        count,
        source_path,
    )
end

function ffi_gmod_individualizable_set_free(set::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_individualizable_set_free, VISTA_LIB),
        Cvoid,
        (Ptr{Cvoid},),
        set,
    )
end

function ffi_gmod_individualizable_set_build(set::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_individualizable_set_build, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        set,
    )
end

function ffi_gmod_individualizable_set_node_count(set::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_individualizable_set_node_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        set,
    )
end

function ffi_gmod_individualizable_set_node_at(set::Ptr{Cvoid}, index::Csize_t)
    ccall(
        (:dnv_vista_sdk_gmod_individualizable_set_node_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Csize_t),
        set,
        index,
    )
end

function ffi_gmod_individualizable_set_index_count(set::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_individualizable_set_index_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        set,
    )
end

function ffi_gmod_individualizable_set_index_at(
    set::Ptr{Cvoid},
    position::Csize_t,
    out_index::Ref{Cint},
)
    ccall(
        (:dnv_vista_sdk_gmod_individualizable_set_index_at, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Csize_t, Ptr{Cint}),
        set,
        position,
        out_index,
    )
end

function ffi_gmod_individualizable_set_location(set::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_individualizable_set_location, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        set,
    )
end

function ffi_gmod_individualizable_set_to_string(set::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_individualizable_set_to_string, VISTA_LIB),
        Ptr{UInt8},
        (Ptr{Cvoid},),
        set,
    )
end
