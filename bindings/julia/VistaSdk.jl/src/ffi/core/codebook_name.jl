function ffi_codebook_name_from_prefix(prefix::Cstring, out::Ref{Cint})
    ccall(
        (:dnv_vista_sdk_codebook_names_from_prefix, VISTA_LIB),
        Cint,
        (Cstring, Ptr{Cint}),
        prefix,
        out,
    )
end

function ffi_codebook_name_to_prefix(name::Cint)
    ccall((:dnv_vista_sdk_codebook_names_to_prefix, VISTA_LIB), Cstring, (Cint,), name)
end

function ffi_codebook_name_to_string(name::Cint)
    ccall((:dnv_vista_sdk_codebook_names_to_string, VISTA_LIB), Cstring, (Cint,), name)
end
