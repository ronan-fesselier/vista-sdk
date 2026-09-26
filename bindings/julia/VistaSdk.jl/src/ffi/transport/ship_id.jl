function ffi_ship_id_from_imo_number(imo_ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_ship_id_from_imo_number, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        imo_ptr,
    )
end

function ffi_ship_id_from_other_id(other_id::Cstring)
    ccall(
        (:dnv_vista_sdk_ship_id_from_other_id, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring,),
        other_id,
    )
end

function ffi_ship_id_from_string(value::Cstring)
    ccall((:dnv_vista_sdk_ship_id_from_string, VISTA_LIB), Ptr{Cvoid}, (Cstring,), value)
end

function ffi_ship_id_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_ship_id_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_ship_id_equals(a::Ptr{Cvoid}, b::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_ship_id_equals, VISTA_LIB), Cint, (Ptr{Cvoid}, Ptr{Cvoid}), a, b)
end

function ffi_ship_id_is_imo_number(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_ship_id_is_imo_number, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end

function ffi_ship_id_is_other_id(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_ship_id_is_other_id, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end

function ffi_ship_id_imo_number(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_ship_id_imo_number, VISTA_LIB), Ptr{Cvoid}, (Ptr{Cvoid},), ptr)
end

function ffi_ship_id_other_id(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_ship_id_other_id, VISTA_LIB), Cstring, (Ptr{Cvoid},), ptr)
end

function ffi_ship_id_to_string(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_ship_id_to_string, VISTA_LIB), Ptr{UInt8}, (Ptr{Cvoid},), ptr)
end
