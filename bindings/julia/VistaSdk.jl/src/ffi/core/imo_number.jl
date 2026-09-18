function ffi_imo_number_is_valid(value::Cint)
    ccall((:dnv_vista_sdk_imo_number_is_valid, VISTA_LIB), Cint, (Cint,), value)
end

function ffi_imo_number_create(value::Cint)
    ccall((:dnv_vista_sdk_imo_number_create, VISTA_LIB), Ptr{Cvoid}, (Cint,), value)
end

function ffi_imo_number_from_string(value::Cstring)
    ccall((:dnv_vista_sdk_imo_number_from_string, VISTA_LIB), Ptr{Cvoid}, (Cstring,), value)
end

function ffi_imo_number_free(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_imo_number_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), ptr)
end

function ffi_imo_number_value(ptr::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_imo_number_value, VISTA_LIB), Cint, (Ptr{Cvoid},), ptr)
end
