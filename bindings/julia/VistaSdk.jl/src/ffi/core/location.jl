function ffi_location_free(location::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_location_free, VISTA_LIB), Cvoid, (Ptr{Cvoid},), location)
end

function ffi_location_value(location::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_location_value, VISTA_LIB), Cstring, (Ptr{Cvoid},), location)
end
