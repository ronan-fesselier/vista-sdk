function ffi_relative_location_code(rl::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_relative_location_code, VISTA_LIB), Cchar, (Ptr{Cvoid},), rl)
end

function ffi_relative_location_name(rl::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_relative_location_name, VISTA_LIB), Cstring, (Ptr{Cvoid},), rl)
end

function ffi_relative_location_definition(rl::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_relative_location_definition, VISTA_LIB),
        Cstring,
        (Ptr{Cvoid},),
        rl,
    )
end

function ffi_relative_location_location(rl::Ptr{Cvoid})
    ccall(
        (:dnv_vista_sdk_relative_location_location, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid},),
        rl,
    )
end
