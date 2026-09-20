function ffi_universal_id_naming_entity()
    ccall((:dnv_vista_sdk_universal_id_naming_entity, VISTA_LIB), Cstring, ())
end

function ffi_universal_id_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_universal_id_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_universal_id_from_string(s::Cstring)
    ccall((:dnv_vista_sdk_universal_id_from_string, VISTA_LIB), Ptr{Cvoid}, (Cstring,), s)
end

function ffi_universal_id_local_id(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_universal_id_local_id, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), ptr)
end

function ffi_universal_id_builder(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_universal_id_builder, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), ptr)
end

function ffi_universal_id_equals(a::Ptr{Cvoid}, b::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_universal_id_equals, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        a,
        b,
    )
end

function ffi_universal_id_to_string(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_universal_id_to_string, VISTA_LIB),
        Ptr{UInt8},
        (Ptr{Cvoid},),
        ptr,
    )
end
