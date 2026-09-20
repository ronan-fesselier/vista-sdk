function ffi_local_id_naming_rule()
    ccall((:dnv_vista_sdk_local_id_naming_rule, VISTA_LIB), Cstring, ())
end

function ffi_local_id_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_local_id_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_local_id_from_string(s::Cstring)
    ccall((:dnv_vista_sdk_local_id_from_string, VISTA_LIB), Ptr{Cvoid}, (Cstring,), s)
end

function ffi_local_id_from_string_with_errors(s::Cstring, out_errors::Ref{Ptr{Cvoid}})
    ccall(
        (:dnv_vista_sdk_local_id_from_string_with_errors, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring, Ptr{Ptr{Cvoid}}),
        s,
        out_errors,
    )
end

function ffi_local_id_version(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_local_id_version, VISTA_LIB), Cstring, (Ptr{Cvoid},), ptr)
end

function ffi_local_id_primary_item(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_local_id_primary_item, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), ptr)
end

function ffi_local_id_secondary_item(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_local_id_secondary_item, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_local_id_is_verbose_mode(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_local_id_is_verbose_mode, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end

function ffi_local_id_has_custom_tag(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_local_id_has_custom_tag, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end

function ffi_local_id_equals(a::Ptr{Cvoid}, b::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_local_id_equals, VISTA_LIB), Cint, (Ptr{Cvoid}, Ptr{Cvoid}), a, b)
end

function ffi_local_id_metadata_tag(ptr::Ptr{Cvoid}, name::Cint)
    ccall(
        (:dnv_vista_sdk_local_id_metadata_tag, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cint),
        ptr,
        name,
    )
end

function ffi_local_id_builder(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_local_id_builder, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), ptr)
end

function ffi_local_id_to_string(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_local_id_to_string, VISTA_LIB), Ptr{UInt8}, (Ptr{Cvoid},), ptr)
end
