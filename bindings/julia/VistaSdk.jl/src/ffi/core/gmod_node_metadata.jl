function ffi_gmod_node_metadata_category(metadata::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_node_metadata_category, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        metadata,
    )
end

function ffi_gmod_node_metadata_type(metadata::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_node_metadata_type, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        metadata,
    )
end

function ffi_gmod_node_metadata_full_type(metadata::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_node_metadata_full_type, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        metadata,
    )
end

function ffi_gmod_node_metadata_name(metadata::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_node_metadata_name, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        metadata,
    )
end

function ffi_gmod_node_metadata_common_name(metadata::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_node_metadata_common_name, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        metadata,
    )
end

function ffi_gmod_node_metadata_definition(metadata::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_node_metadata_definition, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        metadata,
    )
end

function ffi_gmod_node_metadata_common_definition(metadata::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_node_metadata_common_definition, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        metadata,
    )
end

function ffi_gmod_node_metadata_install_substructure(
    metadata::Ptr{Cvoid},
    out_value::Ref{Cint},
)
    ccall(
        (:dnv_vista_sdk_gmod_node_metadata_install_substructure, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ptr{Cint}),
        metadata,
        out_value,
    )
end

function ffi_gmod_node_metadata_normal_assignment_name_count(metadata::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_count, VISTA_LIB),
        Csize_t,
        (Ptr{Cvoid},),
        metadata,
    )
end

function ffi_gmod_node_metadata_normal_assignment_name_key_at(
    metadata::Ptr{Cvoid},
    index::Csize_t,
)
    ccall(
        (:dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_key_at, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid}, Csize_t),
        metadata,
        index,
    )
end

function ffi_gmod_node_metadata_normal_assignment_name_value_at(
    metadata::Ptr{Cvoid},
    index::Csize_t,
)
    ccall(
        (:dnv_vista_sdk_gmod_node_metadata_normal_assignment_name_value_at, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid}, Csize_t),
        metadata,
        index,
    )
end
