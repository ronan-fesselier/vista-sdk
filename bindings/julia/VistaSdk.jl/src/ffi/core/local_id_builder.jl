function ffi_local_id_builder_naming_rule()
    ccall((:dnv_vista_sdk_local_id_builder_naming_rule, VISTA_LIB), Cstring, ())
end

function ffi_local_id_builder_create(vis_version::Cstring)
    ccall(
        (:dnv_vista_sdk_local_id_builder_create, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring,),
        vis_version,
    )
end

function ffi_local_id_builder_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_local_id_builder_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_local_id_builder_version(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_local_id_builder_version, VISTA_LIB), Cstring, (Ptr{Cvoid},), ptr)
end

function ffi_local_id_builder_is_verbose_mode(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_builder_is_verbose_mode, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_local_id_builder_is_valid(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_local_id_builder_is_valid, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end

function ffi_local_id_builder_is_empty(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_local_id_builder_is_empty, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end

function ffi_local_id_builder_is_empty_metadata(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_builder_is_empty_metadata, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_local_id_builder_has_custom_tag(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_builder_has_custom_tag, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_local_id_builder_equals(a::Ptr{Cvoid}, b::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_builder_equals, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        a,
        b,
    )
end

function ffi_local_id_builder_primary_item(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_builder_primary_item, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_local_id_builder_secondary_item(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_builder_secondary_item, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_local_id_builder_metadata_tag(ptr::Ptr{Cvoid}, name::Cint)
    ccall(
        (:dnv_vista_sdk_local_id_builder_metadata_tag, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cint),
        ptr,
        name,
    )
end

function ffi_local_id_builder_with_vis_version(ptr::Ptr{Cvoid}, vis_version::Cstring)
    ccall(
        (:dnv_vista_sdk_local_id_builder_with_vis_version, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cstring),
        ptr,
        vis_version,
    )
end

function ffi_local_id_builder_without_vis_version(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_builder_without_vis_version, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_local_id_builder_with_primary_item(ptr::Ptr{Cvoid}, path::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_builder_with_primary_item, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}),
        ptr,
        path,
    )
end

function ffi_local_id_builder_without_primary_item(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_builder_without_primary_item, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_local_id_builder_with_secondary_item(ptr::Ptr{Cvoid}, path::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_builder_with_secondary_item, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}),
        ptr,
        path,
    )
end

function ffi_local_id_builder_without_secondary_item(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_builder_without_secondary_item, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_local_id_builder_with_metadata_tag(ptr::Ptr{Cvoid}, tag::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_builder_with_metadata_tag, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}),
        ptr,
        tag,
    )
end

function ffi_local_id_builder_without_metadata_tag(ptr::Ptr{Cvoid}, name::Cint)
    ccall(
        (:dnv_vista_sdk_local_id_builder_without_metadata_tag, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cint),
        ptr,
        name,
    )
end

function ffi_local_id_builder_with_verbose_mode(ptr::Ptr{Cvoid}, verbose::Cint)
    ccall(
        (:dnv_vista_sdk_local_id_builder_with_verbose_mode, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cint),
        ptr,
        verbose,
    )
end

function ffi_local_id_builder_build(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_builder_build, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_local_id_builder_to_string(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_builder_to_string, VISTA_LIB),
        Ptr{UInt8},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_local_id_builder_from_string(s::Cstring)
    ccall(
        (:dnv_vista_sdk_local_id_builder_from_string, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring,),
        s,
    )
end

function ffi_local_id_builder_from_string_with_errors(
    s::Cstring,
    out_errors::Ref{Ptr{Cvoid}},
)
    ccall(
        (:dnv_vista_sdk_local_id_builder_from_string_with_errors, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring, Ptr{Ptr{Cvoid}}),
        s,
        out_errors,
    )
end
