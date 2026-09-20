function ffi_universal_id_builder_naming_entity()
    ccall((:dnv_vista_sdk_universal_id_builder_naming_entity, VISTA_LIB), Cstring, ())
end

function ffi_universal_id_builder_create(vis_version::Cstring)
    ccall(
        (:dnv_vista_sdk_universal_id_builder_create, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring,),
        vis_version,
    )
end

function ffi_universal_id_builder_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_universal_id_builder_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_universal_id_builder_imo_number(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_universal_id_builder_imo_number, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_universal_id_builder_local_id(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_universal_id_builder_local_id, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_universal_id_builder_is_valid(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_universal_id_builder_is_valid, VISTA_LIB),
        Cint,
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_universal_id_builder_equals(a::Ptr{Cvoid}, b::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_universal_id_builder_equals, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        a,
        b,
    )
end

function ffi_universal_id_builder_with_imo_number(ptr::Ptr{Cvoid}, imo::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_universal_id_builder_with_imo_number, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}),
        ptr,
        imo,
    )
end

function ffi_universal_id_builder_without_imo_number(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_universal_id_builder_without_imo_number, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_universal_id_builder_with_local_id(
    ptr::Ptr{Cvoid},
    local_id_builder::Ptr{Cvoid},
)
    ccall(
        (:dnv_vista_sdk_universal_id_builder_with_local_id, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Ptr{Cvoid}),
        ptr,
        local_id_builder,
    )
end

function ffi_universal_id_builder_without_local_id(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_universal_id_builder_without_local_id, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_universal_id_builder_build(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_universal_id_builder_build, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_universal_id_builder_to_string(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_universal_id_builder_to_string, VISTA_LIB),
        Ptr{UInt8},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_universal_id_builder_from_string(s::Cstring)
    ccall(
        (:dnv_vista_sdk_universal_id_builder_from_string, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring,),
        s,
    )
end

function ffi_universal_id_builder_from_string_with_errors(
    s::Cstring,
    out_errors::Ref{Ptr{Cvoid}},
)
    ccall(
        (:dnv_vista_sdk_universal_id_builder_from_string_with_errors, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring, Ptr{Ptr{Cvoid}}),
        s,
        out_errors,
    )
end
