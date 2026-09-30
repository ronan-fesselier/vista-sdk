function ffi_tsd_channel_id_from_string(value::Cstring)
    ccall(
        (:dnv_vista_sdk_tsd_channel_id_from_string, VISTA_LIB),
        Ptr{Cvoid},
        (Cstring,),
        value,
    )
end

function ffi_tsd_channel_id_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_channel_id_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_channel_id_equals(a::Ptr{Cvoid}, b::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_channel_id_equals, VISTA_LIB),
        Cint,
        (Ptr{Cvoid}, Ptr{Cvoid}),
        a,
        b,
    )
end

function ffi_tsd_channel_id_is_local_id(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_channel_id_is_local_id, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_channel_id_is_short_id(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_channel_id_is_short_id, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_channel_id_local_id(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_channel_id_local_id, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        ptr,
    )
end

function ffi_tsd_channel_id_short_id(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_tsd_channel_id_short_id, VISTA_LIB), Cstring, (Ptr{Cvoid},), ptr)
end

function ffi_tsd_channel_id_to_string(ptr::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_tsd_channel_id_to_string, VISTA_LIB),
        Ptr{UInt8},
        (Ptr{Cvoid},),
        ptr,
    )
end
