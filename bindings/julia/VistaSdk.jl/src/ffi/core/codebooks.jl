function ffi_codebooks_version(codebooks::Ptr{Cvoid})
    ccall((:dnv_vista_sdk_codebooks_version, VISTA_LIB), Cstring, (Ptr{Cvoid},), codebooks)
end

function ffi_codebooks_at(codebooks::Ptr{Cvoid}, name::Cint)
    ccall(
        (:dnv_vista_sdk_codebooks_at, VISTA_LIB),
        Ptr{Cvoid},
        (Ptr{Cvoid}, Cint),
        codebooks,
        name,
    )
end
